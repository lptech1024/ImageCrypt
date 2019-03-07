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

const char PNG_SIGNATURE[] = { 0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A, 0x00 };

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

static png_chunk* create_png_chunk(uint32_t data_size)
{
	png_chunk *new_png_chunk = malloc_or_exit(sizeof(*new_png_chunk));
	new_png_chunk->data_size = data_size;
	new_png_chunk->crc32 = 0;
	new_png_chunk->name = malloc_or_exit(PNG_NAME_LENGTH - 1);
	new_png_chunk->data = malloc(data_size);
	return new_png_chunk;
}

static void destroy_png_chunk(png_chunk *png_chunk)
{
	free((void *) png_chunk->name);
	free((void *) png_chunk->data);
	free(png_chunk);
}

static return_status read_signature(FILE *file)
{
	//printf("read_signature\n");
	size_t signature_length = strlen(PNG_SIGNATURE);

	char buffer[signature_length];
	//printf("\trs fread\n");
	size_t bytes_read = fread(buffer, 1, signature_length, file);
	//printf("\trs fread complete\n");
	if (bytes_read != signature_length)
	{
		//printf("\trs ERROR\n");
		return RETURN_STATUS_ERROR;
	}
	else
	{
		for (int i = 0; i < signature_length; i++)
		{
			if (PNG_SIGNATURE[i] != buffer[i])
			{
				//printf("\trs NOT_SUCCESS\n");
				return RETURN_STATUS_NOT_SUCCESS;
			}
		}

		//printf("\trs SUCCESS\n");
		return RETURN_STATUS_SUCCESS;
	}
}

static return_status write_signature(FILE *file)
{
	fwrite(PNG_SIGNATURE, 1, strlen(PNG_SIGNATURE), file);
	return RETURN_STATUS_SUCCESS;
}

return_status is_png(file_details *file_details)
{
	//printf("is_png\n");
	return_status signature_read_status = read_signature(file_details->file);

	//printf("\trewind complete\n");
	rewind(file_details->file);
	//printf("\trewind complete\n");

	//printf("is_png complete\n");
	return signature_read_status;
}

static bool apply_chunk_cryptography(const char *name)
{
	size_t index = 0;
	png_chunk_spec *match = NULL;
	for (const png_chunk_spec *current = &png_chunk_specs[index]; current->name; current = &png_chunk_specs[++index])
	{
		if (!strncmp(name, current->name, PNG_NAME_LENGTH - 1))
		{
			match = (png_chunk_spec *) current;
			break;
		}
	}

	return (match ? match->cryptography_applies : true);
}

static bool write_next_chunk(file_details *file_details, png_chunk *png_chunk)
{
	//printf("write_next_chunk\n");
	uint32_t htonl_result = htonl(png_chunk->data_size);
	size_t chunks_read = fwrite(&htonl_result, 1, 4, file_details->file);

	chunks_read = fwrite(png_chunk->name, 1, PNG_NAME_LENGTH - 1, file_details->file);

	chunks_read = fwrite(png_chunk->data, 1, png_chunk->data_size, file_details->file);

	htonl_result = htonl(png_chunk->crc32);
	chunks_read = fwrite(&htonl_result, 1, 4, file_details->file);
	//printf("write_next_chunk complete\n");
	return true;
}

static png_chunk* read_next_chunk(file_details *file_details)
{
	//printf("read_next_chunk\n");

	size_t buffer_size = sizeof(unsigned char) * 4;
	unsigned char *buffer = malloc_or_exit(8192);
	uint32_t network_order_temp;

	size_t chunks_read = fread(buffer, 1, buffer_size, file_details->file);
	if (!chunks_read)
	{
		//printf("\t!chunks_read\n");
		free(buffer);
		return NULL;
	}
	else if (chunks_read != sizeof(network_order_temp))
	{
		fprintf(stderr, "\tPNG | Bad size in [%s]\n", file_details->file_path);
		exit(-1);
	}

	network_order_temp = /*(uint32_t)*/ buffer[0] << 24 |
	                     /*(uint32_t)*/ buffer[1] << 16 |
	                     /*(uint32_t)*/ buffer[2] << 8 |
	                     /*(uint32_t)*/ buffer[3];
	//printf("buffer[0] is [%" PRIu8 "]\n", buffer[0]);
	//printf("buffer[1] is [%" PRIu8 "]\n", buffer[1]);
	//printf("buffer[2] is [%" PRIu8 "]\n", buffer[2]);
	//printf("buffer[3] is [%" PRIu8 "]\n", buffer[3]);
	png_chunk *png_chunk = create_png_chunk(network_order_temp);
	//png_chunk.data_size = ntohl(network_order_temp);
	//printf("!ntohl is [%" PRIu32 "]\n", png_chunk->data_size);
	//printf("ntohl is [%" PRIu32 "]\n", ntohl(network_order_temp));

	chunks_read = fread(png_chunk->name, 1, PNG_NAME_LENGTH - 1, file_details->file);
	if (chunks_read != (PNG_NAME_LENGTH - 1))
	{
		fprintf(stderr, "\tPNG | Bad chunk name in [%s]\n", file_details->file_path);
	}

	if ((png_chunk->data_size) > buffer_size)
	{
		buffer_size = png_chunk->data_size;
		//printf("\trealloc to buffer_size [%zu]\n", buffer_size);
		buffer = realloc(buffer, sizeof(buffer) * buffer_size);
	}

	chunks_read = fread(buffer, 1, png_chunk->data_size, file_details->file);
	if (chunks_read != png_chunk->data_size)
	{
		fprintf(stderr, "\tPNG | Bad data read in [%s]\n", file_details->file_path);
	}

	memcpy(png_chunk->data, buffer, png_chunk->data_size);
	chunks_read = fread(buffer, 1, sizeof(network_order_temp), file_details->file);
	if (chunks_read != sizeof(network_order_temp))
	{
		fprintf(stderr, "\tPNG | Bad CRC32 in [%s]\n", file_details->file_path);
	}

	network_order_temp = /*(uint32_t)*/ buffer[0] << 24 |
	                     /*(uint32_t)*/ buffer[1] << 16 |
	                     /*(uint32_t)*/ buffer[2] << 8 |
	                     /*(uint32_t)*/ buffer[3];
	//png_chunk->crc32 = ntohl(network_order_temp);
	png_chunk->crc32 = network_order_temp;

	free(buffer);
	// Create this last so we can set data array length
	//printf("\tread_next_chunk complete\n");
	return png_chunk;
}

