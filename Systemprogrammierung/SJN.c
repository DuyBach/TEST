#include "SJN.h"
#include "task.h"

#include <stdio.h>
#include <stdlib.h>


// struct for queue
typedef struct q_elem{
	int id;
	int length;
	struct q_elem *next;
} q_elem;

struct Queue{
	q_elem *actual;
	q_elem *root;
};


struct Queue *que;


int init_SJN()
{
	// creating queue
	que = (struct Queue*)malloc(sizeof(struct Queue));
	if (que == NULL) {
		printf("FAILED TO CREATE QUEUE\n");
		exit(1);
	}

	que->actual = NULL;
	que->root = NULL;

	switch_task(IDLE);

	return 1;
}

void free_SJN()
{
	// free queue
	free(que->actual);
	q_elem *current, *next;

	current = que->root;

	while(current != NULL) {
		next = current->next;
		free(current);
		current = next;
	}
}

void arrive_SJN(int id, int length)
{
	// create new q_elem
	q_elem *new = (q_elem*)malloc(sizeof(q_elem));
	new->id = id;
	new->length = length;

	// put new in cpu or in queue
	// if not put in cpu put in the queue with the shortest job order
	if (que->actual == NULL) {
		que->actual = new;
		switch_task(que->actual->id);
	} else if (que->root == NULL){
		que->root = new;
	} else {
		q_elem *before, *current;
		current = que->root;		
		while(current->next != NULL) {
			before = current;
			current = current->next;
			
			if (current->length > new->length) {
				before->next = new;
				new->next = current;

				return;
			}
		}

		current->next = new;
	}
}

void tick_SJN()
{
	// UNUSED
}

void finish_SJN()
{
	// free q_elem in cpu and put new q_elem in cpu
	free(que->actual);
	if (que->root == NULL){
		que->actual = NULL;

		switch_task(IDLE);
	} else {
		que->actual = que->root;
		que->root = que->root->next;

		switch_task(que->actual->id);
	}
}
