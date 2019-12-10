# include<iostream>
# include<windows.h>
# include<time.h>
using namespace std;


struct buf
{
    int remain;
    char data[3][1024];
};


buf* parse_share_memory(const char* ipName)
{
    HANDLE hMapping = OpenFileMapping(FILE_MAP_ALL_ACCESS, TRUE, (LPCSTR)ipName);
    buf* p_buf = (buf*)MapViewOfFile(hMapping, FILE_MAP_ALL_ACCESS, 0, 0, 0);
    CloseHandle(hMapping);
    return p_buf;
}


int main(int argc, char* argv[])
{
    if (argc < 5)
    {
        cout << "error: missing param" << endl;
        return -1;
    }
    HANDLE semEmpty = OpenSemaphore(SEMAPHORE_ALL_ACCESS, TRUE, (LPCSTR)argv[1]);
    HANDLE semOp = OpenSemaphore(SEMAPHORE_ALL_ACCESS, TRUE, (LPCSTR)argv[2]);
    HANDLE semFull = OpenSemaphore(SEMAPHORE_ALL_ACCESS, TRUE, (LPCSTR)argv[3]);
    buf* p_buf = parse_share_memory(argv[4]);
    Sleep(1000);
    srand(time(nullptr));
    for (int i = 0; i < 6; ++i)
    {
        Sleep(rand() % 10 * 1000);
        WaitForSingleObject(semEmpty, INFINITE);
        WaitForSingleObject(semOp, INFINITE);
        cout << "produced at time " << \
        time(nullptr) << ", buf remain " << p_buf->remain + 1 << endl;
        p_buf->remain += 1;
        ReleaseSemaphore(semOp, 1, NULL);
        ReleaseSemaphore(semFull, 1, NULL);
    }
    CloseHandle(semEmpty);
    CloseHandle(semOp);
    CloseHandle(semFull);
    return 0;
}