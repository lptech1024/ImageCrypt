#include <stdio.h>
#include <string.h>
#include "../src/third_party/format_preserving_encryption/fpe.h"

#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>

void hex2ints(unsigned char *hex, unsigned int hex_length, unsigned int *result)
{
	for (int i = 0; i < hex_length; i++)
	{
		result[i] = hex[i];
	}
}

int main()
{
	unsigned char sample_data[] = { 0x87, 0x0A, 0x00, 0x00, 0x8C, 0x0D, 0x00, 0x00, 0xFD, 0x4B, 0x00, 0x00, 0x81, 0x3F, 0x00, 0x00, 0x7D, 0x76, 0x00, 0x00, 0xE9, 0x8C, 0x00, 0x00, 0x3C, 0xE5, 0x00, 0x00, 0x19, 0xCD, 0xB0, 0xDF, 0xF5, 0x17, 0x00 };

	unsigned int sample_data_length = sizeof(sample_data) - 1;

	const unsigned char sample_256bit_key[] = { 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61 };

	unsigned int sample_data_ints[sample_data_length];
	hex2ints(sample_data, sample_data_length, sample_data_ints);

	printf("Converted:\n");
	for (int i = 0; i < sample_data_length; i++)
	{
		printf(" %03d", sample_data_ints[i]);
	}

	printf("\n\n");

	FPE_KEY ff1;
	FPE_set_ff1_key(sample_256bit_key, 8 * sizeof(sample_256bit_key), NULL, 0, 256, &ff1);

	unsigned int encrypted_data_ints[sample_data_length + 1];
	FPE_ff1_encrypt(sample_data_ints, encrypted_data_ints, sample_data_length, &ff1, FPE_ENCRYPT);

	printf("Encrypted:\n");
	for (int i = 0; i < sample_data_length; i++)
	{
		printf(" %03d", encrypted_data_ints[i]);
	}

	printf("\n\n");

	bool encrypted = false;
	for (int i = 0; i < sample_data_length; i++)
	{
		if (sample_data_ints[i] != encrypted_data_ints[i])
		{
			encrypted = true;
			break;
		}
	}

	if (!encrypted)
	{
		printf("Data was not successfully encrypted.\n\n");
	}

	unsigned int decrypted_data_ints[sample_data_length + 1];
	FPE_ff1_encrypt(encrypted_data_ints, decrypted_data_ints, sample_data_length, &ff1, FPE_DECRYPT);

	printf("Decrypted:\n");
	for (int i = 0; i < sample_data_length; i++)
	{
		printf(" %03d", decrypted_data_ints[i]);
	}

	printf("\n\n");

	for (int i = 0; i < sample_data_length; i++)
	{
		if (decrypted_data_ints[i] != sample_data_ints[i])
		{
			printf("Decrypted data is different than original!\n");
			break;
		}
	}

	FPE_unset_ff1_key(&ff1);

	return 0;
}
