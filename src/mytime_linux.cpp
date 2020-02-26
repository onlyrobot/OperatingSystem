# include<iostream>
# include<unistd.h>
# include<sys/time.h>
# include<sys/wait.h>
# include<cstring>

using namespace std;


void display_time(long long total)
{
    int hour = total / (60 * 60 * 1000000LL);
    total %= 60 * 60 * 1000000LL;
    int minute = total / (60 * 1000000);
    total %= 60 * 1000000;
    int second = total / 1000000;
    total %= 1000000;
    int ms = total / 1000;
    int us = total % 1000;
    cout << "spend time: " << endl << hour 
    << "h " << minute << "m " << second << "s " 
    << ms << "ms " << us << "us" << endl;
}


int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        cout << "error: missing parameters" << endl;
        exit(-1);
    }
    pid_t pid;
    if ((pid = fork()) < 0)
    {
        cout << "fork error" << endl;
        exit(-1);
    }
    else if (pid == 0)
    {
        // FILE *file;
        // char cmd[4096] = "which ";
        // char abs_path[4096] = {0};
        // strcat(cmd, argv[1]);
        // if ((file = popen(cmd, "r")) != NULL)
        // {
        //     fgets(abs_path, 4096, file);
        //     pclose(file);
        // }
        execlp(argv[1], argv[1], argv[2], nullptr);
    }
    else
    {
        timeval start;
        timeval end;
        gettimeofday(&start, nullptr);
        wait(NULL);
        gettimeofday(&end, NULL);
        long long total = 1000000LL * (end.tv_sec - start.tv_sec) + 
        end.tv_usec - start.tv_usec;
        cout << "process: \"" << argv[1] << "\" ended" << endl;
        display_time(total);
    }
    exit(0);
}