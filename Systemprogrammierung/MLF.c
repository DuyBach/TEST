#include "MLF.h"
#include "task.h"

#include <stdio.h>
#include <stdlib.h>


// struct for queue
typedef struct q_elem{
	int id;
	int length;
	int time_step;
	int time_left;
	struct q_elem *next;
} q_elem;

struct Queue{
	q_elem *root;
	int time_step;
	struct Queue *next;
};

struct QueueList{
	struct Queue *root;
	int num_queues;
};


struct QueueList *queList;


void add_que(int tmp_step)
{
	struct Queue *que = (struct Queue*)malloc(sizeof(struct Queue));
	if (que == NULL) {
		printf("FAILED TO CREATE QUEUE\n");
		exit(1);
	}

	que->root = NULL;
	que->time_step = tmp_step;

	if (queList->root == NULL) {
		queList->root = que;
	} else {
		que->next = queList->root;
		queList->root = que;
	}
}

//FIFO when q_elem->time_step == num_queues*time_step
int init_MLF(int time_step, int num_queues)
{
	// TODO
	queList = (struct QueueList*)malloc(sizeof(struct QueueList));
	if (queList == NULL) {
		printf("FAILED TO CREATE QUEUE\n");
		exit(1);
	}

	queList->num_queues = num_queues;

	int tmp_step = num_queues*time_step;
	
	int i;
	for (i = 0; i < num_queues; i++) {
		add_que(tmp_step);
		//recalculate tmp_step
		tmp_step -= time_step;
	}
	
	return 1;
}

void free_MLF()
{
	// TODO
}

void arrive_MLF(int id, int length)
{
	id = id;
	length = length;
	// TODO
	
}

void tick_MLF()
{
	// TODO
}

void finish_MLF()
{
	// TODO (optional)
}
