# include<iostream>
# include<sys/shm.h>
# include<unistd.h>
# include<cstring>
# include<sys/wait.h>
using namespace std;


int main()
{
    int shmid = shmget(100, 1024, 0666 | IPC_CREAT);
    if (fork() > 0)
    {
        char* addr = (char*)shmat(shmid, 0, 0);
        cout << "parent write: " << "hello world" << endl;
        strcpy(addr, "hello world");
        wait(nullptr);
        shmdt(addr);
    }
    else
    {
        char* addr = (char*)shmat(shmid, 0, 0);
        sleep(1);
        cout << "child read: " << addr << endl;
        shmdt(addr);
    }
    shmctl(shmid, IPC_RMID, nullptr);
    return 0;
}