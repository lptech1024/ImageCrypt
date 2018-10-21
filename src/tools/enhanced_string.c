#include "enhanced_string.h"

// NB: string is const
void append_enhanced_char_pointer_array(enhanced_char_pointer_array *enhanced_char_pointer, char *string)
{
   // Change size to allow for one more char *
   // TODO: What if size is already large enough?
   if ((enhanced_char_pointer->size) < (enhanced_char_pointer->count + 1))
   {   
      enhanced_char_pointer->char_pointer_array = realloc(enhanced_char_pointer->char_pointer_array, (enhanced_char_pointer->count + 1) * sizeof(char *));
      enhanced_char_pointer->size++;
   }      

   enhanced_char_pointer->char_pointer_array[enhanced_char_pointer->count++] = strdup(string);
}

enhanced_char_pointer_array* create_enhanced_char_pointer_array(int initial_size)
{
   enhanced_char_pointer_array *ecpa = malloc(sizeof(enhanced_char_pointer_array));
   ecpa->count = initial_size;
   ecpa->size = initial_size;
   return ecpa;
}

enhanced_char_pointer_array* create_enhanced_char_pointer_array2(char **initial)
{
   int initial_count = 0;
   for (int i = 0; initial[i]; i++)
   {   
      initial_count++;
   }      

   enhanced_char_pointer_array *ecpa = create_enhanced_char_pointer_array(initial_count);
    
   for (int i = 0; initial[i]; i++)
   {   
      append_enhanced_char_pointer_array(ecpa, initial[i]);
   }      

   return ecpa;
}

bool char_pointer_starts_with_enhanced_char_pointer_array(char *string, enhanced_char_pointer_array * prefixes)
{
   bool starts_with_prefix = false;

   const int string_null_index = strlen(string);

   for (int i = 0; i < prefixes->count; i++)
   {
      const int prefix_null_index = strlen(prefixes->char_pointer_array[i]);

      if (prefix_null_index < string_null_index)
      {
         continue;
      }

      for (int j = 0; j < prefix_null_index; j++)
      {
         if (string[j] != prefixes->char_pointer_array[i][j])
         {
            j = prefix_null_index;
            continue;
         }
         else if ((j + 1) == prefix_null_index)
         {
            starts_with_prefix = true;
         }
      }
   }

   return starts_with_prefix;
}
