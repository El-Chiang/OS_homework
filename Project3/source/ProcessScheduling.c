/*******************************************************************
 * Lab 3
 * Author: jing1
 * Date:   2018-11-12
 * Desc:   Process Scheduling and Synchronization
 *******************************************************************/

#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#define THREAD_NUMBER 20
#define TIME_SLICE 100000
#define MAX_TIME 100
#define TRUE 1
#define FALSE 0

typedef int bool;

static int timer = 0;

/**
 * Define Process Control Block struct
 */
struct PCB
{
    pthread_t id;        // thread ID
    int priority;        // priority
    int arrivalTime;     // arrival time (suppose all arrive at the same time, because lazy...)
    int intervalTime;    // interval time
    bool *operationFlag; // run -> TRUE, block -> FALSE
} PCBQueue[THREAD_NUMBER];

/**
 * FCFS & SJF sub-thread control
 */
void *subThread(void *threadCount)
{
    int count = *(int *)threadCount;
    int subTimer = 0;
    int prevCount = -1;
    while (TRUE)
    {
        if (subTimer >= PCBQueue[count].intervalTime)
            pthread_exit(0);

        // set flag -> TRUE
        while (!(PCBQueue[count].operationFlag[timer]))
        {
            usleep(TIME_SLICE);
            timer++;
        }
        if (prevCount != count)
        {
            printf("Thread %2d : %d\n", count + 1, ++timer);
            prevCount = count;
        }
        subTimer++;
        usleep(TIME_SLICE);
    }
    pthread_exit(0);
}

/**
 * RR sub-thread control
 */
void *RRSubThread(void *threadCount)
{
    int count = *(int *)threadCount;
    int subTimer = 0;
    int t = timer - 1;
    while (TRUE)
    {
        if (subTimer >= PCBQueue[count].intervalTime)
            pthread_exit(0);
        while (t == timer)
            ; // wait for timer increasing
        while (!(PCBQueue[count].operationFlag[timer]))
            ; // wait for FLAG being TRUE
        t = timer;

        // thread can execute, let timer increase and print out
        printf("Thread %2d: %d\n", count + 1, timer + 1);
        subTimer++;
    }
    pthread_exit(0);
}

/**
 * initial FCFS thread queue:
 * Create 20 child threads and set every flag to TRUE
 */
void FCFSQueInit()
{
    int currTime = 0;
    for (int i = 0; i < THREAD_NUMBER; i++)
    {
        for (int j = currTime; j < currTime + PCBQueue[i].intervalTime; j++)
        {
            PCBQueue[i].operationFlag[j] = TRUE;
        }
        currTime += PCBQueue[i].intervalTime;
    }
}

/**
 * Execute FCFS
 */
int doFCFS()
{
    FCFSQueInit();  // initial
    int totalTime = 0;
    for (int i = 0; i < THREAD_NUMBER; i++)
    {
        totalTime += PCBQueue[i].intervalTime;
    }
    int ret[THREAD_NUMBER] = {0};
    int threadCount = 0;
    for (int i = 0; i < THREAD_NUMBER; i++)
    {
        ret[i] = pthread_create(&PCBQueue[threadCount].id, NULL, subThread, &threadCount);
        if (ret[i])
        {
            printf("create thread %d ERROR\n", i);
            return 1;
        }
        pthread_join(PCBQueue[threadCount].id, NULL);
        threadCount++;
    }
    return 0;
}

/**
 * SJF Scheduling Sort
 */
int *SJFSort(int *sorte) {}

/**
 * Execute SJF
 */
int doSJF() {}

/**
 * RR Scheduling
 */
void RRQueInit() {}

/**
 * Execute RR Scheduling
 */
int doRR() {}

void printHelp()
{
    printf("\n-args...：\n\n"
           "    --FCFS\tFirst Come First Served Scheduling,\n"
           "    --SJF \tShortest Job First Scheduling,\n"
           "    --RR  \tRound Robin Scheduling,\n"
           "    --PS  \tPriority Scheduling(TODO),\n"
           "    --MLQS\tMulti-Level Queue Scheduling(TODO).\n"
           "\neg: ./ProcessScheduling --fcfs\n\n");
    return;
}

/**
 * SJF Scheduling
 */

int main(int argc, char *argv[])
{
    printHelp();
    // initial operationalFlag of all PCB, set intervalTime to random 1~5
    srand((unsigned)time(NULL));
    for (int i = 0; i < THREAD_NUMBER; i++)
    {
        PCBQueue[i].intervalTime = rand() % 5 + 1;
        PCBQueue[i].operationFlag = (int *)malloc(MAX_TIME * sizeof(int));
        for (int j = 0; j < MAX_TIME; j++)
        {
            PCBQueue[i].operationFlag[j] = 0;
        }
    }

    printf("FCFS\n");
    doFCFS();
    return 0;
}