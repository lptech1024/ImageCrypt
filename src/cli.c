#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include "tools/enhanced_string.h"

int main(int argc, char *argv[])
{
	unsigned int counter = 1;

	char *passphrase_aliases[] = { "-p", "--passphrase", NULL };
	enhanced_char_pointer_array *passphrase_arguments = create_enhanced_char_pointer_array2(passphrase_aliases);

	const char *passphrase = NULL;
	// User may pass password as argument
	// This will be too large (inefficent) if we add additional arguments
	enhanced_char_pointer_array *file_paths = create_enhanced_char_pointer_array(argc - 2);

	while (counter < argc)
	{
		char *current_parameter = argv[counter++];

		if (char_pointer_starts_with_enhanced_char_pointer_array(current_parameter, passphrase_arguments))
		{
			if (passphrase)
			{
				fprintf(stderr, "%s", "Multiple passphrase arguments supplied!\n");
				exit(1);
			}

			passphrase = argv[counter++];
			if (!passphrase)
			{
				printf("No passphrase provided!\n");
				return 1;
			}
		}
		else
		{
			append_enhanced_char_pointer_array(file_paths, current_parameter);
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

	if (!file_paths->count)
	{
		fprintf(stderr, "%s", "No file paths specified!\n");
		exit(1);
	}

	handle_user_inputs(passphrase, file_paths);

	return 0;
}
