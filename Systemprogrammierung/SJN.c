#include "SJN.h"
#include "task.h"

#include <stdio.h>
#include <stdlib.h>


// struct for queue
typedef struct q_elem{
	int id;
	int length;
	int work;
	struct q_elem *next;
} q_elem;

struct Queue{
	q_elem *actual;
	q_elem *root;
};


struct Queue *que;


int init_SJN()
{
	// TODO
	que = (struct Queue*)malloc(sizeof(struct Queue));
	if (que == NULL) {
		printf("FAILED TO CREATE QUEUE\n");
		exit(1);
	}

	que->actual = NULL;
	que->root = NULL;

	return 1;
}

void free_SJN()
{
	// TODO
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
	// TODO
	q_elem *new = (q_elem*)malloc(sizeof(q_elem));
	new->id = id;
	new->length = length;
	new->work = length;

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
			
			if (current->id > new->id) {
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
	// TODO
	if (que->root == NULL){
		free(que->actual);
		que->actual = NULL;

		switch_task(IDLE);
	} else {
		free(que->actual);
		que->actual = que->root;
		que->root = que->root->next;

		switch_task(que->actual->id);
	}
}
