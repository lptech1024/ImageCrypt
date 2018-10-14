#include <openssl/evp.h>
#include <stdbool.h>
#include "cryptography.h"

// Accepts user input and sets a key using a secure algorithm
// NB: Output must remain consistent once in use. Create other functions if we want to change the methodology/output.
// Returns success flag
bool set_256_bit_key(const char *passphrase, unsigned char *key)
{
	// Random, hardcoded salt to make effective use of rainbow tables more difficult/less feasilbe
   unsigned char salt[] = { 0xEC, 0xBF, 0x99, 0xA0, 0xEE, 0x4B, 0x1D, 0x76, 0x00 };

	return ((bool) PKCS5_PBKDF2_HMAC(passphrase, -1, salt, sizeof(salt) - 1, 100000, EVP_sha512(), (256 / 8), key));
}
