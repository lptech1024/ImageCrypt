#ifndef IMAGECRYPT_STATUS
#define IMAGECRYPT_STATUS

#ifdef __cplusplus
extern "C" {
#endif

// TODO: Add more specific errors
typedef enum {
	RETURN_STATUS_ERROR = -1,
	RETURN_STATUS_NOT_SUCCESS = 0,
	RETURN_STATUS_SUCCESS = 1,
} return_status;

#ifdef __cplusplus
}
#endif

#endif
