#include "tools/string_collection.h"
#include "tools/fpe.h"
#include "tools/cryptography.h"
#include "determine_format.h"
#include "user_input_handling.h"

void handle_user_inputs(transform_details_iterator *iterator, const char *passphrase, cryptography_mode cryptography_mode)
{
	unsigned char *key = NULL;
	unsigned int key_length = 32;
	set_key(passphrase, key, key_length);

	FPE_KEY ff1;
	fpe_set_ff1_key(key, key_length, NULL, 0, key_length, &ff1);

	transform_details_iterator_reset(iterator);
	set_conversion(iterator);

	transform_details_iterator_reset(iterator);
	do
	{
		if (iterator->current->convert)
		{
			iterator->current->convert(iterator->current, &ff1, cryptography_mode);
		}
	}
	while(transform_details_iterator_next(iterator));
}
