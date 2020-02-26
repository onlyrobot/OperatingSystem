# include<iostream>
# include<cstdio>
# include<sys/wait.h>
# include<unistd.h>
using namespace std;


int main()
{
    FILE* sub_program = popen("./popen_linux", "r");
    char buf[50] = {0};
    fread(buf, sizeof(char), sizeof(buf), sub_program);
    cout << "read from sub-program: " << buf << endl;
    pclose(sub_program);
    return 0;
}