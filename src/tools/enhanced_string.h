#ifndef IMAGECRYPT_ENHANCED_STRING
#define IMAGECRYPT_ENHANCED_STRING

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
   char **char_pointer_array;
   // TODO: Improve name
   int count;
   int size;
} enhanced_char_pointer_array;

void append_enhanced_char_pointer_array(enhanced_char_pointer_array *enhanced_char_pointer, char *string);

enhanced_char_pointer_array* create_enhanced_char_pointer_array(int initial_size);

enhanced_char_pointer_array* create_enhanced_char_pointer_array2(char **initial);

bool char_pointer_starts_with_enhanced_char_pointer_array(char *string, enhanced_char_pointer_array * prefixes)

#ifdef __cplusplus
}
#endif

#endif
