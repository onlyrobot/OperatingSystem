# include<iostream>
# include<sys/msg.h>
# include<unistd.h>
# include<cstring>
# include<sys/wait.h>
using namespace std;


struct my_msg
{
    long type;
    char text[200];
};


int main()
{
    int msgid = msgget(200, 0666 | IPC_CREAT);
    if (fork() > 0)
    {
        my_msg msg;
        msg.type = 1;
        for (int i = 0; i < 5; ++i)
        {
            sprintf(msg.text, "parent %d", i);
            msgsnd(msgid, &msg, strlen(msg.text) + 1, 0);
            sleep(1);
        }
        msg.type = 2;
        for (int i = 0; i < 5; ++i)
        {
            memset(msg.text, 0, sizeof(msg.text));
            msgrcv(msgid, &msg, sizeof(msg.text), 2, 0);
            cout << "parent recieved message: " << msg.text << endl;
        }
    }
    else
    {
        my_msg msg;
        msg.type = 2;
        for (int i = 0; i < 5; ++i)
        {
            sprintf(msg.text, "child %d", i);
            msgsnd(msgid, &msg, strlen(msg.text) + 1, 0);
            sleep(1);
        }
        msg.type = 1;
        for (int i = 0; i < 5; ++i)
        {
            memset(msg.text, 0, sizeof(msg.text));
            msgrcv(msgid, &msg, sizeof(msg.text), 1, 0);
            cout << "child recieved message: " << msg.text << endl;
        }
    }
    msgctl(msgid, IPC_RMID, nullptr);
    return 0;
}