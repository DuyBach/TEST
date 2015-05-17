#include <stdio.h>
#include <stdlib.h>

#include "SRTN.h"
#include "task.h"


//for SJN & SRTN
/*
int main()
{
	// just a very basic example to show you, how to test your implementation (in this case: SRTN)
	if (init_SRTN())
	{
		arrive_SRTN(0, 1);
		arrive_SRTN(1, 2);
		arrive_SRTN(2, 3);

		printf("%i\n", running_task);	// should print: 0
		finish_SRTN();
		printf("%i\n", running_task);	// should print: 1
		finish_SRTN();
		printf("%i\n", running_task);	// should print: 2
		finish_SRTN();
		printf("%i\n", running_task);	// should print: -1 (no process left)
	}
	else
	{
		printf("SRTN is not implemented yet. Quitting.\n");
	}

	return 0;
}
*/

//for SRTN & MLF
int main()
{
	if (init_SRTN())
	{
		printf("%i\n", running_task); // should print: -1
		arrive_SRTN(20, 5);
		arrive_SRTN(10, 5);
		printf("%i\n", running_task); // should print: 20
		tick_SRTN();
		tick_SRTN();
		tick_SRTN();
		tick_SRTN();
		tick_SRTN();
		printf("%i\n", running_task); // should print: 10
	}
	else
	{
		printf("SRTN is not implemented yet. Quitting.\n");
	}

	return 0;
}
