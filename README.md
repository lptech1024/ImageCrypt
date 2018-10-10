# ImageCrypt
In-place Image Encryption

## Goal
Encrypt existing images while remaining a valid image file

## Q&A
*What is a good use case for ImageCrypt?*\
Securely backing up image files where a valid image format is desired (secure obfuscation) and/or required.
\
\
*Can this be used for compressible image formats?*\
At the current unfinished state, I would **expect** these format would work.\
Please review the next question.\
\
*Will I be able to decrypt photos if the encrypted photos's encrypted portions are modified (including compression)?*\
No.\
\
*Does the encrypted image have to be viewable?*\
No. At the current unfinished state, I would **expect** them to be.\
\
*Can I submit pull requests to support image formats not yet supported?*\
\
Yes.

## Image Format Support Priority
Primary Developer Image Format Support Priority
1. [] [PNG](https://en.wikipedia.org/wiki/Portable_Network_Graphics) (in progress)
2. [] [NEF](https://www.nikonusa.com/en/learn-and-explore/a/products-and-innovation/nikon-electronic-format-nef.html), [TIFF/EP](https://en.wikipedia.org/wiki/TIFF/EP), and/or [TIFF](https://en.wikipedia.org/wiki/TIFF)

## Technologies/Concepts
* [Cyclic Redundancy Check](https://en.wikipedia.org/wiki/Cyclic_redundancy_check)
* [Endianness](https://en.wikipedia.org/wiki/Endianness)
* [Format Preserving Encryption](https://en.wikipedia.org/wiki/Format-preserving_encryption)
* Deep Image Format Knowledge

## TODO
* Code standards decisions and guide
* Add license file containing MIT
* Create project structure
* Review GCC and Clang
* Add makefile
* Add build instructions
* Provide test cases
