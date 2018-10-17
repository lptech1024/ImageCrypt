#ifndef IMAGECRYPT_CRC
#define IMAGECRYPT_CRC

unsigned long update_crc(unsigned long crc, unsigned char *buf, int len);

unsigned long crc(unsigned char *buf, int len);

#endif
