# include<iostream>
# include<windows.h>
using namespace std;


struct buf
{
    int remain;
    char data[3][1024];
};


const LPCSTR semEmptyName = (LPCSTR)"sememptyname";
const LPCSTR semOpName = (LPCSTR)"semopname";
const LPCSTR semFullName = (LPCSTR)"semfullname";
const LPCSTR sfmBuffName = (LPCSTR)"sfmbufname";


HANDLE MakeSharedFile()
{
    HANDLE hMapping = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, sizeof(buf), sfmBuffName);
    buf* p_buf = (buf*)MapViewOfFile(hMapping, FILE_MAP_ALL_ACCESS, 0, 0, 0);
    p_buf->remain = 0;
    return hMapping;
}


int main()
{
    HANDLE semEmpty = CreateSemaphore(NULL, 3, 3, semEmptyName);
    HANDLE semOp = CreateSemaphore(NULL, 1, 1, semOpName);
    HANDLE semFull = CreateSemaphore(NULL, 0, 3, semFullName);
    HANDLE sfmaping = MakeSharedFile();

    PROCESS_INFORMATION pis[5];
    // create producer processes
    for (int i = 0; i < 2; ++i)
    {
        STARTUPINFO si;
        memset(&si, 0, sizeof(STARTUPINFO));
        si.dwFlags = STARTF_USESHOWWINDOW;
        si.wShowWindow = SW_SHOW;
        TCHAR* cmdLine = (TCHAR*)"./producer.exe sememptyname semopname semfullname sfmbufname";
        if (!CreateProcess((TCHAR*)"./producer.exe", cmdLine, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pis[i]))
        {
            cout << "create process \"producer\" failed. error code: " << GetLastError() << endl;
            return -1;
        }
        
    }
    // create consumer processes
    for (int i = 2; i < 5; ++i)
    {
        STARTUPINFO si;
        memset(&si, 0, sizeof(STARTUPINFO));
        si.dwFlags = STARTF_USESHOWWINDOW;
        si.wShowWindow = SW_SHOW;
        TCHAR* cmdLine = (TCHAR*)"./consumer.exe sememptyname semopname semfullname sfmbufname";
        if (!CreateProcess((TCHAR*)"./consumer.exe", cmdLine, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pis[i]))
        {
            cout << "create process \"consumer\" failed. error code: " << GetLastError() << endl;
            return -1;
        }
    }

    for (int i = 0; i < 5; ++i)
    {
        WaitForSingleObject(pis[i].hProcess, INFINITE);
        CloseHandle(pis[i].hProcess);
        CloseHandle(pis[i].hThread);
    }
    CloseHandle(semEmpty);
    CloseHandle(semOp);
    CloseHandle(semFull);
    CloseHandle(sfmaping);
    return 0;
}