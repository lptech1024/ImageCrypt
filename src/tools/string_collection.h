#ifndef IMAGECRYPT_STRING_COLLECTION
#define IMAGECRYPT_STRING_COLLECTION

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
   char **strings;
   unsigned int count;
   unsigned int size;
} string_collection;

string_collection* create_string_collection(int initial_size);

string_collection* create_string_collection_initial(char **initial);

void destroy_string_collection(string_collection *ecpa);

void append_string_collection(string_collection *enhanced_char_pointer, char *string);

bool char_pointer_starts_with_string_collection(char *string, string_collection *prefixes);

bool char_pointer_ends_with_string_collection(char *string, string_collection *suffixes);

#ifdef __cplusplus
}
#endif

#endif
