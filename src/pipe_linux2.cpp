# include<iostream>
# include<sys/wait.h>
# include<unistd.h>
using namespace std;


int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        cout << "error: no fdes" << endl;
        return -1;
    }
    int fdes;
    char buf[50] = {0};
    sscanf(argv[1], "%d", &fdes);
    read(fdes, buf, 11);
    cout << "sub-program read: " << buf << endl;
    return 0;
}