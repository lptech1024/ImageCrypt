#TODO: Research how to do this with Clang
CC=gcc

ODIR=obj

LIBS=-lm -lcrypto
SOURCE=src

THIRD_PARTY=$(SOURCE)/third_party

.PHONY: all clean

all: $(ODIR)/cryptography.o $(ODIR)/fpe.o $(ODIR)/crc32.o
#image_crypt

$(ODIR)/cryptography.o: $(SOURCE)/cryptography.h $(SOURCE)/cryptography.c
	$(CC) -c $(SOURCE)/cryptography.c -o $@

$(ODIR)/fpe.o: $(THIRD_PARTY)/format_preserving_encryption/fpe.h $(THIRD_PARTY)/format_preserving_encryption/fpe.c
	$(CC) -c $(THIRD_PARTY)/format_preserving_encryption/fpe.c -o $@

$(ODIR)/crc32.o: $(THIRD_PARTY)/libpng/crc32.h $(THIRD_PARTY)/libpng/crc32.c
	$(CC) -c $(THIRD_PARTY)/libpng/crc32.c -o $@

#TODO: Enable once we have a main function
#image_crypt: $(ODIR)/cryptography.o $(ODIR)/fpe.o $(ODIR)/crc32.o\
	$(CC) $(ODIR)/*.o $(LIBS) -o $@

clean:
	-rm $(ODIR)/*.o
#./image_crypt
