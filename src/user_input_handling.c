#include <stdio.h>
#include "tools/string_collection.h"
#include "tools/fpe.h"
#include "tools/cryptography.h"
#include "determine_format.h"
#include "user_input_handling.h"

void handle_user_inputs(transform_details_iterator *iterator, const char *passphrase, cryptography_mode cryptography_mode)
{
	//printf("handle_user_inputs start\n");
	const signed int key_length = 32;
	unsigned char key[key_length];
	// TODO: Handle return value
	set_key(passphrase, key, key_length);

	//printf("\thandle_user_inputs past set_key\n");
	// TODO: Handle errors
	FPE_KEY *ff1 = malloc(sizeof(*ff1));
	const signed int byte_length = 8;
	const signed int key_bits = byte_length * key_length;
	// TODO: Handle errors
	fpe_set_ff1_key(key, key_bits, NULL, 0, key_bits, ff1);

	//printf("\thandle_user_inputs past fpe_set_ff1_key\n");
	// TODO: Handle errors
	set_conversion(iterator);

	//printf("\thandle_user_inputs past set_conversion\n");
	for (transform_details *current = transform_details_iterator_first(iterator); current; current = transform_details_iterator_next_final(iterator))
	{
		//printf("\tconvert looped\n");
		if (current->convert)
		{
			//printf("\t\ttrying to convert\n");
			// TODO: Handle errors
			current->convert(current, ff1, cryptography_mode);
		}
	}

	fpe_unset_ff1_key(ff1);
	free(ff1);
	//printf("handle_user_inputs end\n");
}
