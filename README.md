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

CI builds: Debian, Ubuntu, Fedora, Mageia, Arch Linux, and Clear Linux

[![pipeline status](https://gitlab.com/lptech1024/ImageCrypt/badges/master/pipeline.svg)](https://gitlab.com/lptech1024/ImageCrypt/commits/master)

## Setup

Please review .gitlab-ci.yml for minimal configuration.

### Recommended Developer Tools
* valgrind (debugging, profiling)
* GDB (debugger)
