#include "LCFS.h"
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


int init_LCFS()
{
	// TODO
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

void free_LCFS()
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

void arrive_LCFS(int id, int length)
{
	// TODO
	q_elem *new = (q_elem*)malloc(sizeof(q_elem));
	new->id = id;
	new->length = length;

	if (que->actual == NULL) {
		que->actual = new;
		switch_task(que->actual->id);
	} else {
		new->next = que->root;
		que->root = new;
	}
}

void tick_LCFS()
{
	// UNUSED
}

void finish_LCFS()
{
	// TODO
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
