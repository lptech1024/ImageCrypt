#include <stdlib.h>
#include "safety.h"
#include "transform_details.h"

file_details* create_file_details(const char *path)
{
	file_details *details = malloc_or_exit(sizeof(file_details *));
	details->file_path = path;
	return details;
}

void destroy_file_details(file_details *file_details)
{
	free((void *) file_details->file_path);
	free(file_details->file);
	free(file_details);
}

transform_details* create_append_transform_details(transform_details *previous, const char *input_path, const char *output_path)
{
	transform_details *new_transform_details = malloc_or_exit(sizeof(transform_details *));
	new_transform_details->previous = previous;
	new_transform_details->input = create_file_details(input_path);
	new_transform_details->output = create_file_details(output_path);
	new_transform_details->next = NULL;

	if (previous)
	{
		if (previous->next)
		{
			previous->next->previous = new_transform_details;
			new_transform_details->next = previous->next;
		}

		previous->next = new_transform_details;
	}

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
	transform_details_iterator *iterator = malloc_or_exit(sizeof(transform_details_iterator *));
	iterator->head = head;
	iterator->current = head;
	return iterator;
}

void destroy_transform_details_iterator(transform_details_iterator *transform_details_iterator)
{
	free(transform_details_iterator);
}

void transform_details_iterator_reset(transform_details_iterator *iterator)
{
	iterator->current = iterator->head;
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
	transform_details *next = iterator->current->next;

	if (next)
	{
		iterator->current = next;
	}

	return next;
}

void transform_details_iterator_append(transform_details_iterator **iterator, const char *input_path, const char *output_path)
{
	transform_details *current = (*iterator) ? (*iterator)->current : NULL;
	transform_details *new_transform_details = create_append_transform_details(current, input_path, output_path);

	if (!(*iterator))
	{
		*iterator = create_transform_details_iterator(new_transform_details);
	}
	else
	{
		transform_details_iterator_next(*iterator);
	}
}
