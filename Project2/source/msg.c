/*******************************************************************
 * Lab 2
 * Author: jing1
 * Date:   2018-11-09
 * Desc:   Inter-process Communication
 *******************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <sys/msg.h>
#include <sys/wait.h>

typedef struct myMsg
{
	int msgtype; // message type
	char *text;  // message text content
} myMsg;

// process 1 write data to file and send message "ok"
void sndmsg(int msqid, myMsg msg)
{
	FILE *fp = fopen("1.txt", "w");
	if (fp == NULL)
	{
		printf("open file ERROR\n");
		exit(0);
	}
	fprintf(fp, "Something was written in 1.txt.\n"); // write data to the file
	fclose(fp);
	if (msgsnd(msqid, &msg, sizeof(msg.text), IPC_NOWAIT) != 0)
		perror("send message ERROR\n");
	else
		printf("send message success\n");
	exit(0);
}

// process 2 receive message and print out content of file
void rcvmsg(int msqid, myMsg msg)
{
	myMsg testmsg;
	msgrcv(msqid, &testmsg, sizeof(msg.text), msg.msgtype, IPC_NOWAIT);
	if (testmsg.text == msg.text)
	{
		printf("receive message success\n");
		FILE *fp = fopen("1.txt", "r");
		char buffer;
		while (EOF != (buffer = fgetc(fp)))
			printf("%c", buffer);
		fclose(fp);
	}
	else
		printf("receive message ERROR\n");
	if (msgctl(msqid, IPC_RMID, 0) == 0) // clean message queue, remove samephore
		printf("clean message queue\n");
	else
		printf("clean message queue ERROR\n");
	exit(0);
}

// get message queue ID
int getqid()
{
	key_t key = ftok(".", 1);				   // uniquqe key, use current path
	int msqid = msgget(key, 0600 | IPC_CREAT); // create message queue
	return msqid;
}

// create child process 1 & 2
void createProcess(int msqid, myMsg msg)
{
	for (int i = 1; i <= 2; i++)
	{
		pid_t child = fork();
		if (child == -1)
		{
			printf("fork ERROR\n");
			exit(0);
		}
		else if (child == 0)
		{
			if (i == 1)
				sndmsg(msqid, msg); // process 1: send message
			else
				rcvmsg(msqid, msg); // process 2: receive message
		}
		wait(NULL);
	}
	exit(0);
}

int main()
{
	int msqid = getqid(); // parent process create message queue
	myMsg msg = {1, "ok"};
	createProcess(msqid, msg);
	return 0;
}
