#ifndef IMAGECRYPT_SAFETY
#define IMAGECRYPT_SAFETY

#ifdef __cplusplus
extern "C" {
#endif

void* malloc_or_exit(size_t size);

#ifdef __cplusplus
}
#endif

#endif
