#include <string.h>
#include <stdlib.h>
#include "cryptography.h"
#include "transform_details.h"

file_details* create_file_details(const char *path)
{
	//printf("create_file_details start\n");
	file_details *details = malloc(sizeof(*details));
	details->file = NULL;
	details->file_path = path ? strdup(path) : NULL;
	//printf("create_file_details end\n");
	return details;
}

void destroy_file_details(file_details *file_details)
{
	free((void *) file_details->file_path);
	free(file_details);
}

transform_details* create_append_transform_details(transform_details *previous, const char *input_path, const char *output_path)
{
	//printf("create_append_transform_details start\n");
	transform_details *new_transform_details = malloc(sizeof(*new_transform_details));
	new_transform_details->convert = NULL;
	new_transform_details->previous = previous;
	new_transform_details->input = create_file_details(input_path);
	new_transform_details->output = create_file_details(output_path);
	new_transform_details->next = NULL;

	if (previous)
	{
		//printf("\tprevious\n");
		if (previous->next)
		{
			//printf("\t\tprevious->next\n");
			previous->next->previous = new_transform_details;
			new_transform_details->next = previous->next;
		}

		previous->next = new_transform_details;
	}

	//printf("create_append_transform_details end\n");
	return new_transform_details;
}

void destroy_transform_details(transform_details *transform_details)
{
	destroy_file_details(transform_details->output);
	destroy_file_details(transform_details->input);

	if (transform_details->previous)
	{
		transform_details->previous->next = transform_details->next;
	}

	if (transform_details->next)
	{
		transform_details->next->previous = transform_details->previous;
	}

	free(transform_details);
}

transform_details_iterator* create_transform_details_iterator(transform_details *head)
{
	//printf("create_transform_details_iterator start\n");
	transform_details_iterator *iterator = malloc(sizeof(*iterator));
	iterator->head = head;
	iterator->current = head;
	//printf("create_transform_details_iterator end\n");
	return iterator;
}

void destroy_transform_details_iterator(transform_details_iterator *transform_details_iterator)
{
	free(transform_details_iterator);
}

static void transform_details_iterator_reset(transform_details_iterator *iterator)
{
	iterator->current = iterator->head;
}

transform_details* transform_details_iterator_first(transform_details_iterator *iterator)
{
	transform_details_iterator_reset(iterator);
	return iterator->current;
}

// Trailing slash of input path will be dropped when no file extension provided
void default_output_file_path(transform_details *transform_details, cryptography_mode cryptography_mode)
{
	const char *unencrypted = "_unencrypted";
	const char *encrypted = "_encrypted";

	const char *mode_text = cryptography_mode == DECRYPT ? unencrypted : encrypted;
	const size_t mode_text_length = strlen(mode_text);

	size_t last_non_slash_index = strlen(transform_details->input->file_path) - 1;
	while (transform_details->input->file_path[last_non_slash_index] == '/')
	{
		last_non_slash_index--;
	}

	int last_slash_index = 0;
	int last_dot_index = 0;
	// Don't need to check 0th index
	for (int reversed_cursor = last_non_slash_index; reversed_cursor > 0; reversed_cursor--)
	{
		if (!last_slash_index && transform_details->input->file_path[reversed_cursor] == '/')
		{
			last_slash_index = reversed_cursor;
		}

		if (!last_dot_index && transform_details->input->file_path[reversed_cursor] == '.')
		{
			last_dot_index = reversed_cursor;
		}

		if (last_slash_index && last_dot_index)
		{
			break;
		}
	}

	if (last_slash_index >= last_dot_index)
	{
		transform_details->output->file_path = malloc(sizeof(char) * (last_non_slash_index + mode_text_length + 1));
		memcpy(transform_details->output->file_path, transform_details->input->file_path, last_non_slash_index);
		memcpy(transform_details->output->file_path + last_non_slash_index, mode_text, mode_text_length + 1);
	}
	else
	{
		transform_details->output->file_path = malloc(sizeof(char) * (strlen(transform_details->input->file_path) + mode_text_length + 1));
		memcpy(transform_details->output->file_path, transform_details->input->file_path, last_dot_index);
		memcpy(transform_details->output->file_path + last_dot_index, mode_text, mode_text_length);
		memcpy(transform_details->output->file_path + last_dot_index + mode_text_length, transform_details->input->file_path + last_dot_index, strlen(transform_details->input->file_path + last_dot_index) + 1);
	}

	//printf("\tDefaulted output file_path to [%s]\n", transform_details->output->file_path);
}

transform_details* transform_details_iterator_previous(transform_details_iterator *iterator)
{
	if (iterator->current->previous)
	{
		iterator->current = iterator->current->previous;
	}

	return iterator->current->previous;
}

transform_details* transform_details_iterator_next(transform_details_iterator *iterator)
{
	//printf("transform_details_iterator_next start\n");
	transform_details *next = iterator->current->next;

	if (next)
	{
		iterator->current = next;
		//printf("\tnext found\n");
	}

	//printf("transform_details_iterator_next end\n");
	return next;
}

transform_details* transform_details_iterator_next_final(transform_details_iterator *iterator)
{
	//printf("transform_details_iterator_next_final start\n");
	transform_details *current = iterator->current;

	transform_details *next = transform_details_iterator_next(iterator);

	destroy_transform_details(current);

	//printf("transform_details_iterator_next_final end\n");
	return next;
}

void transform_details_iterator_append(transform_details_iterator **iterator, const char *input_path, const char *output_path)
{
	//printf("transform_details_iterator_append start\n");
	transform_details *current = (*iterator) ? (*iterator)->current : NULL;
	transform_details *new_transform_details = create_append_transform_details(current, input_path, output_path);

	if (!(*iterator))
	{
		*iterator = create_transform_details_iterator(new_transform_details);
	}
	else
	{
		//printf("\ttransform_details_iterator_append next\n");
		transform_details_iterator_next(*iterator);
	}
	//printf("transform_details_iterator_append end\n");
}
