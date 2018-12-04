#ifndef IMAGECRYPT_PNG
#define IMAGECRYPT_PNG

#include <stdbool.h>
#include <stdint.h>
#include "tools/transform_details.h"
#include "tools/status.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
	const unsigned char[5] name;
	const bool cryptygraphy_applies;
} png_chunk_spec;

typedef struct {
	const unsigned char[5] name;
	const uint32_t data_size;
	const unsigned char[] data;
	const uint32_t crc32;
} png_chunk;

status is_png(file_details *file_details);

//status encrypt_png(transform_details *details);

//status decrypt_png(transform_details *details);

#ifdef __cplusplus
}
#endif

#endif
