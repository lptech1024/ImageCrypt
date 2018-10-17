#NB: gcc should also work
CC=clang
CFLAGS=-Wall

CCCFLAGS=$(CC) $(CFLAGS)

ODIR=obj
BDIR=bin
TDIR=tests
SOURCE=src

LIB_MATH=-lm
LIB_CRYPTO=-lcrypto
ALL_LIBS=$(LIB_MATH) $(LIB_CRYPTO)

THIRD_PARTY=$(SOURCE)/third_party

.PHONY: all clean

all: $(ODIR)/cryptography.o $(ODIR)/fpe.o $(ODIR)/crc32.o
#image_crypt

$(ODIR)/cryptography.o: $(SOURCE)/cryptography.h $(SOURCE)/cryptography.c
	$(CCCFLAGS) -c $(SOURCE)/cryptography.c -o $@

$(ODIR)/cryptography_test.o: $(SOURCE)/cryptography.h $(TDIR)/cryptography_test.c
	$(CCCFLAGS) -c $(TDIR)/cryptography_test.c -o $@

$(BDIR)/cryptography_test: $(ODIR)/cryptography.o $(ODIR)/cryptography_test.o
	$(CCCFLAGS) $(ODIR)/cryptography.o $(ODIR)/cryptography_test.o $(LIB_CRYPTO) -o $@

$(ODIR)/fpe.o: $(THIRD_PARTY)/format_preserving_encryption/fpe.h $(THIRD_PARTY)/format_preserving_encryption/fpe.c
	$(CCCFLAGS) -c $(THIRD_PARTY)/format_preserving_encryption/fpe.c -o $@

$(ODIR)/fpe_test.o: $(THIRD_PARTY)/format_preserving_encryption/fpe.h $(TDIR)/fpe_test.c
	$(CCCFLAGS) -c $(TDIR)/fpe_test.c -o $@

$(BDIR)/fpe_test: $(ODIR)/fpe.o $(ODIR)/fpe_test.o
	$(CCCFLAGS) $(ODIR)/fpe.o $(ODIR)/fpe_test.o $(ALL_LIBS) -o $@

$(ODIR)/crc32.o: $(THIRD_PARTY)/libpng/crc32.h $(THIRD_PARTY)/libpng/crc32.c
	$(CCCFLAGS) -c $(THIRD_PARTY)/libpng/crc32.c -o $@

$(ODIR)/crc32_test.o: $(THIRD_PARTY)/libpng/crc32.h $(ODIR)/crc32.o
	$(CCCFLAGS) -c $(TDIR)/crc32_test.c -o $@

$(BDIR)/crc32_test: $(ODIR)/crc32.o $(ODIR)/crc32_test.o
	$(CCCFLAGS) $(ODIR)/crc32.o $(ODIR)/crc32_test.o -o $@

tests: $(BDIR)/cryptography_test $(BDIR)/fpe_test $(BDIR)/crc32_test

#TODO: Enable once we have a main function
#$(BDIR)/image_crypt: $(ODIR)/cryptography.o $(ODIR)/fpe.o $(ODIR)/crc32.o\
	$(CCCFLAGS) $(ODIR)/*.o $(ALL_LIBS) -o $@

clean:
	-rm $(ODIR)/*.o $(BDIR)/*
