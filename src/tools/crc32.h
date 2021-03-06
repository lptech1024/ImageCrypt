#ifndef IMAGECRYPT_CRC32
#define IMAGECRYPT_CRC32

#include <stdint.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

uint32_t crc(unsigned char *buf, size_t len);

uint32_t update_crc(uint32_t crc, const unsigned char *buf, size_t len);

#ifdef __cplusplus
}
#endif

#endif
