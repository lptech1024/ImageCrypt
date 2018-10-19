[PNG](https://en.wikipedia.org/wiki/Portable_Network_Graphics)\
\
**File header (signature)**

| 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 |
| --- | --- | --- | --- | --- | --- | --- | --- |
| 89 | 50 | E4 | 47 | 0D | 0A | 1A | 0A |
| | P | N | G | | | | |

\
**Chunk Layout**

| Length | Chunk Type (Name) | Chunk Data | CRC |
| --- | --- | --- | --- |
| n=4B | 4B | nB | 4B |
| Big-endian | Little-endian | ? | Big-endian |

\
**Critial Chunks**\
\
IHDR
* Must be first
* Basic image details
* Encrypted by default? No

\
PLTE
* Palette (list of colors)
* Encrypted by default? No

\
IDAT
* Contains image
* Multiple possible? Yes
* Encrypted by default? Yes

\
IEND
* Marks image end
* Encrypted? No

\
\
**Ancillary Chunks**
\
bKGD
* Default background color
* Encrypted by default? No

\
cHRM
* Objective color data
* 32 bytes in chunks of 4
* Encrypted by default? Yes

\
dSIG
* Digital signatures
* Encrypted by default? Yes

\
eXIf
* Metadata
* Encrypted by default? Yes

\
gAMA
* Four (4) byte unsigned integer
* Encrypted by default? No

\
hIST
* Series of two (2) byte unsigned integers

\
iCCP
* Embedded ICC Profile
* Encrypted by default? Yes

\
iTXt
* Encrypted by default? Yes

\
pHYs
* Aspect ratio
* Nine (9) bytes
* Encrypted by default? No

\
sBIT
* Sample depths
* Encrypted by default? No

\
sPLT
* Suggested reduced palette
* Encrypted by default? No

\
sRGB
* One (1) byte
* Encrypted by default? No

\
sTER
* One (1) byte
* Encrypted by default? No


\
tEXt
* Encrypted by default? Yes

\
tIME
* Time of last image modification
* Seven (7) bytes
* Encrypted by default? Yes

\
tRNS
* Simple transparency
* Encrypted by default? Yes

\
zTXt
* Encrypted by default? Yes

\
**Unknown**
* Additional "mainstream" tags found in the wild should be added to this list, especially if they shouldn't be encrypted by default.
* Encrypted by default? Yes
