#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "parser.h"
#include "queue.h"

void init_q(queue *q){

   q->front = 0;
   q->rear = -1;
   q->itemCount = 0;
}

vector peek(queue *q) {
   return q->vecArray[q->front];
}

int isEmpty(queue *q) {
   return q->itemCount == 0;
}

int isFull(queue *q) {
   return q->itemCount == MAX;
}

int size(queue *q) {
   return q->itemCount;
}  

void insert(queue *q, vector data) {

   if(!isFull(q)) {
	
      if(q->rear == MAX-1) {
         q->rear = -1;            
      }       

      q->vecArray[++q->rear] = data;
      q->itemCount++;
   }
}

vector removeData(queue *q) {
   vector data = q->vecArray[q->front++];
	
   if(q->front == MAX) {
      q->front = 0;
   }
	
   //printf("%i\n", q->itemCount);
   if (q->itemCount<=0) {printf("NOTHING LEFT TO POP\n");}
   q->itemCount--;
   return data;  
}
