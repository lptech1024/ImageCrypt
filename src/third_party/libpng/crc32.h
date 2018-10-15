#ifndef HEADER_CRC
#define HEADER_CRC

unsigned long update_crc(unsigned long crc, unsigned char *buf, int len);

unsigned long crc(unsigned char *buf, int len);

#endif
