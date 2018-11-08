#include <stdio.h>
#include <stdlib.h>
#include "safety.h"

void* malloc_or_exit(size_t size)
{
	void *result = malloc(size);
	if (!result)
	{
		perror("malloc");
		exit(-1);
	}
	else
	{
		return result;
	}
}
