/*******************************************************************
 * Lab 2
 * Author: jing1
 * Date:   2018-11-09
 * Desc:   Inter-process Communication
 *******************************************************************/

#include <unistd.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUF_SIZE 16

/**
 * Define Message Queue struce
 */
typedef struct mymsg
{
    long mytype;               // message type
    unsigned char mytext[128]; // message text content
} mymsg;

/**
 * This function is used to juedge whether the message
 * was sent successfully
 */
void sendmsg(int msqid, mymsg msg)
{
    if (msgsnd(msqid, &msg, sizeof(msg.mytext), IPC_NOWAIT) != 0)
    {
        perror("process 1 send message fail!\n");
    }
    else
    {
        printf("process 1 send message successfully!\n");
    }
}

/**
 * This function is used to receive message
 */
void rcvmsg(int msqid)
{
    /* TODO: if "ok" in message queue, read data, then close the file */
}

/**
 * This function is used to create a queue
 */
int getqid()
{
    key_t key = ftok(".", 1); // use the current path to get the unique key
    /* TODO: create message queue */
    int msqid = msgget(key, 0666 | IPC_CREAT);
}

/**
 * This function is used to open the file and write data
 * (if the file does not exit, create it first)
 * return:
 *      - 1: open file fail
 *      - 0: open file success
 */
int writetofile()
{
    FILE *fp = NULL;
    fp = fopen("1.txt", "w");
    if (fp == NULL)
    {
        printf("Open file '1.txt' ERROR!\n");
        return 0;
    }
    fprintf(fp, "ok");
    fclose(fp);
    return 1;
}

/**
 * This function is used to read the file when get "ok" from message queue
 *  return:
 *      - 1: open file fail
 *      - 0: open file success
 */
int readfromfile(char *buffer)
{
    FILE *fp = NULL;
    fp = fopen("1.txt", "r");
    if (fp == NULL)
    {
        printf("Open file '1.txt' ERROR!\n");
        return 0;
    }
    fgets(buffer, BUF_SIZE, (FILE *)fp);
    printf("FILE content: %s\n", buffer);
    fclose(fp);
    return 1;
}

void err_quit(char *err)
{
    printf("%s\n", err);
    exit(-1);
}

int main()
{
    int msqid = getqid(); // parent process create message queue
    int fd[2];
    char buf[BUF_SIZE] = "ok";
    pid_t child;

    if (pipe(fd) < 0)
    {
        err_quit("pipe ERROR");
    }
    if ((child = fork()) < 0) // create a child process 1
    {
        err_quit("create process 1 ERROR");
    }
    else if (child == 0)
    {
        close(fd[0]);
        int flag = write(fd[1], buf, strlen(buf));
        writetofile();
    }
    else // create a child process 2
    {
        if ((child = fork()) < 0)
        {
            err_quit("create process 2 ERROR");
        }
        if (child == 0)
        {
            close(fd[1]);
            int len = read(fd[0], buf, BUF_SIZE);
            if (len < 0)
            {
                err_quit("read ERROR");
            }
            printf("receive data: %s\n", buf);
            readfromfile(buf);
        }
    }
    sleep(1);
    return 0;
}