#ifndef IMAGECRYPT_ENHANCED_STRING
#define IMAGECRYPT_ENHANCED_STRING

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
   char **char_pointer_array;
   unsigned int count;
   unsigned int size;
} enhanced_char_pointer_array;

enhanced_char_pointer_array* create_enhanced_char_pointer_array(int initial_size);

enhanced_char_pointer_array* create_enhanced_char_pointer_array_initial(char **initial);

void destroy_enhanced_char_pointer_array(enhanced_char_pointer_array *ecpa);

void append_enhanced_char_pointer_array(enhanced_char_pointer_array *enhanced_char_pointer, char *string);

bool char_pointer_starts_with_enhanced_char_pointer_array(char *string, enhanced_char_pointer_array *prefixes);

bool char_pointer_ends_with_enhanced_char_pointer_array(char *string, enhanced_char_pointer_array *suffixes);

#ifdef __cplusplus
}
#endif

#endif
