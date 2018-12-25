#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

// Table of CRCs of all 8-bit messages.
uint32_t crc_table[256];

// Flag: has the table been computed?
bool crc_table_computed = false;

// Make the table for a fast CRC
void make_crc_table()
{
	for (uint32_t n = 0; n < 256; n++)
	{
		uint32_t c = n;
		for (int k = 0; k < 8; k++)
		{
			if (c & 1)
			{
				c = 0xedb88320L ^ (c >> 1);
			}
			else
			{
				c = c >> 1;
			}
		}

		crc_table[n] = c;
	}

	crc_table_computed = true;
}

/* Update a running CRC with the bytes buf[0..len-1]--the CRC
   should be initialized to all 1's, and the transmitted value
   is the 1's complement of the final running CRC (see the
   crc() routine below)). */
uint32_t update_crc(uint32_t crc, unsigned char *buf, size_t len)
{
	if (!crc_table_computed)
	{
		make_crc_table();
	}

	uint32_t c = crc;
	for (size_t n = 0; n < len; n++)
	{
		c = crc_table[(c ^ buf[n]) & 0xff] ^ (c >> 8);
	}

	return c;
}

// Return the CRC of the bytes buf[0..len-1]
uint32_t crc(unsigned char *buf, size_t len)
{
	return update_crc(0xffffffffL, buf, len) ^ 0xffffffffL;
}
