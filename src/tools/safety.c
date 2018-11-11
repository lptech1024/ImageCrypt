#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

char* strdup_or_exit(const char *s)
{
	char *result = strdup(s);
	if (!result)
	{
		perror("strdup");
		exit(-1);
	}
	else
	{
		return result;
	}
}
