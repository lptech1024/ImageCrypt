#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <termios.h>
#include "tools/string_collection.h"
#include "tools/transform_details.h"
#include "tools/cryptography.h"
#include "user_input_handling.h"

bool get_passphrase(char *buffer)
{
	struct termios visible, hidden;

	if (tcgetattr(fileno(stdin), &visible))
	{
		fprintf(stderr, "%s", "Couldn't get stdin attributes.\n");
		exit(-1);
	}

	hidden = visible;
	hidden.c_lflag &= ~(ECHO | ECHOE | ECHOK | ECHONL | ICANON);
	if (tcsetattr(fileno(stdin), TCSAFLUSH, &hidden))
	{
		fprintf(stderr, "%s", "Could not protect passphrase echo.\n");
		exit(-1);
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

	char *input_aliases[] = { "-i", "--input", NULL };
	string_collection *input_arguments = create_string_collection_initial(input_aliases);

	char *output_aliases[] = { "-o", "--output", NULL };
	string_collection *output_arguments = create_string_collection_initial(output_aliases);

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
				exit(-1);
			}

			passphrase = argv[counter++];
			if (!passphrase)
			{
				fprintf(stderr, "No passphrase provided!\n");
				exit(-1);
			}
		}
		else if (string_collection_contains_string(encrypt_arguments, current_parameter))
		{
			if (cryptography_mode)
			{
				fprintf(stderr, "Multiple cryptography modes specified!\n");
				exit(-1);
			}

			cryptography_mode = ENCRYPT;
		}
		else if (string_collection_contains_string(decrypt_arguments, current_parameter))
		{
			if (cryptography_mode)
			{
				fprintf(stderr, "Multiple cryptography modes specified!\n");
				exit(-1);
			}

			cryptography_mode = DECRYPT;
		}
		else if (string_collection_contains_string(output_arguments, current_parameter))
		{
			const char *output_parameter = argv[counter++];

			current_parameter = argv[counter];

			if (!current_parameter || !string_collection_contains_string(input_arguments, current_parameter))
			{
				fprintf(stderr, "Missing input parameter for [%s]\n", output_parameter);
				exit(-1);
			}

			current_parameter = argv[++counter];
			if (!current_parameter)
			{
				fprintf(stderr, "Missing input file for [%s]\n", output_parameter);
				exit(-1);
			}

			counter++;

			transform_details_iterator_append(&transform_details_iterator, current_parameter, output_parameter);
		}
		else
		{
			const char *input_file_path;
			char *output_file_path = NULL;

			if (current_parameter && string_collection_contains_string(input_arguments, current_parameter))
			{
				counter++;
			}

			input_file_path = argv[counter - 1];
			current_parameter = argv[counter];

			if (current_parameter && string_collection_contains_string(output_arguments, current_parameter))
			{
				counter++;
				output_file_path = argv[counter++];
				if (!output_file_path)
				{
					fprintf(stderr, "Missing output file path for [%s]\n", current_parameter);
					exit(-1);
				}
			}

			transform_details_iterator_append(&transform_details_iterator, input_file_path, output_file_path);
		}
	}

	bool sane_user_inputs = true;

	if (cryptography_mode == NOCRYPT)
	{
		sane_user_inputs = false;
		fprintf(stderr, "%s", "No cryptography mode specified!\n");
	}

	if (!transform_details_iterator)
	{
		sane_user_inputs = false;
		fprintf(stderr, "%s", "No file paths specified!\n");
	}

	// Don't bother to prompt for a passphrase if we won't be able to use it
	if (sane_user_inputs && !passphrase)
	{
		printf("%s", "Please enter a passphrase: ");
		bool get_passphrase_successful = get_passphrase(passphrase);
		printf("\n");
		if (!get_passphrase_successful)
		{
			sane_user_inputs = false;
			fprintf(stderr, "%s", "No passphrase supplied!\n");
		}
	}

	if (!sane_user_inputs)
	{
		exit(-1);
	}

	transform_details_iterator_reset(transform_details_iterator);
	do
	{
		if (!transform_details_iterator->current->output->file_path)
		{
			default_output_file_path(transform_details_iterator->current, cryptography_mode);
		}
	}
	while(transform_details_iterator_next(transform_details_iterator));

	handle_user_inputs(transform_details_iterator, passphrase, cryptography_mode);

	return EXIT_SUCCESS;
}
