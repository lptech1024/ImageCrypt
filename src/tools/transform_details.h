#ifndef IMAGECRYPT_TRANSFORM_DETAILS
#define IMAGECRYPT_TRANSFORM_DETAILS

#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
	FILE *file;
	const char *file_path;
} file_details;

typedef struct transform_details {
	struct transform_details *previous;
	//IMAGE_FORMAT format;
	file_details *input;
	file_details *output;
	struct transform_details *next;
} transform_details;

typedef struct {
	transform_details *head;
	transform_details *current;
} transform_details_iterator;

file_details* create_file_details(const char *path);

void destroy_file_details(file_details *file_details);

transform_details* create_append_transform_details(transform_details *previous, const char *input_path, const char *output_path);

void destroy_transform_details(transform_details *transform_details);

transform_details_iterator* create_transform_details_iterator(transform_details *head);

void destroy_transform_details_iterator(transform_details_iterator *transform_details_iterator);

void transform_details_iterator_reset(transform_details_iterator *iterator);

transform_details* transform_details_iterator_previous(transform_details_iterator *iterator);

transform_details* transform_details_iterator_next(transform_details_iterator *iterator);

transform_details* transform_details_iterator_append(transform_details_iterator **iterator, const char *input_path, const char *output_path);

#ifdef __cplusplus
}
#endif

#endif
