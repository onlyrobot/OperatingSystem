# include<sys/sem.h>
# include<iostream>
# include<unistd.h>
# include<sys/shm.h>
# include<sys/wait.h>
using namespace std;


struct buf
{
    int remain;
    char data[3][1024];
};


union semun
{
    int val;
    semid_ds* buf;
    unsigned short* array;
};


const int SEMKEY = 400;
const int SHMKEY = 500;


int main()
{
    // initialization of 3 sem and shm
    int semid = semget(SEMKEY, 3, 0666 | IPC_CREAT);
    semun sem_val;
    sem_val.val = 3;
    semctl(semid, 0, SETVAL, sem_val);
    sem_val.val = 1;
    semctl(semid, 1, SETVAL, sem_val);
    sem_val.val = 0;
    semctl(semid, 2, SETVAL, sem_val);

    int shmid = shmget(SHMKEY, sizeof(buf), 0666 | IPC_CREAT);
    buf* p_buf = (buf*)shmat(shmid, 0, 0);
    p_buf->remain = 0;
    shmdt(p_buf);
    // start processes
    char semkey_s[50] = {0}, shmkey_s[50] = {0};
    sprintf(semkey_s, "%d", SEMKEY);
    sprintf(shmkey_s, "%d", SHMKEY);
    for (int i = 0; i < 2; ++i)
    {
        if (fork() == 0)
        {
            char producer_id[50] = {0};
            sprintf(producer_id, "%d", i);
            execlp("./producer_linux", "./producer_linux", semkey_s, \
            shmkey_s, producer_id, nullptr);
        }
    }
    for (int i = 2; i < 5; ++i)
    {
        if (fork() == 0)
        {
            char consumer_id[50] = {0};
            sprintf(consumer_id, "%d", i);
            execlp("./consumer_linux", "./consumer_linux", semkey_s, \
            shmkey_s, consumer_id, nullptr);
        }
    }
    while (wait(nullptr) != -1);
    shmctl(shmid, IPC_RMID, nullptr);
    semctl(semid, 0, IPC_RMID);
    return 0;
}