#include "MLF.h"
#include "task.h"

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>


// struct for queue
typedef struct q_elem{
	int id;
	int length;
	int time_left;
	int nr;
	struct q_elem *next;
} q_elem;

struct Queue{
	q_elem *root;
	int time_step;
	struct Queue *next;
};

// list of queues
struct QueueList{
	struct Queue *root;
	q_elem *actual;
	int num_queues;
	int time_step;
};


struct QueueList *queList;


// extra function to add a queue to the queList
void add_que()
{
	struct Queue *que = (struct Queue*)malloc(sizeof(struct Queue));
	if (que == NULL) {
		printf("FAILED TO CREATE QUEUE\n");
		exit(1);
	}

	que->root = NULL;

	if (queList->root == NULL) {
		queList->root = que;
	} else {
		que->next = queList->root;
		queList->root = que;
	}

	switch_task(IDLE);
}

int init_MLF(int time_step, int num_queues)
{
	// create queList with (num_queues) queues
	queList = (struct QueueList*)malloc(sizeof(struct QueueList));
	if (queList == NULL) {
		printf("FAILED TO CREATE QUEUE\n");
		exit(1);
	}

	queList->actual = NULL;
	queList->num_queues = num_queues;
	queList->time_step = time_step;

	
	int i;
	for (i = 0; i < num_queues; i++) {
		add_que();
	}
	
	return 1;
}

void free_MLF()
{
	// free queList
	q_elem *tmpEle, *toDelete;
	struct Queue *queDelete;
	struct Queue *current = queList->root;

	free(queList->actual);

	int i;
	for (i = 0; i < queList->num_queues; i++) {
		// delete queue elements		
		tmpEle = current->root;
		while(tmpEle != NULL) {
			toDelete = tmpEle;
			tmpEle = tmpEle->next;

			free(toDelete);
		}

		queDelete = current;
		current = current->next;

		free(queDelete);		
	}

	free(queList);
}

void arrive_MLF(int id, int length)
{
	// if new q_elem incoming put in first queue
	// if cpu in IDLE-Mode fill with the new q_elem
	q_elem *new = (q_elem*)malloc(sizeof(q_elem));
	new->id = id;
	new->length = length;
	new->nr = 0;
	new->next = NULL;
	new->time_left = queList->time_step;

	if (queList->actual == NULL) {
		queList->actual = new;

		switch_task(new->id);

		return;
	}
	
	if (queList->root->root == NULL) {
		queList->root->root = new;
	} else {
		q_elem *current;
		current = queList->root->root;	
		
		while (1) {
			if (current->next == NULL) {
				current->next = new;
				return;
			}

			current = current->next;
		}
	}
}

void tick_MLF()
{
	// if cpu in IDLE-Mode stay IDLE (arrive_MLF will put q_elem in cpu if cpu is in IDLE-Mode)
	if(queList->actual == NULL) {
		switch_task(IDLE);

		return;
	}

	queList->actual->time_left--;
	queList->actual->length--;

	// if q_elem done remove and search for new q_elem for the cpu
	// else put actual q_elem in a higher que and search for new q_elem for cpu
	if (queList->actual->length == 0) {
		free(queList->actual);

		//search for best new work
		struct Queue *current = queList->root;

		int i;
		for (i = 0; i < queList->num_queues; i++) {
			if (current->root != NULL) {
				queList->actual = current->root;
				current->root = current->root->next;				
				
				switch_task(queList->actual->id);

				return;
			}

			current = current->next;
		}

		switch_task(IDLE);
	} else if (queList->actual->time_left == 0) {
		//put in next que
		queList->actual->nr++;

		if (queList->actual->nr == queList->num_queues) {
			queList->actual->time_left = INT_MAX;
		} else {
			queList->actual->time_left = queList->actual->nr * queList->time_step;
		}
		struct Queue *current = queList->root;

		int i;
		for (i = 0; i < queList->actual->nr; i++) {
			current = current->next;
		}

		q_elem *mover = current->root;

		if(mover == NULL) {
			current->root = queList->actual;
		} else {		
			while (1) {
				if (mover->next == NULL) {
					mover->next = queList->actual;

					break;
				}

				mover = mover->next;
			}
		}
		
		//search for best new work
		current = queList->root;

		for (i = 0; i < queList->num_queues; i++) {
			if (current->root != NULL) {
				queList->actual = current->root;
				current->root = current->root->next;				
				
				switch_task(queList->actual->id);

				return;
			}

			current = current->next;
		}

		switch_task(IDLE);
	}
}

void finish_MLF()
{
	// TODO (optional)
}
