#ifndef IMAGECRYPT_SAFETY
#define IMAGECRYPT_SAFETY

#ifdef __cplusplus
extern "C" {
#endif

void* malloc_or_exit(size_t size);

char* strdup_or_exit(const char *s);

#ifdef __cplusplus
}
#endif

#endif
