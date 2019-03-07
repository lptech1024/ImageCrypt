#ifndef IMAGECRYPT_TRANSFORM_DETAILS
#define IMAGECRYPT_TRANSFORM_DETAILS

#include <stdio.h>
#include "fpe.h"
#include "status.h"
#include "cryptography.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
	FILE *file;
	char *file_path;
} file_details;

typedef struct transform_details {
	struct transform_details *previous;
	//
	return_status (*convert)(struct transform_details *details, FPE_KEY *fpe_key, cryptography_mode cryptography_mode);
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

void default_output_file_path(transform_details *transform_details, cryptography_mode cryptography_mode);

transform_details_iterator* create_transform_details_iterator(transform_details *head);

void destroy_transform_details_iterator(transform_details_iterator *transform_details_iterator);

transform_details* transform_details_iterator_first(transform_details_iterator *iterator);

transform_details* transform_details_iterator_previous(transform_details_iterator *iterator);

transform_details* transform_details_iterator_next(transform_details_iterator *iterator);

transform_details* transform_details_iterator_next_final(transform_details_iterator *iterator);

void transform_details_iterator_append(transform_details_iterator **iterator, const char *input_path, const char *output_path);

#ifdef __cplusplus
}
#endif

#endif
