# include<iostream>
# include<windows.h>
using namespace std;


HANDLE g_hSemThreads;


DWORD WINAPI ThreadProc(LPVOID lpParam)
{
    int nPauseMs = (int)lpParam;
    Sleep(nPauseMs);
    LONG nPrevCt(0);
    ReleaseSemaphore(g_hSemThreads, 1, &nPrevCt);
    cout << "previous sem-count: " << nPrevCt << endl;
    return 0;
}


int main()
{
    g_hSemThreads = CreateSemaphore(NULL, 5, 5, NULL);
    for (int i = 100; i > 0; --i)
    {
        cout << "current i is " << i << endl;
        WaitForSingleObject(g_hSemThreads, INFINITE);
        HANDLE hThread = CreateThread(NULL, 0, ThreadProc, (LPVOID)(i * 5), 0, NULL);
        CloseHandle(hThread);
    }
    CloseHandle(g_hSemThreads);
    return 0;
}