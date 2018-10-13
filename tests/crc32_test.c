/*
 * gcc "../src/third_party/libpng/crc32.c" crc32_test.c -o crc32_test
 * 
 * TODO: Use a make file and use the $/obj directory
 * TODO: What's the Clang equivalent?
 */

#include <stdio.h> // puts, printf
// TODO: #include "../src/third_party/libpng/crc32.h" to fix the undefined function warning (GCC)

// TODO: Determine whether to create here or in main
union {
	unsigned char crc32_chars[4];
	unsigned long crc32_value;
} crc32_union;

int main()
{
	// TODO: Don't use magic values here
	unsigned char sample_data[] = { 0x74, 0x49, 0x4D, 0x45, 0x07, 0xE2, 0x0A, 0x07, 0x04, 0x27, 0x1B, 0x00 };

	unsigned long calculated_crc32 = crc(sample_data, sizeof(sample_data) - 1);

	// TODO: Don't use magic values here
	// NB: This is in big-endian ordering
	unsigned char expected_crc32[] = { 0x17, 0x96, 0xE3, 0x64 };

	// Populate the char array expecting little-endian. We may be able to futher utilize the union to avoid endian issues.
	crc32_union.crc32_chars[0] = expected_crc32[3];
	crc32_union.crc32_chars[1] = expected_crc32[2];
	crc32_union.crc32_chars[2] = expected_crc32[1];
	crc32_union.crc32_chars[3] = expected_crc32[0];

	// TODO: Make this work for big-endian if we aren't going to utilize the union
	//unsigned long expected_crc32_long = expected_crc32[0] | (expected_crc32[1] << 8) | (expected_crc32[2] << 16) | (expected_crc32[3] << 24);

	if (calculated_crc32 != crc32_union.crc32_value)
	{
		printf("crc32 [%lu]. expected_crc32_long [%lu].\n", calculated_crc32, crc32_union.crc32_value);
	}
	else
	{
		puts("Good");
	}

	return 0;
}
