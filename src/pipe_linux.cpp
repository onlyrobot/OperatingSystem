# include<unistd.h>
# include<sys/wait.h>
# include<iostream>
using namespace std;


int main()
{
    int fdes[2];
    pipe(fdes);
    // sub-thread demo
    // if (fork() > 0)
    // {
    //     close(fdes[1]);
    //     wait(NULL);
    //     char buf[50] = {0};
    //     read(fdes[0], buf, 50);
    //     cout << "parent read: " << buf << endl;
    // }
    // else
    // {
    //     close(fdes[0]);
    //     write(fdes[1], "hello world", 11);
    //     cout << "child write: " << "hello world" << endl;
    // }
    // sub-program demo
    if (fork() > 0)
    {
        close(fdes[0]);
        write(fdes[1], "hello world", 11);
        cout << "parent write: " << "hello world" << endl;
    }
    else
    {
        char param[50];
        sprintf(param, "%d", fdes[0]);
        execlp("./pipe_linux2", "./pipe_linux2", param, nullptr);
    }
    return 0;
}