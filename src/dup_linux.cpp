# include<iostream>
# include<cstdio>
# include<sys/wait.h>
# include<unistd.h>
using namespace std;


int main()
{
    int fdes[2];
    pipe(fdes);
    if (fork() > 0)
    {
        char buf[50] = {0};
        dup2(fdes[0], 0);
        cin.getline(buf, sizeof(buf));
        cout << "parent read: " << buf << endl;
    }
    else
    {
        cout << "child will write: " << "hello world" << endl;
        dup2(fdes[1], 1);
        cout << "hello world" << endl;
    }
    return 0;
}