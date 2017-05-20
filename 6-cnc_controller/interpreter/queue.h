#ifndef queue_h
#define queue_h

typedef struct {
	float x;
	float y;
	float z;
} vector;

#define MAX 1000

/*Modified from tutorialspoint.com - https://www.tutorialspoint.com/data_structures_algorithms/queue_program_in_c.htm*/

typedef struct{
   vector vecArray[MAX];
   int front;
   int rear;
   /* Could be done smarter to remove need for itemCount
    * Could use a trick 
    * - head=tail => empty
   	* - tail=head-1 || (head==0 && tail==MAX) => full
    */
   int itemCount;
} queue;

/*Initializes a queue object - a queue must be defined first
  This function must be called to initialize the queue*/
void init_q(queue *q);

/*Read the next value from the queue without removing it*/
vector peek(queue *q);

/*Returns boolean TRUE if the queue is empty*/
int isEmpty(queue *q);

/*Returns boolean TRUE if the queue is full*/
int isFull(queue *q);

/*Returns the size of what is currently in the queue - how full it is*/
int size(queue *q);

/*Pushes data vector to the queue*/
void insert(queue *q, vector data);

/*Returns the first object put into the queue and removes it from the queue - FIFO*/
vector removeData(queue *q);

#endif