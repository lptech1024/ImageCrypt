#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
//#include <netinet/in.h> // Alternative to <arpa/inet.h>. ifdef as appropriate
#include <arpa/inet.h>
#include "tools/crc32.h"
#include "tools/cryptography.h"
#include "tools/status.h"
#include "tools/safety.h"
#include "tools/transform_details.h"
#include "png.h"

const png_chunk_spec[] png_chunk_specs =
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

size_t read_signature(FILE *file)
{
	const char *signature = { 0x89, 0x50, 0xE4, 0x47, 0x0D, 0x0A, 0x1A, 0x0A };

	unsigned char buffer[sizeof(signature)];
	return fread(buffer, sizeof(buffer), strlen(signature), file_details->file_path);
}

status is_png(file_details *file_details)
{
	size_t chunks_read = read_signature(file_details->file);
	if (!chunks_read)
	{
		stderr("Couldn't read [%s].", file_details->file_path);
		return ERROR;
	}

	rewind(file_details->file);

	return strcmp(buffer, signature) ? SUCCESS : NOT_SUCCESS;
}

bool apply_chunk_cryptography(const char *name)
{
	size_t index = 0;
	png_chunk_spec current = NULL;
	while ((current = png_chunk_specs[index++]))
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

	chunks_read = fwrite(png_chunk->name, sizeof(png_chunk->name), PNG_NAME_LENGTH - 1, file_details->file);

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

	chunks_read = fread(buffer, sizeof(buffer), PNG_NAME_LENGTH - 1, file_details->file);
	if (chunks_read != (PNG_NAME_LENGTH - 1))
	{
		fprintf(stderr, "PNG | Bad chunk name in [%s]", file_details->file_path);
	}

	png_chunk.name = buffer;

	chunks_read = fread(buffer, sizeof(buffer), png_chunk.data_size, file_details->file);
	if (chunks_read != png_chunk.data_size)
	{
		fprintf(stderr, "PNG | Bad data read in [%s]", file_details->file_path);
	}

	png_chunk.data = buffer;
	chunks_read = fread(buffer, sizeof(buffer), sizeof(network_order_temp), file_details->file);
	if (chunks_read != sizeof(network_order_temp))
	{
		fprintf(stderr, "PNG | Bad CRC32 in [%s]", file_details->file_path);
	}

	network_order_temp = (uint32_t) buffer[0] << 24 |
	                     (uint32_t) buffer[1] << 16 |
	                     (uint32_t) buffer[2] << 8 |
	                     (uint32_t) buffer[3];
	png_chunk.crc32 = ntohl(network_order_temp);

	// Create this last so we can set data array length
	return create_png_chunk(png_chunk);
}

void hex_to_ints(unsigned char *hex, unsigned int hex_length, unsigned int *result)
{
	for (int i = 0; i < hex_length; i++)
	{
		result[i] = hex[i];
	}
}


bool convert_chunk(png_chunk *png_chunk, FPE_KEY *fpe_key, cryptography_mode cryptography_mode)
{
	const int crypt = -1;
	switch (cryptography_mode)
	{
		case ENCRYPT:
			crypt = FPE_ENCRYPT;
			break;
		case DECRYPT:
			crypt = FPE_DECRYPT;
			break;
		default:
			return false;
	}

	unsigned int crypt_data[png_chunk->data_size];
	hex_to_ints(png_chunk->data, png_chunk->data_size, crypt_data);
	fpe_ff1_encrypt(png_chunk->data, png_chunk->data, png_chunk->data_size, &fpe_key, crypt);

	unsigned long crc32 = crc(png_chunk->name, strlen(png_chunk->name));
	update_crc(crc32, png_chunk->data, png_chunk->data_size);
	png_chunk->crc32 = crc32;

	return true;
}

status convert_png(transform_details *details, FPE_KEY *fpe_key, cryptography_mode cryptography_mode)
{
	if (!read_signature(details->input))
	{
		return ERROR;
	}

	png_chunk *png_chunk = NULL;
	while (png_chunk = read_next_chunk(details->input))
	{
		if (apply_chunk_cryptography(png_chunk->name))
		{
			convert_chunk(png_chunk, fpe_key, cryptography_mode);
		}

		write_next_chunk(png_chunk, details->output);
	}

	// TODO: Check for details->input->file_path EOF

	return true;
}
