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
    set nocompatible
    filetype off
    " set the runtime path to include Vundle and initialize
    set rtp+=~/vim/bundle/Vundle.vim
    call vundle#begin()
    
    Plugin 'VundleVim/Vundle.vim'
    
    Plugin 'Valloric/YouCompleteMe'
    
    call vundle#end()
    
    " \t maps to $x spaces
    set tabstop=$x
    
    " Visual < and > uses tabstop setting
    set shiftwidth=0
    
    let g:ycm_collect_identifiers_from_comments_and_strings = 1
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
  * [Valloric/YouCompleteMe](https://github.com/Valloric/YouCompleteMe) (and [VundleVim/Vundle](https://github.com/VundleVim/Vundle.vim))
