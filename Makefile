CC=clang
CFLAGS=-Wall
CIFLAGS=-Werror
CDEBUG=-g
CSTD=-std=gnu17
OPT=-O2
CCCFLAGS=$(CC) $(CSTD)

ODIR=obj
TDIR=tests
SOURCE=src

prefix=/usr/local
exec_prefix=$(prefix)
bindir=$(exec_prefix)/bin
datarootdir=$(prefix)/share
mandir=$(datarootdir)/man
man1dir=$(mandir)/man1
manext='.1.'

LIB_MATH=-lm
LIB_CRYPTO=-lcrypto
ALL_LIBS=$(LIB_MATH) $(LIB_CRYPTO)

TOOLS=$(SOURCE)/tools

ALL=all
INSTALL_TARGET=install
RELEASE_TARGET=release
DEBUG_TARGET=debug
CI_TARGET=ci
CLEAN=clean

.PHONY: $(ALL) $(INSTALL_TARGET) $(RELEASE_TARGET) $(DEBUG_TARGET) $(CI_TARGET) $(CLEAN)

# Don't delete any intermediate files (e.g. .o)
.SECONDARY:

$(ALL): $(RELEASE_TARGET) $(RELEASE_TARGET)/tests

.SECONDEXPANSION:

$(INSTALL_TARGET): $(RELEASE_TARGET)
$(INSTALL_TARGET):
	mkdir -p $(bindir) && install -m 0755 ./release/imagecrypt $(bindir)/imagecrypt
	gzip -c doc/imagecrypt.1 > $(man1dir)/imagecrypt.1.gz && mandb

$(RELEASE_TARGET): CCCFLAGS += $(OPT)
$(RELEASE_TARGET): $$@/imagecrypt

$(DEBUG_TARGET): CCCFLAGS += $(CDEBUG) $(CFLAGS)
$(DEBUG_TARGET): $$@/imagecrypt $$@/tests

$(CI_TARGET): CCCFLAGS += $(CIFLAGS)
$(CI_TARGET): $(DEBUG_TARGET)

$(CLEAN):
	-rm release/* release/obj/*.o debug/* debug/obj/*.o

%/cryptography.o: $(TOOLS)/cryptography.c $(TOOLS)/cryptography.h
	$(CCCFLAGS) -c $< -o $@

%/cryptography_test.o: $(TDIR)/cryptography_test.c $(TOOLS)/cryptography.h
	$(CCCFLAGS) -c $< -o $@

%/cryptography_test: %/$(ODIR)/cryptography.o %/$(ODIR)/cryptography_test.o
	$(CCCFLAGS) $^ $(LIB_CRYPTO) -o $@

%/fpe.o: $(TOOLS)/fpe.c $(TOOLS)/fpe.h
	$(CCCFLAGS) -c $< -o $@

%/fpe_test.o: $(TDIR)/fpe_test.c $(TOOLS)/fpe.h
	$(CCCFLAGS) -c $< -o $@

%/fpe_test: %/$(ODIR)/fpe.o %/$(ODIR)/fpe_test.o
	$(CCCFLAGS) $^ $(ALL_LIBS) -o $@

%/crc32.o: $(TOOLS)/crc32.c $(TOOLS)/crc32.h
	$(CCCFLAGS) -c $< -o $@

%/crc32_test.o: $(TDIR)/crc32_test.c $(TOOLS)/crc32.h %/crc32.o
	$(CCCFLAGS) -c $< -o $@

%/crc32_test: %/$(ODIR)/crc32.o %/$(ODIR)/crc32_test.o
	$(CCCFLAGS) $^ -o $@

%/string_collection.o: $(TOOLS)/string_collection.c $(TOOLS)/string_collection.h
	$(CCCFLAGS) -c $< -o $@

%/string_collection_test.o: $(TDIR)/string_collection_test.c $(TOOLS)/string_collection.h %/string_collection.o
	$(CCCFLAGS) -c $< -o $@

%/string_collection_test: %/$(ODIR)/string_collection.o %/$(ODIR)/string_collection_test.o
	$(CCCFLAGS) $^ -o $@

%/transform_details.o: $(TOOLS)/transform_details.c $(TOOLS)/transform_details.h
	$(CCCFLAGS) -c $< -o $@

#%/transform_details_test.o: $(TDIR)/transform_details_test.c $(TOOLS)/transform_details.h %/transform_details.o\
	$(CCCFLAGS) -c $< -o $@

#%/transform_details_test: %/$(ODIR)/transform_details.o %/$(ODIR)/transform_details_test.o\
	$(CCCFLAGS) $^ -o $@

%/png.o: $(SOURCE)/png.c $(SOURCE)/png.h %/fpe.o %/crc32.o
	$(CCCFLAGS) -c $< -o $@

%/determine_format.o: $(SOURCE)/determine_format.c $(SOURCE)/determine_format.h %/transform_details.o %/png.o
	$(CCCFLAGS) -c $< -o $@

%/user_input_handling.o: $(SOURCE)/user_input_handling.c $(SOURCE)/user_input_handling.h %/transform_details.o %/png.o %/fpe.o %/cryptography.o
	$(CCCFLAGS) -c $< -o $@

%/cli.o: $(SOURCE)/cli.c %/string_collection.o %/user_input_handling.o
	$(CCCFLAGS) -c $< -o $@

%/imagecrypt: %/$(ODIR)/string_collection.o %/$(ODIR)/cli.o %/$(ODIR)/transform_details.o %/$(ODIR)/user_input_handling.o %/$(ODIR)/determine_format.o %/$(ODIR)/png.o %/$(ODIR)/fpe.o %/$(ODIR)/cryptography.o %/$(ODIR)/crc32.o
	$(CCCFLAGS) $^ $(ALL_LIBS) -o $@

%/tests: %/cryptography_test %/fpe_test %/crc32_test %/string_collection_test;
