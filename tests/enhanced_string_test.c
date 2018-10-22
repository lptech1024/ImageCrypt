#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../src/tools/enhanced_string.h"

int main()
{
	char *end_strings[] = { "test", NULL };
	enhanced_char_pointer_array *ecpa = create_enhanced_char_pointer_array_initial(end_strings);

	char *test = "test";
	printf("[%d]\n", char_pointer_ends_with_enhanced_char_pointer_array(test, ecpa));

	destroy_enhanced_char_pointer_array(ecpa);
}
