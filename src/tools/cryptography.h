#ifndef IMAGECRYPT_CRYPTOGRAPHY
#define IMAGECRYPT_CRYPTOGRAPHY

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
	NOCRYPT = 0,
	ENCRYPT,
	DECRYPT
} cryptography_mode;

bool set_key(const char *passphrase, unsigned char *key, unsigned int key_length);

#ifdef __cplusplus
}
#endif

#endif
