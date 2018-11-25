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

## Recommended Setup Steps
1. If using Vim,

    a. Use [tpope's "Effortless Ctags with Git"](https://tbaggery.com/2011/08/08/effortless-ctags-with-git.html)
  
    b. .vimrc
    ```Makefile
    
    " \t maps to $x spaces
    set tabstop=$x
    
    " Visual < and > uses tabstop setting
    set shiftwidth=0
    ```
2. git clone https://github.com/lptech1024/ImageCrypt

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
* vim
  * ctags
  * vim-fugitive
