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

$(ODIR)/string_collection.o: $(TOOLS)/string_collection.h $(TOOLS)/string_collection.c
	$(CCCFLAGS) -c $(TOOLS)/string_collection.c -o $@

$(ODIR)/string_collection_test.o: $(TOOLS)/string_collection.h $(ODIR)/string_collection.o $(TDIR)/string_collection_test.c
	$(CCCFLAGS) -c $(TDIR)/string_collection_test.c -o $@

$(BDIR)/string_collection_test: $(ODIR)/string_collection.o $(ODIR)/string_collection_test.o
	$(CCDEBUG) $^ -o $@

$(ODIR)/cli.o: $(ODIR)/string_collection.o $(SOURCE)/cli.c
	$(CCCFLAGS) -c $(SOURCE)/cli.c -o $@

$(BDIR)/imagecrypt: $(ODIR)/string_collection.o $(ODIR)/cli.o
	$(CCCFLAGS) $^ -o $@

tests: $(BDIR)/cryptography_test $(BDIR)/fpe_test $(BDIR)/crc32_test $(BDIR)/string_collection_test

clean:
	-rm $(ODIR)/*.o $(BDIR)/*
