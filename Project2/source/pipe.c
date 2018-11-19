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
#include <sys/wait.h>

// process 1 write data and send message to pipe
void writeData(int rfd, int wfd)
{
	FILE *fp = fopen("1.txt", "w");
	if (fp == NULL)
	{
		printf("open file ERROR\n");
		exit(0);
	}
	close(rfd);
	fprintf(fp, "ok\n"); // write data "ok"
	write(wfd, "ok", 2); // send data "ok" to pipe
	fclose(fp);
	exit(0);
}

// process 2 read data
void readData(int rfd, int wfd)
{
	int nbytes;
	char readbuffer[200];
	nbytes = read(rfd, readbuffer, sizeof(readbuffer));
	// judge if it is "ok"
	if (strcmp(readbuffer, "ok") == 0)
	{
		FILE *fp = fopen("1.txt", "r");
		char buffer;
		while (EOF != (buffer = fgetc(fp)))
			printf("%c", buffer);
		fclose(fp);
	}
	else
		printf("write data ERROR");
	exit(0);
}

// create child process 1 & 2
void createProcess(int fd[2])
{
	int rfd = fd[0]; // read
	int wfd = fd[1]; // write
	for (int i = 1; i <= 2; i++)
	{
		pid_t child;
		child = fork();
		if (child == -1)
		{
			perror("fork ERROR\n");
			exit(0);
		}
		else if (child == 0)
		{
			if (i == 1)
				writeData(rfd, wfd);
			else
				readData(rfd, wfd);
		}
		wait(NULL);
	}
	exit(0);
}

int main()
{
	int fd[2];
	pipe(fd); // create pipe
	createProcess(fd);
	return 0;
}