static void hex_to_ints(unsigned char *hex, unsigned int hex_length, unsigned int *result)
{
	for (int i = 0; i < hex_length; i++)
	{
		//printf("hex[%i] is [%x]\n", i, hex[i]);
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

//union {
//	unsigned char crc32_chars[4];
//	uint32_t crc32_value;
//} crc32_union;

static bool convert_chunk(png_chunk *png_chunk, FPE_KEY *fpe_key, cryptography_mode cryptography_mode)
{
	//printf("convert_chunk\n");
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

	//printf("\tpng_chunk->data_size [%" PRIu32 "]\n", png_chunk->data_size);
	unsigned int *crypt_data = malloc_or_exit(sizeof(*crypt_data) * png_chunk->data_size);
	unsigned int *crypt_data2 = malloc_or_exit(sizeof(*crypt_data2) * png_chunk->data_size);
	hex_to_ints(png_chunk->data, png_chunk->data_size, crypt_data);
	//printf("\tfpe_ff1_encrypt\n");
	//printf("\tcrypt is [%i]\n", crypt);
	if (fpe_key == NULL)
	{
		//printf("\tfpe_key is NULL\t");
		exit(-1);
	}

	fpe_ff1_encrypt(crypt_data, crypt_data2, png_chunk->data_size, fpe_key, crypt);
	//printf("\tfpe_ff1_encrypt complete\n");
	ints_to_hex(crypt_data2, png_chunk->data_size, png_chunk->data);
	free(crypt_data);
	free(crypt_data2);

	//printf("\tcrc32\n");
	//printf("png_chunk->data_size is [%" PRIu32 "]\n", png_chunk->data_size);
	//crc32_union.crc32_value = png_chunk->crc32;
	//printf("crc32 (c) is [%02X]\n", crc32_union.crc32_chars[0]);
	//printf("crc32 (c) is [%02X]\n", crc32_union.crc32_chars[1]);
	//printf("crc32 (c) is [%02X]\n", crc32_union.crc32_chars[2]);
	//printf("crc32 (c) is [%02X]\n", crc32_union.crc32_chars[3]);
	unsigned char full_data[4 + png_chunk->data_size];
	memcpy(full_data, png_chunk->name, 4);
	memcpy(full_data + 4, png_chunk->data, png_chunk->data_size);
	uint32_t crc32 = crc(full_data, 4 + png_chunk->data_size);
	//crc32 = update_crc(crc32, png_chunk->data, png_chunk->data_size);
	//crc32_union.crc32_value = crc32_2;
	//crc32_union.crc32_value = crc32;
	png_chunk->crc32 = crc32;
	//png_chunk->crc32 = htonl(png_chunk->crc32);
	//png_chunk->crc32 = ntohl(png_chunk->crc32);
	//printf("crc32 is [%02X]\n", crc32_union.crc32_chars[0]);
	//printf("crc32 is [%02X]\n", crc32_union.crc32_chars[1]);
	//printf("crc32 is [%02X]\n", crc32_union.crc32_chars[2]);
	//printf("crc32 is [%02X]\n", crc32_union.crc32_chars[3]);
	//printf("convert_chunk complete\n");
	return true;
}

return_status convert_png(transform_details *details, FPE_KEY *fpe_key, cryptography_mode cryptography_mode)
{
	//printf("convert_png start\n");
	FILE *file = fopen(details->input->file_path, "r");
	if (read_signature(file) != RETURN_STATUS_SUCCESS)
	{
		//printf("\tread_signature ! SUCCESS\n");
		return RETURN_STATUS_ERROR;
	}

	details->input->file = file;
	FILE *output_file = fopen(details->output->file_path, "w");
	details->output->file = output_file;

	write_signature(details->output->file);

	png_chunk *png_chunk = NULL;
	//printf("\twhile loop prepped\n");
	while ((png_chunk = read_next_chunk(details->input)))
	{
		//printf("\twhile loop\n");
		if (apply_chunk_cryptography(png_chunk->name))
		{
			convert_chunk(png_chunk, fpe_key, cryptography_mode);
		}

		write_next_chunk(details->output, png_chunk);
		destroy_png_chunk(png_chunk);
	}

	fclose(output_file);
	//printf("convert_png end\n");

	return true;
}
