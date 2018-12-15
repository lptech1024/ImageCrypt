#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "tools/status.h"
#include "tools/string_collection.h"
#include "tools/transform_details.h"
#include "png.h"
#include "determine_format.h"

typedef struct {
	status (*image_format_test)(file_details *file_details);
	status (*convert)(transform_details *details, FPE_KEY *fpe_key, cryptography_mode cryptography_mode);
	//unsigned char *typical_file_extensions;
} image_format;

// Extensions won't be useful until we have more than a single format
const image_format image_formats[] =
{
	{ &is_png, &convert_png /*, { "png", "apng", NULL }*/ },
	NULL
};

// Starting from the iterator's current position, set transform_details.convert if found
// If conversion is not possible, transform_details will not change
void set_conversion(transform_details_iterator *iterator)
{
	do
	{
		FILE *input_file = fopen(iterator->current->input->file_path, "r");
		size_t index = 0;
		image_format current = NULL;
		while ((image_format = image_formats[index++]));
		{
			if (image_format->image_format_test(input_file))
			{
				iterator->current->convert = image_format.convert;
			}
		}

		fclose(input_file);
	}
	while(transform_details_iterator_next(iterator));
}
