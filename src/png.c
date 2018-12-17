#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
//#include <netinet/in.h> // Alternative to <arpa/inet.h>. ifdef as appropriate
#include <arpa/inet.h>
#include "tools/fpe.h"
#include "tools/crc32.h"
#include "tools/cryptography.h"
#include "tools/status.h"
#include "tools/safety.h"
#include "tools/transform_details.h"
#include "png.h"

const png_chunk_spec png_chunk_specs[] =
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
	{ NULL,   true }
};

png_chunk* create_png_chunk(png_chunk chunk)
{
	png_chunk *new_png_chunk = malloc_or_exit(sizeof(png_chunk *));
	new_png_chunk->name = strdup_or_exit(chunk.name);
	new_png_chunk->data_size = chunk.data_size;
	new_png_chunk->data = (unsigned char *) strdup_or_exit((const char *) chunk.data);
	new_png_chunk->crc32 = chunk.crc32;
	return new_png_chunk;
}

void destroy_png_chunk(png_chunk *png_chunk)
{
	free((void *) png_chunk->name);
	free((void *) png_chunk->data);
	free(png_chunk);
}

status read_signature(FILE *file)
{
	const char signature[] = { 0x89, 0x50, 0xE4, 0x47, 0x0D, 0x0A, 0x1A, 0x0A };
	const size_t signature_length = strlen(signature);

	char buffer[signature_length + 1];
	size_t bytes_read = fread(buffer, sizeof(buffer), signature_length, file);
	if (bytes_read != signature_length)
	{
		return ERROR;
	}
	else
	{
		return (strncmp(buffer, signature, signature_length) ? SUCCESS : NOT_SUCCESS);
	}
}

status is_png(file_details *file_details)
{
	status signature_read_status = read_signature(file_details->file);

	rewind(file_details->file);

	return signature_read_status;
}

bool apply_chunk_cryptography(const char *name)
{
	size_t index = 0;
	png_chunk_spec *match = NULL;
	for (const png_chunk_spec *current = &png_chunk_specs[index]; current->name; current = &png_chunk_specs[++index])
	{
		if (!strcmp(name, current->name))
		{
			match = (png_chunk_spec *) current;
			break;
		}
	}

	return (match ? match->cryptography_applies : true);
}

bool write_next_chunk(file_details *file_details, png_chunk *png_chunk)
{
	uint32_t *htonl_result = malloc_or_exit(sizeof(uint32_t *));
	*htonl_result = htonl(png_chunk->data_size);
	size_t chunks_read = fwrite(htonl_result, sizeof(png_chunk->data_size), 1, file_details->file);

	chunks_read = fwrite(png_chunk->name, sizeof(png_chunk->name), PNG_NAME_LENGTH - 1, file_details->file);

	chunks_read = fwrite(png_chunk->data, sizeof(png_chunk->data), png_chunk->data_size, file_details->file);

	*htonl_result = htonl(png_chunk->crc32);
	chunks_read = fwrite(htonl_result, sizeof(png_chunk->crc32), 1, file_details->file);
	return true;
}

png_chunk* read_next_chunk(file_details *file_details)
{
	png_chunk png_chunk;

	size_t buffer_size = sizeof(uint8_t) * 4;
	uint8_t *buffer = malloc_or_exit(buffer_size);
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

	png_chunk.name = (char *) buffer;

	if (png_chunk.data_size > buffer_size)
	{
		buffer_size = png_chunk.data_size;
		buffer = realloc(buffer, buffer_size);
	}
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

void ints_to_hex(unsigned int *ints, size_t int_length, unsigned char *result)
{
	for (int i = 0; i < int_length; i++)
	{
		result[i] = ints[i];
	}
}

bool convert_chunk(png_chunk *png_chunk, FPE_KEY *fpe_key, cryptography_mode cryptography_mode)
{
	int crypt = -1;
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
	fpe_ff1_encrypt(crypt_data, crypt_data, png_chunk->data_size, fpe_key, crypt);
	ints_to_hex(crypt_data, png_chunk->data_size, png_chunk->data);

	unsigned long crc32 = crc((unsigned char *) png_chunk->name, strlen(png_chunk->name));
	update_crc(crc32, png_chunk->data, png_chunk->data_size);
	png_chunk->crc32 = crc32;

	return true;
}

status convert_png(transform_details *details, FPE_KEY *fpe_key, cryptography_mode cryptography_mode)
{
	if (read_signature(details->input->file) != SUCCESS)
	{
		return ERROR;
	}

	bool first_write = true;

	png_chunk *png_chunk = NULL;
	while ((png_chunk = read_next_chunk(details->input)))
	{
		if (apply_chunk_cryptography(png_chunk->name))
		{
			convert_chunk(png_chunk, fpe_key, cryptography_mode);
		}

		if (first_write)
		{
			first_write = false;
			printf("Would write to [%s]\n", details->output->file_path);
		}
		//write_next_chunk(details->output, png_chunk);
	}

	printf("Writing complete\n");
	// TODO: Check for details->input->file_path EOF

	return true;
}
