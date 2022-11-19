// void countA()
// void countB()
// void countC()
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include <sched.h>
long int count = 4294967296;
long long unsigned int time1;
long long unsigned int time2;
long long unsigned int time3;

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
    printf("elapsed time1 = %llu nanoseconds\n", (long long unsigned int)time1);
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
    printf("elapsed time2 = %llu nanoseconds\n", time2);

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
    printf("elapsed time3 = %llu nanoseconds\n", (long long unsigned int)time3);
    return NULL;
}

int main(int arg, char *argv[])
{
    pthread_t thr, thr1, thr2;
    pthread_attr_t attr, attrp, attrpp;
    int s, s1, s2;
    int priority = 15;
    s = pthread_attr_init(&attr);
    struct sched_param sche;
    s = pthread_attr_setschedpolicy(&attr, SCHED_RR);
    s = pthread_attr_getschedparam(&attr, &sche);
    sche.sched_priority = priority;
    s = pthread_attr_setschedparam(&attr, &sche);
    s = pthread_create(&thr, &attr, &countA, NULL);
    pthread_join(thr, NULL);

    s1 = pthread_attr_init(&attrp);
    struct sched_param schedParam;
    s = pthread_attr_setschedpolicy(&attrp, SCHED_FIFO);
    s = pthread_attr_getschedparam(&attrp, &schedParam);
    schedParam.sched_priority = priority;
    s1 = pthread_attr_setschedpolicy(&attrp, SCHED_FIFO);
    s1 = pthread_create(&thr1, &attrp, &countB, NULL);
    pthread_join(thr1, NULL);

    s2 = pthread_attr_init(&attrpp);
    s2 = pthread_attr_setschedpolicy(&attrpp, SCHED_OTHER);
    s2 = pthread_create(&thr2, &attrpp, &countC, NULL);
    pthread_join(thr2, NULL);

    FILE *fptr;
    fptr = fopen("aqi.data", "w");
    fprintf(fptr, "%d %llu %llu %llu", priority, time1, time2, time3);
    fclose(fptr);
}