# include<unistd.h>
# include<iostream>
# include<sys/stat.h>
# include<sys/types.h>
# include<fcntl.h>
using namespace std;


int main()
{
    mkfifo("/tmp/myfifo", 0777);
    int fd = open("/tmp/myfifo", O_WRONLY);
    write(fd, "hello world", 11);
    cout << "parent write: " << "hello world" << endl;
    close(fd);
    return 0;
}