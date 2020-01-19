#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "tools/status.h"
#include "tools/string_collection.h"
#include "tools/transform_details.h"
#include "png.h"
#include "determine_format.h"

typedef struct {
	return_status (*image_format_test)(file_details *);
	return_status (*convert)(transform_details *, FPE_KEY *, cryptography_mode);
	//unsigned char **typical_file_extensions;
} image_format;

const image_format image_formats[] =
{
	// Extensions won't be useful until we have more than a single format
	{ is_png, convert_png /*, { "png", "apng", NULL }*/ },
	{ NULL, NULL }
};

// Starting from the iterator's current position, set transform_details.convert if found
// If conversion is not possible, transform_details will not change
void set_conversion(transform_details_iterator *iterator)
{
	for (transform_details *current = transform_details_iterator_first(iterator); current; current = transform_details_iterator_next(iterator))
	{
		// TODO: Handle unexpected results
		FILE *input_file = fopen(iterator->current->input->file_path, "r");
		// TODO: Handle errors
		file_details *file_details = create_file_details(iterator->current->input->file_path);
		file_details->file = input_file;
		size_t index = 0;
		for (const image_format *current_image_format = &image_formats[index]; current_image_format->image_format_test; current_image_format = &image_formats[++index])
		{
			// TODO: Handle errors
			if (current_image_format->image_format_test(file_details))
			{
				//printf("converting...\n");
				iterator->current->convert = current_image_format->convert;
			}
		}

		// TODO: Note unexpected results for debugging
		fclose(input_file);
		destroy_file_details(file_details);
	}
}
