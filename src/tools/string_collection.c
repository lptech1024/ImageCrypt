#include <string.h>
#include <stdlib.h>
#include "safety.h"
#include "string_collection.h"

string_collection* create_string_collection(int initial_size)
{
	string_collection *new_string_collection = malloc_or_exit(sizeof(string_collection));
	new_string_collection->strings = malloc_or_exit(initial_size * sizeof(char *));
	new_string_collection->size = initial_size;
	new_string_collection->count = 0;
	return new_string_collection;
}

string_collection* create_string_collection_initial(char **initial)
{
	int initial_count = 0;
	for (int i = 0; initial[i]; i++)
	{
		initial_count++;
	}

	string_collection *new_string_collection = create_string_collection(initial_count);

	for (int i = 0; initial[i]; i++)
	{
		append_string_collection(new_string_collection, initial[i]);
	}

	return new_string_collection;
}

void destroy_string_collection(string_collection *collection)
{
	while (collection->count)
	{
		free(collection->strings[--collection->count]);
	}

	free(collection->strings);
	free(collection);
}

// NB: string is const
void append_string_collection(string_collection *enhanced_char_pointer, char *string)
{
	// Change size to allow for one more char *
	if ((enhanced_char_pointer->size) <= (enhanced_char_pointer->count))
	{
		enhanced_char_pointer->strings = realloc(enhanced_char_pointer->strings, (++enhanced_char_pointer->size) * sizeof(char *));
	}

	enhanced_char_pointer->strings[enhanced_char_pointer->count++] = strdup(string);
}

bool string_collection_contains_string(string_collection *potential_matches, char *string)
{
	bool found_match = false;

	const int string_end_index = strlen(string) - 1;

	for (int i = 0; i < potential_matches->count; i++)
	{
		if (string_end_index != (strlen(potential_matches->strings[i]) - 1))
		{
			continue;
		}

		for (int j = 0; j <= string_end_index; j++)
		{
			if (string[j] != potential_matches->strings[i][j])
			{
				j = string_end_index;
				continue;
			}
			else if (j == string_end_index)
			{
				i = potential_matches->count;
				found_match = true;
			}
		}
	}

	return found_match;
}

bool string_starts_with_string_collection(char *string, string_collection *prefixes)
{
	bool starts_with_prefix = false;

	const int string_end_index = strlen(string) - 1;

	for (int i = 0; i < prefixes->count; i++)
	{
		const int prefix_end_index = strlen(prefixes->strings[i]) - 1;

		if (prefix_end_index < string_end_index)
		{
			continue;
		}

		for (int j = 0; j < prefix_end_index; j++)
		{
			if (string[j] != prefixes->strings[i][j])
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

bool string_ends_with_string_collection(char *string, string_collection *suffixes)
{
	bool ends_with_suffix = false;

	const int string_end_index = strlen(string) - 1;

	for (int i = 0; i < suffixes->count; i++)
	{
		const int suffix_end_index = strlen(suffixes->strings[i]) - 1;

		const int string_match_index = string_end_index - suffix_end_index;

		if (string_match_index < 0)
		{
			continue;
		}

		for (int cursor = string_end_index; cursor >= string_match_index; cursor--)
		{
			if (string[cursor] != suffixes->strings[i][cursor - string_match_index])
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
