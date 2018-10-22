#include <string.h>
#include <stdlib.h>
#include "enhanced_string.h"

enhanced_char_pointer_array* create_enhanced_char_pointer_array(int initial_size)
{
	enhanced_char_pointer_array *ecpa = malloc(sizeof(enhanced_char_pointer_array));
	ecpa->char_pointer_array = malloc(initial_size * sizeof(char *));
	ecpa->size = initial_size;
	ecpa->count = 0;
	return ecpa;
}

enhanced_char_pointer_array* create_enhanced_char_pointer_array_initial(char **initial)
{
	int initial_count = 0;
	for (int i = 0; initial[i]; i++)
	{
		initial_count++;
	}

	enhanced_char_pointer_array *ecpa = create_enhanced_char_pointer_array(initial_count);

	for (int i = 0; initial[i]; i++)
	{
		append_enhanced_char_pointer_array(ecpa, initial[i]);
	}

	return ecpa;
}

void destroy_enhanced_char_pointer_array(enhanced_char_pointer_array *ecpa)
{
	while (ecpa->count)
	{
		free(ecpa->char_pointer_array[--ecpa->count]);
	}

	free(ecpa->char_pointer_array);
	free(ecpa);
}

// NB: string is const
void append_enhanced_char_pointer_array(enhanced_char_pointer_array *enhanced_char_pointer, char *string)
{
	// Change size to allow for one more char *
	// TODO: What if size is already large enough?
	if ((enhanced_char_pointer->size) < (enhanced_char_pointer->count + 1))
	{
		enhanced_char_pointer->char_pointer_array = realloc(enhanced_char_pointer->char_pointer_array, (++enhanced_char_pointer->size) * sizeof(char *));
	}

	enhanced_char_pointer->char_pointer_array[enhanced_char_pointer->count++] = strdup(string);
}

bool char_pointer_starts_with_enhanced_char_pointer_array(char *string, enhanced_char_pointer_array *prefixes)
{
	bool starts_with_prefix = false;

	const int string_end_index = strlen(string) - 1;

	for (int i = 0; i < prefixes->count; i++)
	{
		const int prefix_end_index = strlen(prefixes->char_pointer_array[i]) - 1;

		if (prefix_end_index < string_end_index)
		{
			continue;
		}

		for (int j = 0; j < prefix_end_index; j++)
		{
			if (string[j] != prefixes->char_pointer_array[i][j])
			{
				j = prefix_end_index;
				continue;
			}
			else if ((j + 1) == prefix_end_index)
			{
				i = prefixes->count;
				starts_with_prefix = true;
			}
		}
	}

	return starts_with_prefix;
}

bool char_pointer_ends_with_enhanced_char_pointer_array(char *string, enhanced_char_pointer_array *suffixes)
{
	bool ends_with_suffix = false;

	const int string_end_index = strlen(string) - 1;

	for (int i = 0; i < suffixes->count; i++)
	{
		const int suffix_end_index = strlen(suffixes->char_pointer_array[i]) - 1;

		const int string_match_index = string_end_index - suffix_end_index;

		if (string_match_index < 0)
		{
			continue;
		}

		for (int cursor = string_end_index; cursor >= string_match_index; cursor--)
		{
			if (string[cursor] != suffixes->char_pointer_array[i][cursor - string_match_index])
			{
				cursor = string_match_index - 1;
				continue;
			}
			else if (cursor == string_match_index)
			{
				i = suffixes->count;
				ends_with_suffix = true;
			}
		}
	}

	return ends_with_suffix;
}
