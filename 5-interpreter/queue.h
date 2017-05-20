#ifndef queue_h
#define queue_h

#include "parser.h"

#define MAX 100

typedef struct{
   vector vecArray[MAX];
   int front;
   int rear;
   int itemCount;
} queue;

void init_q(queue *q);
vector peek(queue *q);
int isEmpty(queue *q);
int isFull(queue *q);
int size(queue *q);
void insert(queue *q, vector data);
vector removeData(queue *q);

#endif