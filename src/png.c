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
#include "tools/transform_details.h"
#include "png.h"

const char unsigned PNG_SIGNATURE[] = { 0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A, 0x00 };

// Byte = 8 bits
#define UINT32_T_SHIFT_THREE_BYTES 24
#define UINT32_T_SHIFT_TWO_BYTES 16
#define UINT32_T_SHIFT_ONE_BYTE 8

#define BYTES_DATA 4
#define BYTES_CHUNK_TYPE 4
#define BYTES_CRC32 4

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
	{ NULL,   true }   // Encrypt unspecified chunk types
};

static png_chunk* create_png_chunk(uint32_t data_size)
{
	// TODO: Handle error
	png_chunk *new_png_chunk = malloc(sizeof(*new_png_chunk));
	new_png_chunk->data_size = data_size;
	new_png_chunk->crc32 = 0;
	// TODO: Handle error
	new_png_chunk->name = malloc(BYTES_CHUNK_TYPE);
	// TODO: Handle error
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
	size_t signature_length = strlen((const char *) PNG_SIGNATURE);
	//printf("\trs signature_length is %lu\n", signature_length);

	unsigned char buffer[signature_length];
	//printf("\trs fread\n");

	// TODO: Handle error
	size_t bytes_read = fread(buffer, 1, signature_length, file);
	//printf("\trs fread complete\n");
	if (bytes_read != signature_length)
	{
		//printf("\trs ERROR\n");
		return RETURN_STATUS_ERROR;
	}

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

static return_status write_signature(FILE *file)
{
	fwrite(PNG_SIGNATURE, 1, strlen((const char *) PNG_SIGNATURE), file);
	return RETURN_STATUS_SUCCESS;
}

return_status is_png(file_details *file_details)
{
	//printf("is_png\n");
	// TODO: Handle errors
	return_status signature_read_status = read_signature(file_details->file);

	//printf("\trewind started\n");
	// TODO: Handle -1
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
		if (!strncmp(name, current->name, BYTES_CHUNK_TYPE))
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
	// TODO: Handle potential error
	fwrite(&htonl_result, 1, BYTES_DATA, file_details->file);
	// TODO: Handle potential error
	fwrite(png_chunk->name, 1, BYTES_DATA, file_details->file);
	// TODO: Handle potential error
	fwrite(png_chunk->data, 1, png_chunk->data_size, file_details->file);

	htonl_result = htonl(png_chunk->crc32);
	// TODO: Handle potential error
	fwrite(&htonl_result, 1, BYTES_CRC32, file_details->file);
	//printf("write_next_chunk complete\n");
	return true;
}

static png_chunk* read_next_chunk(file_details *file_details)
{
	//printf("read_next_chunk\n");

	size_t buffer_size = sizeof(unsigned char) * BYTES_DATA;
	// TODO: Handle failure
	unsigned char *buffer = malloc(buffer_size);

	size_t chunks_read = fread(buffer, 1, buffer_size, file_details->file);
	if (!chunks_read)
	{
		//printf("\t!chunks_read\n");
		free(buffer);
		return NULL;
	}

	if (chunks_read != BYTES_DATA)
	{
		fprintf(stderr, "\tPNG | Bad size in [%s]\n", file_details->file_path);
		exit(-1);
	}

	uint32_t host_order_buffer = buffer[0] << UINT32_T_SHIFT_THREE_BYTES |
	                             buffer[1] << UINT32_T_SHIFT_TWO_BYTES |
	                             buffer[2] << UINT32_T_SHIFT_ONE_BYTE |
	                             buffer[3];

	// TODO: Handle failure
	png_chunk *png_chunk = create_png_chunk(host_order_buffer);

	chunks_read = fread(png_chunk->name, 1, BYTES_CHUNK_TYPE, file_details->file);
	if (chunks_read != BYTES_CHUNK_TYPE)
	{
		fprintf(stderr, "\tPNG | Bad chunk name in [%s]\n", file_details->file_path);
	}

	if ((png_chunk->data_size) > buffer_size)
	{
		buffer_size = png_chunk->data_size;
		//printf("\trealloc to buffer_size [%zu]\n", buffer_size);
		// TODO: Handle errors
		buffer = realloc(buffer, buffer_size);
	}

	chunks_read = fread(buffer, 1, png_chunk->data_size, file_details->file);
	if (chunks_read != png_chunk->data_size)
	{
		fprintf(stderr, "\tPNG | Bad data read in [%s]. Bytes read is [%lu], but data size is [%d]\n", file_details->file_path, chunks_read, png_chunk->data_size);
	}

	// TODO: Consider dropping usage of memcpy
	// TODO: Handle errors
	memcpy(png_chunk->data, buffer, png_chunk->data_size);
	chunks_read = fread(buffer, 1, BYTES_CRC32, file_details->file);
	if (chunks_read != BYTES_CRC32)
	{
		fprintf(stderr, "\tPNG | Bad CRC32 in [%s]\n", file_details->file_path);
	}

	host_order_buffer =  buffer[0] << UINT32_T_SHIFT_THREE_BYTES |
	                     buffer[1] << UINT32_T_SHIFT_TWO_BYTES |
	                     buffer[2] << UINT32_T_SHIFT_ONE_BYTE |
	                     buffer[3];
	png_chunk->crc32 = host_order_buffer;

	free(buffer);

	//printf("\tread_next_chunk complete\n");
	return png_chunk;
}

static void hex_to_ints(const unsigned char *hex, const unsigned int hex_length, unsigned int *result)
{
	for (int i = 0; i < hex_length; i++)
	{
		//printf("hex[%i] is [%x]\n", i, hex[i]);
		result[i] = hex[i];
	}
}

void ints_to_hex(const unsigned int *ints, const size_t int_length, unsigned char *result)
{
	for (int i = 0; i < int_length; i++)
	{
		result[i] = ints[i];
	}
}

static bool convert_chunk(png_chunk *png_chunk, FPE_KEY *fpe_key, cryptography_mode cryptography_mode)
{
	//printf("convert_chunk\n");
	int crypt;
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
	// TODO: Handle errors
	unsigned int *crypt_data = malloc(sizeof(*crypt_data) * png_chunk->data_size);
	// TODO: Handle errors
	unsigned int *crypt_data2 = malloc(sizeof(*crypt_data2) * png_chunk->data_size);
	hex_to_ints(png_chunk->data, png_chunk->data_size, crypt_data);
	//printf("\tfpe_ff1_encrypt\n");
	//printf("\tcrypt is [%i]\n", crypt);
	if (fpe_key == NULL)
	{
		//printf("\tfpe_key is NULL\t");
		exit(-1);
	}

	// TODO: Handle errors
	fpe_ff1_encrypt(crypt_data, crypt_data2, png_chunk->data_size, fpe_key, crypt);
	//printf("\tfpe_ff1_encrypt complete\n");
	ints_to_hex(crypt_data2, png_chunk->data_size, png_chunk->data);
	free(crypt_data);
	free(crypt_data2);

	unsigned char full_data[BYTES_DATA + png_chunk->data_size];
	// TODO: Handle errors
	// TODO: Consider dropping use of memcpy
	memcpy(full_data, png_chunk->name, BYTES_DATA);
	// TODO: Handle errors
	// TODO: Consider dropping use of memcpy
	memcpy(full_data + BYTES_DATA, png_chunk->data, png_chunk->data_size);

	png_chunk->crc32 = crc(full_data, BYTES_DATA + png_chunk->data_size);

	//printf("convert_chunk complete\n");
	return true;
}

return_status convert_png(transform_details *details, FPE_KEY *fpe_key, cryptography_mode cryptography_mode)
{
	//printf("convert_png start\n");
	// TODO: Handle errors
	FILE *file = fopen(details->input->file_path, "r");
	if (read_signature(file) != RETURN_STATUS_SUCCESS)
	{
		//printf("\tread_signature ! SUCCESS\n");
		return RETURN_STATUS_ERROR;
	}

	details->input->file = file;
	// TODO: Handle errors
	FILE *output_file = fopen(details->output->file_path, "w");
	details->output->file = output_file;

	// TODO: Handle errors
	write_signature(details->output->file);

	png_chunk *png_chunk = NULL;
	//printf("\twhile loop prepped\n");
	// TODO: Handle errors
	while ((png_chunk = read_next_chunk(details->input)))
	{
		//printf("\twhile loop\n");
		// TODO: Handle errors
		if (apply_chunk_cryptography(png_chunk->name))
		{
			// TODO: Handle errors
			convert_chunk(png_chunk, fpe_key, cryptography_mode);
		}

		// TODO: Handle errors
		write_next_chunk(details->output, png_chunk);
		destroy_png_chunk(png_chunk);
	}

	// TODO: Note errors for debugging
	fclose(output_file);
	//printf("convert_png end\n");

	return true;
}
