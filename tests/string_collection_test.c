#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../src/tools/string_collection.h"

int main()
{
	char *end_strings[] = { "test", NULL };
	string_collection *ecpa = create_string_collection_initial(end_strings);

	char *test = "test";
	printf("[%d]\n", char_pointer_ends_with_string_collection(test, ecpa));

	destroy_string_collection(ecpa);
}
