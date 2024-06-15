#ifndef MYPRIMECOUNETER_H
#define MYPRIMECOUNETER_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>

#define MAX_NUMBERS 100000
#define MAX_THREADS 10

typedef struct node {
    int* numbers; // array of numbers to check
    int size; // size of the array
    struct node* next;
} Node, *PNode;

typedef struct queue {
    PNode head;
    PNode tail;
    int size;
    pthread_mutex_t lock;
} Queue, *PQueue;

/*-----Functions-----*/
PNode newNode(int*, int);
PQueue newQueue();
void enqueue(PQueue, int*, int);
PNode dequeue(PQueue);
void freeQueue(PQueue);
bool isPrime(int);
void *thread_func(PQueue);
#endif