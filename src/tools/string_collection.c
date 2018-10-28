#include <string.h>
#include <stdlib.h>
#include "string_collection.h"

string_collection* create_string_collection(int initial_size)
{
	string_collection *ecpa = malloc(sizeof(string_collection));
	ecpa->char_pointer_array = malloc(initial_size * sizeof(char *));
	ecpa->size = initial_size;
	ecpa->count = 0;
	return ecpa;
}

string_collection* create_string_collection_initial(char **initial)
{
	int initial_count = 0;
	for (int i = 0; initial[i]; i++)
	{
		initial_count++;
	}

	string_collection *ecpa = create_string_collection(initial_count);

	for (int i = 0; initial[i]; i++)
	{
		append_string_collection(ecpa, initial[i]);
	}

	return ecpa;
}

void destroy_string_collection(string_collection *ecpa)
{
	while (ecpa->count)
	{
		free(ecpa->char_pointer_array[--ecpa->count]);
	}

	free(ecpa->char_pointer_array);
	free(ecpa);
}

// NB: string is const
void append_string_collection(string_collection *enhanced_char_pointer, char *string)
{
	// Change size to allow for one more char *
	if ((enhanced_char_pointer->size) <= (enhanced_char_pointer->count))
	{
		enhanced_char_pointer->char_pointer_array = realloc(enhanced_char_pointer->char_pointer_array, (++enhanced_char_pointer->size) * sizeof(char *));
	}

	enhanced_char_pointer->char_pointer_array[enhanced_char_pointer->count++] = strdup(string);
}

bool char_pointer_starts_with_string_collection(char *string, string_collection *prefixes)
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

bool char_pointer_ends_with_string_collection(char *string, string_collection *suffixes)
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
