# define UNICODE

# include<iostream>
# include<windows.h>
// # include<cwchar>
# include<cstring>
using namespace std;


// copy file recursively
void copy_all(WCHAR* from, WCHAR* target)
{
    wcscat(from, TEXT("\\"));
    wcscat(target, TEXT("\\"));
    WCHAR temp[1024] = {0};
    wcscat(temp, from);
    wcscat(temp, TEXT("*.*"));
    WIN32_FIND_DATA findData;
    HANDLE findFile = FindFirstFile(temp, &findData);
    do
    {
        if (wcscmp(findData.cFileName, TEXT(".")) == 0)
            continue;
        if (wcscmp(findData.cFileName, TEXT("..")) == 0)
            continue;
        WCHAR subFrom[1024] = {0}, subTarget[1024] = {0};
        
        wcscpy(subFrom, from);
        wcscat(subFrom, findData.cFileName);
        wcscpy(subTarget, target);
        wcscat(subTarget, findData.cFileName);
        if (findData.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY)
        {
            // copy directory and set attributes
            CreateDirectory(subTarget, NULL);
            copy_all(subFrom, subTarget);
            HANDLE targetFile = CreateFile(subTarget, GENERIC_WRITE, 
            FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_FLAG_BACKUP_SEMANTICS, NULL);
            SetFileTime(targetFile, &findData.ftCreationTime, 
            &findData.ftLastAccessTime, &findData.ftLastWriteTime);
            CloseHandle(targetFile);
        }
        else
        {
            // copy file and set attributes
            HANDLE fromFile = CreateFile(subFrom, GENERIC_READ, 
            FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
            HANDLE targetFile = CreateFile(subTarget, GENERIC_WRITE, 
            0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, fromFile);
            BYTE buf[1024];
            DWORD readedCount = 0, writedCount = 0;
            do
            {
                ReadFile(fromFile, buf, sizeof(buf), &readedCount, NULL);
                WriteFile(targetFile, buf, readedCount, &writedCount, NULL);
            } while (readedCount > 0);
            SetFileTime(targetFile, &findData.ftCreationTime, 
            &findData.ftLastAccessTime, &findData.ftLastWriteTime);
            // close handles
            CloseHandle(fromFile);
            CloseHandle(targetFile);
        }
    } while (FindNextFile(findFile, &findData));
    FindClose(findFile);
}


int main(int argc, char* argv[])
{
    if (argc < 3)
    {
        cout << "error: missing param" << endl;
        return -1;
    }
    WCHAR from[1024] = {0};
    MultiByteToWideChar(CP_ACP,0,argv[1],strlen(argv[1])+1,from,
		sizeof(from)/sizeof(from[0]));
    WCHAR target[1024] = {0};
    MultiByteToWideChar(CP_ACP,0,argv[2],strlen(argv[2])+1,target,
		sizeof(target)/sizeof(target[0]));
    
    CreateDirectory(target, NULL);
    WIN32_FIND_DATA findData;
    HANDLE findFile = FindFirstFile(from, &findData);
    HANDLE targetFile = CreateFile(target, GENERIC_WRITE, 
    FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_FLAG_BACKUP_SEMANTICS, NULL);
    // start copy
    copy_all(from, target);
    SetFileTime(targetFile, &findData.ftCreationTime, 
    &findData.ftLastAccessTime, &findData.ftLastWriteTime);
    CloseHandle(targetFile);
    FindClose(findFile);
    return 0;
}
