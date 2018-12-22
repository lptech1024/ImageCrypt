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
	transform_details_iterator_reset(iterator);
	set_conversion(iterator);

	//printf("\thandle_user_inputs past set_conversion\n");
	transform_details_iterator_reset(iterator);
	do
	{
		//printf("\tconvert looped\n");
		if (iterator->current->convert)
		{
			//printf("\t\ttrying to convert\n");
			iterator->current->convert(iterator->current, ff1, cryptography_mode);
		}
	}
	while(transform_details_iterator_next(iterator));
	fpe_unset_ff1_key(ff1);
	free(ff1);
	//printf("handle_user_inputs end\n");
}
