# ImageCrypt
In-place Image Encryption

## Goal
Encrypt existing images while remaining a valid image file

## Image Format Support Priority
Primary Developer Image Format Support Priority
1. [PNG](https://en.wikipedia.org/wiki/Portable_Network_Graphics) (in progress)
1. [NEF](https://www.nikonusa.com/en/learn-and-explore/a/products-and-innovation/nikon-electronic-format-nef.html), [TIFF/EP](https://en.wikipedia.org/wiki/TIFF/EP), and/or [TIFF](https://en.wikipedia.org/wiki/TIFF)

## Make
* make
* make tests
* make clean

## Dependencies

### Building
* clang (recommended) or gcc
* make
* libm
* libcrypto

### Developing

#### Recommended
* valgrind
* rm
