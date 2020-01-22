# ImageCrypt
In-place Image Encryption

## Goal
Encrypt existing images while remaining a valid image file

## Image Format Support Priority
Primary Developer Image Format Support Priority
1. [PNG](https://en.wikipedia.org/wiki/Portable_Network_Graphics) (complete)
1. [JPEG](https://en.wikipedia.org/wiki/JPEG) (next planned)
1. [NEF](https://www.nikonusa.com/en/learn-and-explore/a/products-and-innovation/nikon-electronic-format-nef.html)

## Build Status
[![pipeline status](https://gitlab.com/lptech1024/ImageCrypt/badges/master/pipeline.svg)](https://gitlab.com/lptech1024/ImageCrypt/commits/master)

## Setup
* I want to install ImageCrypt as a user
  * make install

* I want to run ImageCrypt as a developer
  * make debug
  * ./debug/imagecrypt

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
