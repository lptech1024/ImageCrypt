#include <openssl/evp.h>
#include <stdbool.h>
#include "cryptography.h"

// Accepts user input and sets a key using a secure algorithm
// NB: Output must remain consistent once in use. Create other functions if we want to change the methodology/output.
// Returns success flag
bool set_256_bit_key(const char *passphrase, unsigned char *key)
{
	// Random, hardcoded salt to make effective use of rainbow tables more difficult/less feasilbe
	const unsigned char output_size_salt[] = { 0xEC, 0xBF, 0x99, 0xA0, 0xEE, 0x4B, 0x1D, 0x76, 0x9C, 0xB9, 0x1C, 0xBD, 0xAE, 0x08, 0x25, 0x53, 0x8B, 0x21, 0x5F, 0xCB, 0xC0, 0x78, 0x3C, 0x04, 0xEE, 0xBE, 0x79, 0x85, 0x2E, 0x5C, 0xB4, 0x58 };

	return ((bool) PKCS5_PBKDF2_HMAC(passphrase, -1, output_size_salt, sizeof(output_size_salt), 100000, EVP_sha512(), (256 / 8), key));
}
