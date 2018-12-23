#include <stdio.h>
#include "tools/string_collection.h"
#include "tools/fpe.h"
#include "tools/cryptography.h"
#include "determine_format.h"
#include "user_input_handling.h"

void handle_user_inputs(transform_details_iterator *iterator, const char *passphrase, cryptography_mode cryptography_mode)
{
	//printf("handle_user_inputs start\n");
	const unsigned int key_length = 32;
	unsigned char *key = malloc(key_length);
	set_key(passphrase, key, key_length);

	//printf("\thandle_user_inputs past set_key\n");
	FPE_KEY *ff1 = malloc(sizeof(*ff1));
	fpe_set_ff1_key(key, 8 * key_length, NULL, 0, 8 * key_length, ff1);

	//printf("\thandle_user_inputs past fpe_set_ff1_key\n");
	set_conversion(iterator);

	//printf("\thandle_user_inputs past set_conversion\n");
	for (transform_details *current = transform_details_iterator_first(iterator); current; current = transform_details_iterator_next(iterator))
	{
		//printf("\tconvert looped\n");
		if (current->convert)
		{
			//printf("\t\ttrying to convert\n");
			current->convert(current, ff1, cryptography_mode);
		}
	}

	fpe_unset_ff1_key(ff1);
	free(ff1);
	//printf("handle_user_inputs end\n");
}
