#NB: gcc should also work
CC=clang
MAKE=make

ROOT=..

LIB_MATH=-lm
LIB_CRYPTO=-lcrypto
ALL_LIBS=$(LIB_MATH) $(LIB_CRYPTO)

.PHONY: all source source_clean clean clean_full

all: cryptography_test fpe_test crc32_test

source: 
	$(MAKE) -C $(ROOT) -f Makefile

source_clean:
	$(MAKE) -C $(ROOT) -f Makefile clean

cryptography_test: source $(ROOT)/obj/cryptography.o cryptography_test.c
	$(CC) $(ROOT)/obj/cryptography.o cryptography_test.c $(LIB_CRYPTO) -o $@

fpe_test: source $(ROOT)/obj/fpe.o fpe_test.c
	$(CC) $(ROOT)/obj/fpe.o fpe_test.c $(ALL_LIBS) -o $@

crc32_test: source $(ROOT)/obj/crc32.o crc32_test.c
	$(CC) $(ROOT)/obj/crc32.o crc32_test.c -o $@

clean:
	-rm cryptography_test fpe_test crc32_test

clean_full: clean source_clean
