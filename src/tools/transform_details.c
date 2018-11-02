#include <stdlib.h>
#include "transform_details.h"

file_details* create_file_details()
{
	return malloc(sizeof(file_details *));
}

void destroy_file_details(file_details *file_details)
{
	free(file_details->file_path);
	free(file_details->file);
	free(file_details);
}

transform_details* create_append_transform_details(transform_details *previous)
{
	transform_details *new_transform_details = malloc(sizeof(transform_details *));
	new_transform_details->previous = previous;
	new_transform_details->input = create_file_details();
	new_transform_details->output = create_file_details();
	new_transform_details->next = NULL;
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
	transform_details_iterator *iterator = malloc(sizeof(transform_details_iterator *));
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

	return iterator->current;
}

transform_details* transform_details_iterator_next(transform_details_iterator *iterator)
{
	if (iterator->current->next)
	{
		iterator->current = iterator->current->next;
	}

	return iterator->current;
}
