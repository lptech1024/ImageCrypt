#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "../src/tools/cryptography.h"

int main()
{
	const char passphrase[] = { 0x00 };
	const int user_key_length = 32;
	unsigned char user_key[user_key_length];
	unsigned char expected_user_key[] = { 0x16, 0xAF, 0x48, 0xCC, 0xEE, 0x24, 0x82, 0xF3, 0xFB, 0x2C, 0x48, 0x40, 0x79, 0xCE, 0xC9, 0xBC, 0xC3, 0x66, 0x12, 0xE0, 0x68, 0xE7, 0x49, 0x26, 0xDF, 0x47, 0xF0, 0xC6, 0xC2, 0x25, 0x1F, 0x7C };
	bool success = set_key(passphrase, user_key, user_key_length);
	if (!success)
	{
		printf("set_key was unsuccessful.");
		exit(1);
	}
	else
	{
		for (int i = 0; i < sizeof(user_key); i++)
		{
			if (expected_user_key[i] != user_key[i])
			{
				printf("User key is different than expected.\n");
				break;
			}
		}
	}

	const char passphrase2[] = "abc123";
	unsigned char expected_user_key2[] = { 0xA9, 0x7F, 0x31, 0x73, 0x21, 0x89, 0x2E, 0x86, 0xBD, 0x9A, 0x05, 0xC1, 0x6C, 0x88, 0x54, 0xCC, 0x90, 0xA7, 0x01, 0x90, 0xCF, 0x6F, 0x93, 0x72, 0x65, 0x8A, 0xE4, 0x9F, 0xFF, 0x62, 0xDD, 0x44 };
	success = set_key(passphrase2, user_key, user_key_length);
	if (!success)
	{
		printf("set_key was unsuccessful.");
		exit(1);
	}
	else
	{
		for (int i = 0; i < sizeof(user_key); i++)
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
