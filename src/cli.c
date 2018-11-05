#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <termios.h>
#include "tools/string_collection.h"
#include "tools/transform_details.h"
#include "tools/cryptography.h"

bool get_passphrase(char *buffer)
{
	struct termios visible, hidden;

	if (tcgetattr(fileno(stdin), &visible))
	{
		fprintf(stderr, "%s", "Couldn't get stdin attributes.\n");
		exit(1);
	}

	hidden = visible;
	hidden.c_lflag &= ~(ECHO | ECHOE | ECHOK | ECHONL | ICANON);
	if (tcsetattr(fileno(stdin), TCSAFLUSH, &hidden))
	{
		fprintf(stderr, "%s", "Could not protect passphrase echo.\n");
		exit(1);
	}

	size_t buffer_length = 0;
	// NB: Backspace does not work as expected
	int chars_read = getline(&buffer, &buffer_length, stdin);
	if (buffer[chars_read - 1] == '\n')
	{
		buffer[--chars_read] = '\0';
	}

	if (tcsetattr(fileno(stdin), TCSAFLUSH, &visible))
	{
		fprintf(stderr, "%s", "Could not restore normal echo. It is recommended to replace this terminal after completion.\n");
	}

	return ((bool) chars_read);
}

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

	char *passphrase = NULL;

	transform_details_iterator *transform_details_iterator = NULL;

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
			// TODO: Output file name
			transform_details_iterator_append(&transform_details_iterator, current_parameter, current_parameter);
		}
	}

	bool sane_user_inputs = true;

	if (!passphrase)
	{
		printf("%s", "Please enter a passphrase: ");
		bool get_passphrase_successful = get_passphrase(passphrase);
		printf("\n");
		if (!get_passphrase_successful)
		{
			sane_user_inputs = false;
			printf("%s", "No passphrase supplied!\n");
		}
	}

	if (!transform_details_iterator)
	{
		sane_user_inputs = false;
		fprintf(stderr, "%s", "No file paths specified!\n");
	}

	if (!sane_user_inputs)
	{
		exit(1);
	}

	transform_details_iterator_reset(&transform_details_iterator);

	// TODO: Uncomment when implemented
	//handle_user_inputs(transform_details_iterator, passphrase, cryptography_mode);

	return 0;
}
