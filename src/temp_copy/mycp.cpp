# define UNICODE


# include<iostream>
# include<windows.h>
# include<cstring>
using namespace std;


void copy_all(char* from, char* target)
{
    // LPWIN32_FIND_DATA findData;
    // HANDLE findFile = FindFirstFile((LPCSTR)from, &findData);
    // if (findData->dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY)
    // {
    //     CreateDirectory()
    //     SetCurrentDirectory((LPCSTR)from);
    //     copy_all(from, target);
    //     SetCurrentDirectory((LPCSTR)"../");
    // }
    // else
    // {
    //     HANDLE from_file = CreateFile(findData->cFileName, GENERIC_READ, 
    //     FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    //     HANDLE target_file = CreateFile((LPCSTR)argv[2], GENERIC_WRITE, 
    //     0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, from_file);
    //     BYTE buf[1024];
    //     DWORD readed = 0, writed = 0;
    //     do
    //     {
    //         ReadFile(from_file, buf, sizeof(buf), &readed, NULL);
    //         WriteFile(target_file, buf, readed, &writed, NULL);
    //     } while (readed > 0);


    //     CloseHandle(from_file);
    //     CloseHandle(target_file);
    // }
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
    HANDLE from_file = CreateFile(from, GENERIC_READ, 
    FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    HANDLE target_file = CreateFile(target, GENERIC_WRITE, 
    0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, from_file);
    BYTE buf[1024];
    DWORD readed = 0, writed = 0;
    do
    {
        ReadFile(from_file, buf, sizeof(buf), &readed, NULL);
        WriteFile(target_file, buf, readed, &writed, NULL);
    } while (readed > 0);


    CloseHandle(from_file);
    CloseHandle(target_file);
    copy_all(argv[1], argv[2]);
    return 0;
}