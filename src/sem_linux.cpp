# include<iostream>
# include<sys/sem.h>
# include<unistd.h>
# include<sys/wait.h>
using namespace std;


union semun
{
    int val;
    semid_ds* buf;
    unsigned short* array;
};


void file_operation(int semid, const char* filepath, int pid)
{
    sembuf se;
    se.sem_num = 0;
    se.sem_op = -1;
    se.sem_flg = 0;
    semop(semid, &se, 1);
    FILE* file = fopen(filepath, "a");
    cout << "current pid is: " << pid << endl;
    sleep(3);
    fclose(file);
    se.sem_num = 0;
    se.sem_op = 1;
    se.sem_flg = 0;
    semop(semid, &se, 1);
}


int main()
{
    semun sem_val;
    int semid = semget(300, 0, 0666 | IPC_CREAT);
    sem_val.val = 1;
    semctl(semid, 0, SETVAL, sem_val);
    if (fork() > 0)
    {
        file_operation(semid, "./sem_linux.cpp", getpid());
    }
    else
    {
        file_operation(semid, "./sem_linux.cpp", getpid());
    }
    semctl(semid, 0, IPC_RMID);
    return 0;
}