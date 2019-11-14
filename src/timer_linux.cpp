# include<unistd.h>
# include<iostream>
using namespace std;


int main(int argc, char* argv[])
{
    sleep(atoi(argv[1]));
    exit(0);
}