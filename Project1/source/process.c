/*******************************************************************
 * Lab 1
 * Author: jing1
 * Date:   2018-10-19
 * Desc:   Multi-threaded and Multi-process Scheduling and Management
 *******************************************************************/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <math.h>

/* Macro define some constants */
#define PRIME_NUM 20 // range of prime numbers
#define FIB_NUM 10   // length of Fib seq

/* Functions for managing and scheduling processes and threads */
void *myThread1(void);    // No.1 Threads: generate prime number between 1 ~ n
void *myThread2(void);    // No.2 Threads: generate Fibonacci sequence
int mainProcess();        // No.1 Process: create two sub-processes(No.2 and No.3 Process)
int createTwoProcesses(); // No.3 Process: create two sub-processes(No.4 and No.5 Process)
int createTwoThreads();   // No.4 Process: create two threads(No.1 and No.2 Thread)
void func(int);           // No.2 and No.3 Process exec different tasks
void childFunc(int);      // No.4 and No.5 Process exec different tasks

/* other functions */
int isPrime(int); // determine if it is a prime number

/* main routine */
int main()
{
    mainProcess();
    printf("\n"
           "Process1─┬─Process2\n"
           "         │\n"
           "         └─Process3─┬─Process4─┬─Thread1\n"
           "                    │          │\n"
           "                    │          └─Thread2\n"
           "                    │\n"
           "                    └─Process5\n");
    return 0;
}

int isPrime(int n)
{
    for (int i = 2; i <= sqrt(n); i++)
    {
        if (n % i == 0)
            return 0;
        else
            continue;
    }
    return 1;
}

/* 
 * No.1 Thread:
 * generate prime sequence between 1 ~ n and print out
 */
void *myThread1(void)
{
    int n = PRIME_NUM;
    // printf("Intput the value of n:\n");
    // scanf("%d", &n);
    printf("The prime sequence as following:\n");
    for (int i = 2; i < n; i++)
    {
        if (isPrime(i))
        {
            printf("%d ", i);
        }
    }
    printf("\nThread 1 exit!\n");
    pthread_exit(0);
}

/*
 * No.2 Thread:
 * generate Fibonacci sequence and print out
 */
void *myThread2(void)
{
    int n = FIB_NUM; // the length of Fib seq
    int first = 0, second = 1, temp;
    // printf("\nInput Fib value: ");
    // scanf("%d", &n);
    printf("The Fib sequence as following:\n");
    printf("%d %d ", first, second);
    for (int i = 0; i < n; i++)
    {
        temp = second;
        second = first + second;
        first = temp;
        printf("%d ", second);
    }
    printf("\nThread 2 exit!\n");
    pthread_exit(0);
}

/*
 * This function is used to create 2 threads,
 * (No.1 and No.2 Threads) for No.4 Process
 */
int createTwoThreads()
{
    int ret1 = 0, ret2 = 0;
    pthread_t id1, id2;
    ret1 = pthread_create(&id1, NULL, (void *)myThread1, NULL);
    if (ret1)
    {
        printf("Create pthread ERROR!\n");
        return 1;
    }
    ret2 = pthread_create(&id2, NULL, (void *)myThread2, NULL);
    if (ret2)
    {
        printf("Create pthread ERROR!\n");
        return 1;
    }
    pthread_join(id1, NULL);       // Main Thread wait for No.1 Thread to end
    pthread_join(id2, NULL);       // Main Thread wait for No.2 Thread to end
    printf("main thread exit!\n"); // Main Thread exit
    return 0;
}

/*
 * This function is used to create different sub-processes
 * No.4 and 5 for No.3 Process
 */
int createTwoProcesses()
{
    int i;
    for (i = 4; i < 6; i++)
    {
        pid_t child;
        child = fork();
        if (child < 0)
        {
            printf("ERROR happended in fork function!\n");
            return 1;
        }
        else if (child == 0)
        {
            childFunc(i);
        }
        pid_t cpid = wait(NULL);
        printf("The process %d exit!\n", cpid);
    }
    // for (i = 0; i < 2; i++)
    // {
    //     pid_t cpid = wait(NULL);
    //     printf("The process %d exit!\n", cpid);
    // }
    return 0;
}

/*
 * No.1 Process:
 * create 2 sub-processes(No.2 and No.3 Process)
 */
int mainProcess()
{
    int i;
    printf("No.1 Process\t PID:%d\t PPID:%d\t"
           "Create 2 processes.\n",
           getpid(),
           getppid());
    for (i = 2; i < 4; i++)
    {
        pid_t child;
        child = fork();
        if (child < 0)
        {
            printf("ERROR happened in fork function!\n");
            return 1;
        }
        else if (child == 0)
        {
            func(i);
        }
        pid_t cpid = wait(NULL);
        printf("The process %d exit!\n", cpid);
    }
    // for (i = 0; i < 2; i++)
    // {
    //     pid_t cpid = wait(NULL);
    //     printf("The process %d exit!\n", cpid);
    // }
    printf("No.1 Process %d exit!\n", getpid());
    return 0;
}

/*
 * This function is used to exec the following different tasks:
 * No.4 Process creates two threads.
 * No.5 Process exec a user-written executable file, print out "hello, world!".
 */
void childFunc(int i)
{
    switch (i)
    {
    case 4: // if NO.4 process: create two threads
        printf("No.4 Process\t PID:%d\t PPID:%d\t"
               "Create 2 threads\n",
               getpid(),
               getppid());
        createTwoThreads(); // create 2 threads by calling createTwoThreads()
        break;
    case 5: // if No.5 process: print "hello world"
        printf("No.5 Process\t PID:%d\t PPID:%d\t"
               "Print 'hello, world!'\n",
               getpid(),
               getppid());
        execl("usertask", "./usertask", NULL); // execute user executable file
        break;
    }
    exit(0);
}

/*
 * This function is used to exec the following different tasks:
 * No.2 Process exec system command, like ls, ps, free.
 * No.3 Process create two sub-processes No.4 and 5 by calling createTwoProcesses()
 */
void func(int i)
{
    switch (i)
    {
    case 2: // if No.2 Process: exec system command
        printf("No.2 Process\t PID:%d\t PPID:%d\t"
               "Exexute command 'ls'\n",
               getpid(),
               getppid());
        execl("/bin/ls", "ls", NULL);
        break;
    case 3: // if No.3 Process: createTwoThreads()
        printf("No.3 Process\t PID:%d\t PPID:%d\t"
               "Create 2 processes\n",
               getpid(),
               getppid());
        createTwoProcesses();
        break;
    }
    exit(0);
}