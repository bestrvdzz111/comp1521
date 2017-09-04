// ADT for a FIFO queue
// COMP1521 17s2 Week01 Lab Exercise
// Written by John Shepherd, July 2017
// Modified by ...

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "Queue.h"

typedef struct QueueNode {
   int jobid;  // unique job ID
   int size;   // size/duration of job
   struct QueueNode *next;
} QueueNode;

struct QueueRep {
   int nitems;      // # of nodes
   QueueNode *head; // first node
   QueueNode *tail; // last node
};



// remove the #if 0 and #endif
// once you've added code to use this function

// create a new node for a Queue
static
QueueNode *makeQueueNode(int id, int size)
{
   QueueNode *new;
   new = malloc(sizeof(struct QueueNode));
   assert(new != NULL);
   new->jobid = id;
   new->size = size;
   new->next = NULL;
   return new;
}



// make a new empty Queue
Queue makeQueue()
{
   Queue new;
   new = malloc(sizeof(struct QueueRep));
   assert(new != NULL);
   new->nitems = 0; new->head = new->tail = NULL;
   return new;
}

// release space used by Queue
void  freeQueue(Queue q)
{
	assert(q != NULL);
	QueueNode *curr_head = q->head;
	QueueNode *prev = NULL;
	for(QueueNode *temp = curr_head;temp != NULL;)
	{
		prev = temp;
		free(temp);
		temp = prev->next;
	}
}

// add a new item to tail of Queue
void  enterQueue(Queue q, int id, int size)
{
	assert(q != NULL);
	QueueNode *newnode = makeQueueNode(id,size);
	if(q->nitems == 0)
	{
		q->head = newnode;
		q->tail = newnode;
	}
	else
	{
		assert(q->tail->next==NULL);
		QueueNode *prevtail  = q->tail;
		prevtail->next = newnode;
		q->tail = newnode;
	}
	q->nitems++;
}

// remove item on head of Queue
int   leaveQueue(Queue q)
{
	assert(q != NULL);
	if(q->nitems == 0)
	{
		return  0;
	}
	else
	{
		int old_id = q->head->jobid;
		QueueNode *old_head = q->head;
		QueueNode *new_head = q->head->next;
		free(old_head);
		q->head = new_head;
		if(new_head == NULL)
		{
			q->tail =new_head;
		}
		q->nitems--;
		return old_id;
	}
}

// count # items in Queue
int   lengthQueue(Queue q)
{
   assert(q != NULL);
   return q->nitems;
}

// return total size in all Queue items
int   volumeQueue(Queue q)
{
   assert(q != NULL);
	int sum_size = 0;
	QueueNode *curr = q->head;
	while(curr != NULL)
	{
		sum_size+=curr->size;
		curr = curr->next;
	}
	return sum_size;
}

// return size/duration of first job in Queue
int   nextDurationQueue(Queue q)
{
   assert(q != NULL);
	if(q->nitems == 0)
	{
		return 0;
	}
	else
	{
		return q->head->size;
	}
   return 0; // replace this statement
}


// display jobid's in Queue
void showQueue(Queue q)
{
   QueueNode *curr;
   curr = q->head;
   while (curr != NULL) {
      printf(" (%d,%d)", curr->jobid, curr->size);
      curr = curr->next;
   }
}
