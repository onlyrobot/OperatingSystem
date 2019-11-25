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


void p(int semid, int sem_num)
{
    sembuf se;
    se.sem_num = sem_num;
    se.sem_op = -1;
    se.sem_flg = 0;
    semop(semid, &se, 1);
}


void v(int semid, int sem_num)
{
    sembuf se;
    se.sem_flg = 0;
    se.sem_op = 1;
    se.sem_num = sem_num;
    semop(semid, &se, 1);
}


int main(int argc, char* argv[])
{
    if (argc < 4)
    {
        cout << "error: missing param" << endl;
        return 0;
    }
    int semkey;
    sscanf((const char*)argv[1], "%d", &semkey);
    int semid = semget(semkey, 0, 0666);
    int shmkey;
    sscanf((const char*)argv[2], "%d", &shmkey);
    int shmid = shmget(shmkey, sizeof(buf), 0666);
    int consumer_id;
    sscanf((const char*)argv[3], "%d", &consumer_id);
    buf* p_buf = (buf*)shmat(shmid, nullptr, 0);

    srand(time(nullptr) * consumer_id);
    for (int i = 0; i < 4; ++i)
    {
        sleep(rand() % 10);
        p(semid, 2);
        p(semid, 1);
        p_buf->remain -= 1;
        cout << "consumed by " << consumer_id << ", at time " << \
        time(nullptr) << ", buf remain " << p_buf->remain << endl;
        v(semid, 1);
        v(semid, 0);
    }
    shmdt(p_buf);
    return 0;
}