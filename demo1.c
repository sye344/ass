#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include <sched.h>
long int count = 4294967296;
long long unsigned int time1, time2, time3;
#define BILLION 1000000000L

void *countA(void *arg)
{
    struct timespec start, end;
    long long int i = 0;
    clock_gettime(CLOCK_MONOTONIC, &start);
    for (long long int i = 1; i < count; i++)
    {
    }
    clock_gettime(CLOCK_MONOTONIC, &end);
    time1 = BILLION * (end.tv_sec - start.tv_sec) + end.tv_nsec - start.tv_nsec;
    printf("elapsed time of SCHED RR = %llu nanoseconds\n", (long long unsigned int)time1);
    return NULL;
}
void *countB(void *arg)
{
    struct timespec start, end;
    int i;
    clock_gettime(CLOCK_MONOTONIC, &start);
    for (long long int i = 1; i < count; i++)
    {
    }
    clock_gettime(CLOCK_MONOTONIC, &end);
    time2 = BILLION * (end.tv_sec - start.tv_sec) + end.tv_nsec - start.tv_nsec;
    printf("elapsed time of SCHED FIFO = %llu nanoseconds\n", time2);

    return NULL;
}
void *countC(void *arg)
{
    struct timespec start, end;
    int i;
    clock_gettime(CLOCK_MONOTONIC, &start);
    for (long long int i = 1; i < count; i++)
    {
    }
    clock_gettime(CLOCK_MONOTONIC, &end);
    time3 = BILLION * (end.tv_sec - start.tv_sec) + end.tv_nsec - start.tv_nsec;
    printf("elapsed time of SCHED OTHER = %llu nanoseconds\n", (long long unsigned int)time3);
    return NULL;
}

int main(int arg, char *argv[])
{
    int priority = 1;
    pthread_t thr, thr1, thr2;
    pthread_attr_t attr, attrp, attrpp;
    int s, s1, s2;
    struct sched_param sch, sche, schedParam;

    s = pthread_attr_init(&attr);
    s = pthread_attr_setschedpolicy(&attr, SCHED_RR);
    s = pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED);

    s1 = pthread_attr_init(&attrp);
    s1 = pthread_attr_setschedpolicy(&attrp, SCHED_FIFO);
    s1 = pthread_attr_setinheritsched(&attrp, PTHREAD_EXPLICIT_SCHED);

    s2 = pthread_attr_init(&attrpp);
    s2 = pthread_attr_setschedpolicy(&attrpp, SCHED_OTHER);
    s2 = pthread_attr_setinheritsched(&attrpp, PTHREAD_EXPLICIT_SCHED);

    for (int i = 0; i < 10; i++)
    {
        printf("Time elapsed for priority %d \n", priority);
        printf("-----------------------------------------\n");

        s = pthread_attr_getschedparam(&attr, &sche);
        sche.sched_priority = priority;
        s = pthread_attr_setschedparam(&attr, &sche);
        s = pthread_create(&thr, &attr, &countA, NULL);

        s = pthread_attr_getschedparam(&attrp, &schedParam);
        schedParam.sched_priority = priority;
        s1 = pthread_attr_setschedparam(&attrp, &schedParam);
        s1 = pthread_create(&thr1, &attrp, &countB, NULL);

        s2 = pthread_attr_getschedparam(&attrpp, &sch);
        sch.sched_priority = 0;
        s2 = pthread_attr_setschedparam(&attrpp, &sch);
        s2 = pthread_create(&thr2, &attrpp, &countC, NULL);

        pthread_join(thr, NULL);
        pthread_join(thr1, NULL);
        pthread_join(thr2, NULL);

        FILE *fptr;
        fptr = fopen("aqi.data", "a");
        fprintf(fptr, "%d %llu %llu %llu\n", priority, time1, time2, time3);
        priority += 8; 
        printf("-----------------------------------------\n");
    }
}
