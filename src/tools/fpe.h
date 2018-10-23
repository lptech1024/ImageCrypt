#ifndef IMAGECRYPT_FPE
#define IMAGECRYPT_FPE

#include <openssl/aes.h>
#include <openssl/bn.h>

#ifdef __cplusplus
extern "C" {
#endif

#define FPE_ENCRYPT 1
#define FPE_DECRYPT 0

typedef struct {
	unsigned int radix;
	unsigned int tweaklen;
	unsigned char *tweak;
	AES_KEY aes_enc_ctx;
} FPE_KEY;

/*** FF1 ***/
int fpe_set_ff1_key(const unsigned char *userKey, const int bits, const unsigned char *tweak, const unsigned int tweaklen, const int radix, FPE_KEY *key);

void fpe_unset_ff1_key(FPE_KEY *key);

void fpe_ff1_encrypt(unsigned int *in, unsigned int *out, unsigned int inlen, FPE_KEY *key, const int enc);

#ifdef __cplusplus
}
#endif

#endif
