# include<iostream>
# include<windows.h>
# include<cstring>
using namespace std;


 const LPCTSTR g_szContinueEvent = (LPCTSTR)"w2kdg.EventDemo.event.Continue";


 bool CreateChild()
 {
     TCHAR szFilename[MAX_PATH];
     GetModuleFileName(nullptr, szFilename, MAX_PATH);
     TCHAR szComdLine[MAX_PATH];
     sprintf((char*)szComdLine, "%s child", szFilename);
     STARTUPINFO si;
     ZeroMemory(&si, sizeof(si));
     si.cb = sizeof(si);

     PROCESS_INFORMATION pi;
     BOOL bCreateOK = CreateProcess(szFilename, szComdLine, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
     if (bCreateOK)
     {
         CloseHandle(pi.hProcess);
         CloseHandle(pi.hThread);
     }
     return bCreateOK;
 }


void wait_for_child()
{
    HANDLE hEventContinue = CreateEvent(nullptr, true, false, g_szContinueEvent);
    CreateChild();
    WaitForSingleObject(hEventContinue, INFINITE);
    CloseHandle(hEventContinue);
}

void signal_parent()
{
    HANDLE hEventContinue = OpenEvent(EVENT_MODIFY_STATE, FALSE, g_szContinueEvent);
    if (hEventContinue != NULL)
    {
        cout << "child process begining..." << endl;
        SetEvent(hEventContinue);
        CloseHandle(hEventContinue);
    }
}


int main(int argc, char* argv[])
{
    if (argc > 1 && strcmp(argv[1], "child") == 0)
    {
        signal_parent();
    }
    else
    {
        wait_for_child();
        cout << "parent process released." << endl;
    }
    return 0;
}