#include <stdio.h>
#include <stdlib.h>

#include "MLF.h"
#include "task.h"


//for MLF & MLF
int main()
{
	if (init_MLF(4, 4))
	{
		printf("%i\n", running_task); // -1
		arrive_MLF(10, 5);
		printf("%i\n", running_task); // 10
		tick_MLF();
		tick_MLF();
		arrive_MLF(2, 10);
		tick_MLF();
		tick_MLF();
		tick_MLF();
		printf("%i\n", running_task); // 2
	}
	else
	{
		printf("MLF is not implemented yet. Quitting.\n");
	}

	return 0;
}
