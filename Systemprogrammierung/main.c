#include <stdio.h>
#include <stdlib.h>

#include "SJN.h"
#include "task.h"

int main()
{
	// just a very basic example to show you, how to test your implementation (in this case: SJN)
	if (init_SJN())
	{
		arrive_SJN(0, 1);
		arrive_SJN(1, 2);
		arrive_SJN(2, 3);

		printf("%i\n", running_task);	// should print: 0
		finish_SJN();
		printf("%i\n", running_task);	// should print: 1
		finish_SJN();
		printf("%i\n", running_task);	// should print: 2
		finish_SJN();
		printf("%i\n", running_task);	// should print: -1 (no process left)
	}
	else
	{
		printf("SJN is not implemented yet. Quitting.\n");
	}

	return 0;
}
