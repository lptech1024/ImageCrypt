#ifndef IMAGECRYPT_USER_INPUT_HANDLING
#define IMAGECRYPT_USER_INPUT_HANDLING

#include "tools/transform_details.h"

#ifdef __cplusplus
extern "C" {
#endif

void handle_user_inputs(transform_details_iterator *iterator, const char *passphrase, cryptography_mode cryptography_mode);

#ifdef __cplusplus
}
#endif

#endif
