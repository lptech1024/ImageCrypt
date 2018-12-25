#ifndef IMAGECRYPT_CRC32
#define IMAGECRYPT_CRC32

#ifdef __cplusplus
extern "C" {
#endif

uint32_t crc(unsigned char *buf, size_t len);

uint32_t update_crc(uint32_t crc, unsigned char *buf, size_t len);

#ifdef __cplusplus
}
#endif

#endif
