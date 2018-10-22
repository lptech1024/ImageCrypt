CC=clang
CFLAGS=-Wall
CDEBUG=-g
CCCFLAGS=$(CC) $(CFLAGS)
CCDEBUG=$(CCCFLAGS) $(CDEBUG)

ODIR=obj
BDIR=bin
TDIR=tests
SOURCE=src

LIB_MATH=-lm
LIB_CRYPTO=-lcrypto
ALL_LIBS=$(LIB_MATH) $(LIB_CRYPTO)

THIRD_PARTY=$(SOURCE)/third_party

.PHONY: all clean

all: $(ODIR)/cryptography.o $(ODIR)/fpe.o $(ODIR)/crc32.o $(BDIR)/imagecrypt

$(ODIR)/cryptography.o: $(SOURCE)/cryptography.h $(SOURCE)/cryptography.c
	$(CCCFLAGS) -c $(SOURCE)/cryptography.c -o $@

$(ODIR)/cryptography_test.o: $(SOURCE)/cryptography.h $(TDIR)/cryptography_test.c
	$(CCCFLAGS) -c $(TDIR)/cryptography_test.c -o $@

$(BDIR)/cryptography_test: $(ODIR)/cryptography.o $(ODIR)/cryptography_test.o
	$(CCDEBUG) $^ $(LIB_CRYPTO) -o $@

$(ODIR)/fpe.o: $(THIRD_PARTY)/format_preserving_encryption/fpe.h $(THIRD_PARTY)/format_preserving_encryption/fpe.c
	$(CCCFLAGS) -c $(THIRD_PARTY)/format_preserving_encryption/fpe.c -o $@

$(ODIR)/fpe_test.o: $(THIRD_PARTY)/format_preserving_encryption/fpe.h $(TDIR)/fpe_test.c
	$(CCCFLAGS) -c $(TDIR)/fpe_test.c -o $@

$(BDIR)/fpe_test: $(ODIR)/fpe.o $(ODIR)/fpe_test.o
	$(CCDEBUG) $^ $(ALL_LIBS) -o $@

$(ODIR)/crc32.o: $(THIRD_PARTY)/libpng/crc32.h $(THIRD_PARTY)/libpng/crc32.c
	$(CCCFLAGS) -c $(THIRD_PARTY)/libpng/crc32.c -o $@

$(ODIR)/crc32_test.o: $(THIRD_PARTY)/libpng/crc32.h $(ODIR)/crc32.o $(TDIR)/crc32_test.c
	$(CCCFLAGS) -c $(TDIR)/crc32_test.c -o $@

$(BDIR)/crc32_test: $(ODIR)/crc32.o $(ODIR)/crc32_test.o
	$(CCDEBUG) $^ -o $@

$(ODIR)/enhanced_string.o: $(SOURCE)/tools/enhanced_string.h $(SOURCE)/tools/enhanced_string.c
	$(CCCFLAGS) -c $(SOURCE)/tools/enhanced_string.c -o $@

$(ODIR)/enhanced_string_test.o: $(SOURCE)/tools/enhanced_string.h $(ODIR)/enhanced_string.o $(TDIR)/enhanced_string_test.c
	$(CCCFLAGS) -c $(TDIR)/enhanced_string_test.c -o $@

$(BDIR)/enhanced_string_test: $(ODIR)/enhanced_string.o $(ODIR)/enhanced_string_test.o
	$(CCDEBUG) $^ -o $@

$(ODIR)/cli.o: $(ODIR)/enhanced_string.o $(SOURCE)/cli.c
	$(CCCFLAGS) -c $(SOURCE)/cli.c -o $@

$(BDIR)/imagecrypt: $(ODIR)/enhanced_string.o $(ODIR)/cli.o
	$(CCCFLAGS) $^ -o $@

tests: $(BDIR)/cryptography_test $(BDIR)/fpe_test $(BDIR)/crc32_test $(BDIR)/enhanced_string_test

clean:
	-rm $(ODIR)/*.o $(BDIR)/*
