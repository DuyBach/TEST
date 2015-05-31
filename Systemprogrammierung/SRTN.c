#include "SRTN.h"
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


int init_SRTN()
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

void free_SRTN()
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

void arrive_SRTN(int id, int length)
{
	// TODO
	q_elem *new = (q_elem*)malloc(sizeof(q_elem));
	new->id = id;
	new->length = length;
	new->next = NULL;

	if (que->actual == NULL) {
		que->actual = new;
		switch_task(que->actual->id);
	} else if (que->actual->length > new->length) {
		que->actual->next = que->root;
		que->root = que->actual;

		que->actual = new;

		switch_task(que->actual->id);
	} else if (que->root == NULL) {
		que->root = new;
	} else {
		q_elem *before, *current;
		current = que->root;	

		if (current->length > new->length) {
			new->next = current;
			que->root = new;

			return;
		}

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

void tick_SRTN()
{
	// TODO
	if (que->actual != NULL && que->actual->length != 0) {
		que->actual->length--;

		if (que->actual->length == 0) {
			free(que->actual);

			if (que->root != NULL) {
				que->actual = que->root;
				que->root = que->root->next;

				switch_task(que->actual->id);
			} else {
				switch_task(IDLE);
			}
		}
	} else {
		free(que->actual);

		//que->actual not filled
		if (que->root != NULL) {
			que->actual = que->root;
			que->root = que->root->next;

			switch_task(que->actual->id);
		} else {
			switch_task(IDLE);
		}
	}
}

void finish_SRTN()
{
	// TODO (optional)
}
