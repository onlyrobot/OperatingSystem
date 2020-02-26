# include<iostream>
# include<windows.h>
# include<cstring>
using namespace std;


// void get_abs_path(char* path, char* working_path, char* abs_path)
// {
//     if (path[0] == '.' || strlen(path) > 1 && path[1] == ':')
//     {
//         strcpy(abs_path, path);
//     }
//     else
//     {
//         strcpy(abs_path, working_path);
//         char* p_last_split = abs_path;
//         char* p_temp = abs_path;
//         while (*p_temp != '\0')
//         {
//             if (*p_temp == '\\')
//             {
//                 p_last_split = p_temp;
//             }
//             p_temp += 1;
//         }
//         strcpy(p_last_split + 1, path);
//     }
// }


int main(int argc, char* argv[])
{
    if (argc < 3)
    {
        cout << "error: missing param" << endl;
        return -1;
    }
    // char from_file_path[1024];
    // get_abs_path(argv[1], argv[0], from_file_path);
    // char target_file_path[1024];
    // get_abs_path(argv[2], argv[0], target_file_path);

    // WCHAR from_file_path[256];
    // memset(from_file_path,0,sizeof(from_file_path));
    // MultiByteToWideChar(CP_ACP,0,argv[1],strlen(argv[1])+1,
    // from_file_path, sizeof(from_file_path)/sizeof(from_file_path[0]));

    // WCHAR target_file_path[256];
    // memset(target_file_path,0,sizeof(target_file_path));
    // MultiByteToWideChar(CP_ACP,0,argv[2],strlen(argv[2])+1,
    // target_file_path, sizeof(target_file_path)/sizeof(target_file_path[0]));

    HANDLE from_file = CreateFile((LPCSTR)argv[1], GENERIC_READ, 
    FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    HANDLE target_file = CreateFile((LPCSTR)argv[2], GENERIC_WRITE, 
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

    return 0;
}