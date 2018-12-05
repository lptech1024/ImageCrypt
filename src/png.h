#ifndef IMAGECRYPT_PNG
#define IMAGECRYPT_PNG

#include <stdbool.h>
#include <stdint.h>
#include "tools/transform_details.h"
#include "tools/status.h"

#ifdef __cplusplus
extern "C" {
#endif

#define PNG_NAME_LENGTH 5

typedef struct {
	const unsigned char[PNG_NAME_LENGTH] name;
	const bool cryptygraphy_applies;
} png_chunk_spec;

// Currently in on-disk order. Move crc32 to [1] for potential alignment improvement?
typedef struct {
	const uint32_t data_size;
	const unsigned char[PNG_NAME_LENGTH] name;
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
