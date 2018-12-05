#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
//#include <netinet/in.h> // Alternative to <arpa/inet.h>. ifdef as appropriate
#include <arpa/inet.h>
#include "tools/status.h"
#include "safety.h"
#include "tools/transform_details.h"
#include "png.h"

png_chunk_spec[] png_chunk_specs =
{
	{ "IHDR", false }, // Basic image details
	{ "PLTE", false }, // Palette (list of colors)
	{ "IDAT", true  }, // Image
	{ "IEND", false }, // Marks image end
	{ "bKGD", false }, // Default background color
	{ "cHRM", true  }, // Objective color data
	{ "dSIG", true  }, // Digital signatures
	{ "eXIf", true  }, // Metadata
	{ "gAMA", false }, // Gama
	{ "hIST", true  }, // Histogram - frequency-of-occurence
	{ "iCCP", true  }, // Embedded ICC Profile
	{ "iTXt", true  }, // International Text Annotations
	{ "pHYs", false }, // Aspect ratio
	{ "sBIT", false }, // Sample depths
	{ "sPLT", false }, // Suggested reduced palette
	{ "sRGB", false }, // Standard color space
	{ "sTER", false }, // Indicator of stereo image
	{ "tEXt", true  }, // Uncompressed text annotations
	{ "tIME", true  }, // Time of last image modification
	{ "tRNS", true  }, // Simple transparency
	{ "zTXt", true  }, // Compressed text annotations
	NULL
};

png_chunk* create_png_chunk(png_chunk png_chunk)
{
	png_chunk *new_png_chunk = malloc_or_exit(sizeof(png_chunk *));
	new_png_chunk->name = strdup_or_exit(png_chunk.name);
	new_png_chunk->data_size = png_chunk.data_size;
	new_png_chunk->data = strdup_or_exit(png_chunk.data);
	new_png_chunk->crc32 = png_chunk.crc32;
	return new_png_chunk;
}

void destroy_png_chunk(png_chunk *png_chunk)
{
	free(png_chunk->name);
	free(png_chunk->data);
	free(png_chunk);
}

// file_to_read should not be null
// Will
status is_png(file_details *file_details)
{
	const char *signature = { 0x89, 0x50, 0xE4, 0x47, 0x0D, 0x0A, 0x1A, 0x0A };

	unsigned char buffer[sizeof(signature)];
	size_t chunks_read = fread(buffer, sizeof(char), sizeof(signature) - 1, file_details->file_path);
	// TODO: Move this to higher level
	if (!chunks_read)
	{
		stderr("Couldn't read [%s].", file_details->file_path);
		return ERROR;
	}

	status result = strcmp(buffer, signature) ? SUCCESS : NOT_SUCCESS;

	rewind(file_details->file);

	return result;
}

bool apply_chunk_cryptography(const char *name)
{
	int index = 0;
	png_chunk_spec current;
	while ((current = png_chunk_specs[index]))
	{
		if (!strcmp(name, current->name))
		{
			break;
		}
	}

	return (current ? current->cryptography_applied : true);
}

bool write_next_chunk(file_details *file_details, png_chunk *png_chunk)
{
	size_t chunks_read = fwrite(htonl(png_chunk->data_size), sizeof(png_chunk->data_size), 1, file_details->file);

	chunks_read = fwrite(png_chunk->name, sizeof(char), PNG_NAME_LENGTH - 1, file_details->file);

	chunks_read = fwrite(png_chunk->data, sizeof(png_chunk->data), png_chunk->data_size, file_details->file);

	chunks_read = fwrite(htonl(png_chunk->crc32), sizeof(png_chunk->crc32), 1, file_details->file);
	return true;
}

png_chunk* read_next_chunk(file_details *file_details)
{
	png_chunk png_chunk;

	uint8_t *buffer;
	uint32_t network_order_temp;

	size_t chunks_read = fread(buffer, sizeof(buffer), sizeof(network_order_temp), file_details->file);
	if (!chunks_read)
	{
		return NULL;
	}
	else if (chunks_read != sizeof(network_order_temp))
	{
		fprintf(stderr, "PNG | Bad size in [%s]", file_details->file_path);
	}

	network_order_temp = (uint32_t) buffer[0] << 24 |
	                     (uint32_t) buffer[1] << 16 |
	                     (uint32_t) buffer[2] << 8 |
	                     (uint32_t) buffer[3];
	png_chunk.data_size = ntohl(network_order_temp);

	chunks_read = fread(buffer, sizeof(char), PNG_NAME_LENGTH - 1, file_details->file);
	if (chunks_read != (sizeof(name) - 1))
	{
		fprintf(stderr, "PNG | Bad chunk name in [%s]", file_details->file_path);
	}

	png_chunk.name = buffer;

	chunks_read = fread(buffer, sizeof(char), png_chunk.data_size, file_details->file);
	if (chunks_read != png_chunk.data_size)
	{
		fprintf(stderr, "PNG | Bad data read in [%s]", file_details->file_details);
	}

	png_chunk.data = buffer;
	chunks_read = fread(buffer, sizeof(buffer), sizeof(network_order_temp), file_details->file);
	if (chunks_read != network_order_temp)
	{
		fprintf(stderr, "PNG | Bad CRC32 in [%s]", file_details->file_to_read);
	}

	network_order_temp = (uint32_t) buffer[0] << 24 |
	                     (uint32_t) buffer[1] << 16 |
	                     (uint32_t) buffer[2] << 8 |
	                     (uint32_t) buffer[3];
	png_chunk.crc32 = ntohl(network_order_temp);

	// Create this last so we can set data array length
	return create_png_chunk(png_chunk);
}

/*status encrypt_png(transform_details *details)
{
	printf("Encrypted PNG!");
	return true;
}

status decrypt_png(transform_details *details)
{
	printf("Decrypted PNG!");
	return true;
}*/
