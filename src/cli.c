#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include "tools/string_collection.h"
#include "tools/transform_details.h"
#include "tools/cryptography.h"

int main(int argc, char *argv[])
{
	unsigned int counter = 1;

	char *passphrase_aliases[] = { "-p", "--passphrase", NULL };
	string_collection *passphrase_arguments = create_string_collection_initial(passphrase_aliases);

	char *encrypt_aliases[] = { "-e", "--encrypt", NULL };
	string_collection *encrypt_arguments = create_string_collection_initial(encrypt_aliases);

	char *decrypt_aliases[] = { "-d", "--decrypt", NULL };
	string_collection *decrypt_arguments = create_string_collection_initial(decrypt_aliases);

	cryptography_mode cryptography_mode = NOCRYPT;

	const char *passphrase = NULL;
	// User may pass password as argument
	// This will be too large (inefficent) if we add additional arguments
	//string_collection *file_paths = create_string_collection(argc - (passphrase_arguments->count));
	transform_details *first_transform_details = NULL;
	transform_details *last_transform_details = NULL;

	while (counter < argc)
	{
		char *current_parameter = argv[counter++];

		if (string_collection_contains_string(passphrase_arguments, current_parameter))
		{
			if (passphrase)
			{
				fprintf(stderr, "%s", "Multiple passphrase arguments supplied!\n");
				exit(1);
			}

			passphrase = argv[counter++];
			if (!passphrase)
			{
				fprintf(stderr, "No passphrase provided!\n");
				exit(1);
			}
		}
		else if (string_collection_contains_string(encrypt_arguments, current_parameter))
		{
			if (cryptography_mode)
			{
				fprintf(stderr, "Multiple cryptography modes specified!\n");
				exit(1);
			}

			cryptography_mode = ENCRYPT;
		}
		else if (string_collection_contains_string(decrypt_arguments, current_parameter))
		{
			if (cryptography_mode)
			{
				fprintf(stderr, "Multiple cryptography modes specified!\n");
				exit(1);
			}

			cryptography_mode = DECRYPT;
		}
		else
		{
			if (!first_transform_details)
			{
				first_transform_details = create_append_transform_details(NULL);
				last_transform_details = first_transform_details;
			}
			else
			{
				last_transform_details = create_append_transform_details(last_transform_details);
			}
		}
	}

	if (!passphrase)
	{
		fprintf(stderr, "%s", "No passphrase supplied!\n");
		exit(1);
		/* TODO: Prompt for passphrase
		   Preliminary research indicates it might be worth it to roll our own
		   Looks at getpass (obsolete), termios
		*/
	}

	if (!first_transform_details)
	{
		fprintf(stderr, "%s", "No file paths specified!\n");
		exit(1);
	}

	transform_details_iterator *file_iterator= create_transform_details_iterator(first_transform_details);

	// TODO: Uncomment when implemented
	//handle_user_inputs(file_iterator, passphrase, cryptography_mode);

	return 0;
}
