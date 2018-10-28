#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include "tools/string_collection.h"

int main(int argc, char *argv[])
{
	unsigned int counter = 1;

	char *passphrase_aliases[] = { "-p", "--passphrase", NULL };
	string_collection *passphrase_arguments = create_string_collection_initial(passphrase_aliases);

	const char *passphrase = NULL;
	// User may pass password as argument
	// This will be too large (inefficent) if we add additional arguments
	string_collection *file_paths = create_string_collection(argc - (passphrase_arguments->count));

	while (counter < argc)
	{
		char *current_parameter = argv[counter++];

		if (char_pointer_starts_with_string_collection(current_parameter, passphrase_arguments))
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
				exit(1);
			}
		}
		else
		{
			append_string_collection(file_paths, current_parameter);
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

	// TODO: Uncomment when implemented
	//handle_user_inputs(passphrase, file_paths);

	return 0;
}
