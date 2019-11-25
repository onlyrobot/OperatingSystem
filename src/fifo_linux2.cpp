# include<unistd.h>
# include<iostream>
# include<sys/stat.h>
# include<sys/types.h>
# include<fcntl.h>
using namespace std;


int main()
{
    int fd = open("/tmp/myfifo", O_RDONLY);
    char buf[50] = {0};
    read(fd, buf, sizeof(buf));
    close(fd);
    cout << "sub-program read: " << buf << endl;
    return 0;
}