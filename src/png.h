#ifndef IMAGECRYPT_PNG
#define IMAGECRYPT_PNG

#include <stdbool.h>
#include <stdint.h>
#include "tools/status.h"
#include "tools/transform_details.h"
#include "tools/fpe.h"
#include "tools/cryptography.h"

#ifdef __cplusplus
extern "C" {
#endif

#define PNG_NAME_LENGTH 5

typedef struct {
	char *name;//const ... [PNG_NAME_LENGTH]
	const bool cryptography_applies;
} png_chunk_spec;

// Optimized for alignment
typedef struct {
	uint32_t data_size;
	uint32_t crc32;
	char *name;
	unsigned char *data;
} png_chunk;

return_status is_png(file_details *file_details);

return_status convert_png(transform_details *details, FPE_KEY *fpe_key, cryptography_mode cryptography_mode);

#ifdef __cplusplus
}
#endif

#endif
