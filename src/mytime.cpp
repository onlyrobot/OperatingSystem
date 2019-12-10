# include<iostream>
# include<windows.h>
# include<io.h>
# include<cstdio>
# include<cstdlib>
# include<cstring>
using namespace std;


void display_time(SYSTEMTIME time1, SYSTEMTIME time2)
{
    int total = ((time2.wHour - time1.wHour) * 60 + \
    (time2.wMinute - time1.wMinute) * 60 + \
    (time2.wSecond - time1.wSecond)) * 1000 + \
    time2.wMilliseconds - time1.wMilliseconds;
    int hour = total / (60 * 60 * 1000);
    total %= 60 * 60 * 1000;
    int minute = total / (60 * 1000);
    total %= 60 * 1000;
    int second = total / 1000;
    int ms = total - second * 1000;
    cout << "spend time: " << endl << hour << "h " << \
    minute << "m " << second << "s " << ms << "ms" << endl;
}


int get_abs_path(char* name, char abs_path[])
{
    if (strlen(name) > 1 && (name[1] == ':' || name[0] == '.'))
    {
        strcpy(abs_path, name);
        return 0;
    }
    char* env_path = getenv("path");
    char* p_path = env_path;
    char* last_p = p_path;
    while (*p_path != '\0')
    {
        if (*p_path == ';')
        {
            strncpy(abs_path, last_p, p_path - last_p);
            strcat(abs_path, "\\");
            strcat(abs_path, name);
            if (access(abs_path, 0) != -1)
            {
                return 0;
            }
            last_p = p_path + 1;
        }
        ++p_path;
    }
    return -1;
}


int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        cout << "error: missing parameters" << endl;
        return -1;
    }

    char abs_path[4096];
    if (get_abs_path(argv[1], abs_path) != 0)
    {
        cout << "error: can't find file \"" << argv[1] << "\"" << endl;
        return -1;
    }

    STARTUPINFO si;
    memset(&si, 0, sizeof(STARTUPINFO));
    si.dwFlags = STARTF_USESHOWWINDOW;
    si.wShowWindow = SW_SHOW;
    PROCESS_INFORMATION pi;
    if (!CreateProcess((TCHAR*)abs_path, argc == 2 ? NULL : (TCHAR*)argv[2], NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi))
    {
        cout << "create process failed. error code: " << GetLastError() << endl;
        return -1;
    }
    SYSTEMTIME utcSysTime_before;
    GetSystemTime(&utcSysTime_before);
    DWORD dw = WaitForSingleObject(pi.hProcess, INFINITE);
    SYSTEMTIME utcSysTime_after;
    GetSystemTime(&utcSysTime_after);
    if (dw == WAIT_OBJECT_0)
    {
        cout << "process \"" << argv[1] << "\" ended" << endl;
    } 
    else if (dw == WAIT_TIMEOUT)
    {
        cout << "time out" << endl;
    }
    else if (dw == WAIT_ABANDONED)
    {
        cout << "abandoned" << endl;
    }
    display_time(utcSysTime_before, utcSysTime_after);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    return 0;
}