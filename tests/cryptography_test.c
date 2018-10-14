/*
 * gcc "../src/cryptography.c" cryptography_test.c -lcrypto -o cryptography_test
 */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "../src/cryptography.h"

int main()
{
	const char passphrase[] = { 0x00 };
	unsigned char user_key[33];
	unsigned char expected_user_key[] = { 0x96, 0xD4, 0x3E, 0xC5, 0x12, 0xB5, 0xF5, 0xDF, 0xBD, 0x18, 0x7A, 0x5, 0xBB, 0x68, 0xA1, 0xB3, 0x57, 0x87, 0x11, 0x5F, 0xF1, 0x8E, 0xCB, 0x58, 0x53, 0x40, 0x99, 0x84, 0x68, 0xC2, 0xB3, 0x67, 0x00  };
	bool success = set_256_bit_key(passphrase, user_key);
	if (!success)
	{
		printf("set_256_bit_key was unsuccessful.");
		exit(1);
	}
	else
	{
		for (int i = 0; i < sizeof(user_key) - 1; i++)
		{
			if (expected_user_key[i] != user_key[i])
			{
				printf("User key is different than expected.\n");
				break;
			}
		}
	}

	const char passphrase2[] = "abc123";
	unsigned char expected_user_key2[] = { 0x4E, 0x29, 0xB3, 0xF2, 0xC1, 0xB5, 0x3F, 0x2B, 0xC2, 0xDA, 0x59, 0xF9, 0x6A, 0x93, 0xC5, 0xDE, 0x57, 0x3, 0x9, 0xBF, 0xB0, 0x90, 0xEF, 0xC3, 0x3C, 0x7D, 0x59, 0x99, 0xDA, 0x18, 0xE5, 0x6D, 0x00  };
	success = set_256_bit_key(passphrase2, user_key);
	if (!success)
	{
		printf("set_256_bit_key was unsuccessful.");
		exit(1);
	}
	else
	{
		for (int i = 0; i < sizeof(user_key) - 1; i++)
		{
			if (expected_user_key2[i] != user_key[i])
			{
				printf("User key is different than expected.\n");
				break;
			}
		}
	}

	printf("DONE\n");

	return 0;
}
