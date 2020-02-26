# include<iostream>
# include<unistd.h>
# include<sys/types.h>
# include<dirent.h>
# include<sys/stat.h>
# include<fcntl.h>
# include<cstring>
using namespace std;


// copy file recursively
void copy_all(char* from, char* target)
{
    DIR* dir = opendir(from);
    strcat(from, "/");
    strcat(target, "/");
    dirent* pd;
    while ((pd = readdir(dir)) != nullptr)
    {
        if (strcmp(pd->d_name, ".") == 0)
            continue;
        if (strcmp(pd->d_name, "..") == 0)
            continue;
        char sub_from[PATH_MAX] = {0};
        strcpy(sub_from, from);
        strcat(sub_from, pd->d_name);
        char sub_target[PATH_MAX] = {0};
        strcpy(sub_target, target);
        strcat(sub_target, pd->d_name);
        struct stat st;
        lstat(sub_from, &st);
        if (S_ISDIR(st.st_mode))
        {
            mkdir(sub_target, st.st_mode);
            copy_all(sub_from, sub_target);
            // change the time of dir
            int fd = open(sub_target, O_RDONLY);
            timespec tp[2] = {st.st_atim, st.st_mtim};
            futimens(fd, tp);
            close(fd);
        }
        else if (S_ISLNK(st.st_mode))
        {
            char temp[PATH_MAX] = {0};
            int len = readlink(sub_from, temp, PATH_MAX);
            temp[len] = '\0';
            // change the time of symlik
            symlink(temp, sub_target);
            timespec tp[2] = {st.st_atim, st.st_mtim};
            utimensat(AT_FDCWD, sub_target, tp, AT_SYMLINK_NOFOLLOW);
        }
        else
        {
            int fd_from = open(sub_from, O_RDONLY);
            int fd_target = open(sub_target, O_WRONLY | O_CREAT, st.st_mode);
            int read_count = 0, write_count = 0;
            char buf[1024];
            while ((read_count = read(fd_from, buf, sizeof(buf))) > 0)
            {
                write(fd_target, buf, read_count);
            }
            // change the time of file
            timespec tp[2] = {st.st_atim, st.st_mtim};
            futimens(fd_target, tp);
            close(fd_from);
            close(fd_target);
        }
    }
    closedir(dir);
}


int main(int argc, char* argv[])
{
    if (argc < 3)
    {
        cout << "error: missing param" << endl;
        return -1;
    }
    char from[PATH_MAX] = {0};
    char target[PATH_MAX] = {0};
    strcpy(from, argv[1]);
    strcpy(target, argv[2]);
    struct stat st;
    stat(from, &st);
    mkdir(target, st.st_mode);

    copy_all(from, target);
    // change the initial dir
    timespec tp[2] = {st.st_atim, st.st_mtim};
    int fd = open(target, O_RDONLY);
    futimens(fd, tp);
    close(fd);
    return 0;
}