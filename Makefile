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

TOOLS=$(SOURCE)/tools

.PHONY: all clean

all: $(ODIR)/cryptography.o $(ODIR)/fpe.o $(ODIR)/crc32.o $(BDIR)/imagecrypt

$(ODIR)/cryptography.o: $(TOOLS)/cryptography.h $(TOOLS)/cryptography.c
	$(CCCFLAGS) -c $(TOOLS)/cryptography.c -o $@

$(ODIR)/cryptography_test.o: $(TOOLS)/cryptography.h $(TDIR)/cryptography_test.c
	$(CCCFLAGS) -c $(TDIR)/cryptography_test.c -o $@

$(BDIR)/cryptography_test: $(ODIR)/cryptography.o $(ODIR)/cryptography_test.o
	$(CCDEBUG) $^ $(LIB_CRYPTO) -o $@

$(ODIR)/fpe.o: $(TOOLS)/fpe.h $(TOOLS)/fpe.c
	$(CCCFLAGS) -c $(TOOLS)/fpe.c -o $@

$(ODIR)/fpe_test.o: $(TOOLS)/fpe.h $(TDIR)/fpe_test.c
	$(CCCFLAGS) -c $(TDIR)/fpe_test.c -o $@

$(BDIR)/fpe_test: $(ODIR)/fpe.o $(ODIR)/fpe_test.o
	$(CCDEBUG) $^ $(ALL_LIBS) -o $@

$(ODIR)/crc32.o: $(TOOLS)/crc32.h $(TOOLS)/crc32.c
	$(CCCFLAGS) -c $(TOOLS)/crc32.c -o $@

$(ODIR)/crc32_test.o: $(TOOLS)/crc32.h $(ODIR)/crc32.o $(TDIR)/crc32_test.c
	$(CCCFLAGS) -c $(TDIR)/crc32_test.c -o $@

$(BDIR)/crc32_test: $(ODIR)/crc32.o $(ODIR)/crc32_test.o
	$(CCDEBUG) $^ -o $@

$(ODIR)/enhanced_string.o: $(TOOLS)/enhanced_string.h $(TOOLS)/enhanced_string.c
	$(CCCFLAGS) -c $(TOOLS)/enhanced_string.c -o $@

$(ODIR)/enhanced_string_test.o: $(TOOLS)/enhanced_string.h $(ODIR)/enhanced_string.o $(TDIR)/enhanced_string_test.c
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
