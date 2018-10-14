# Q&A
*What is a good use case for ImageCrypt?*\
\
Securely backing up image files where a valid image format is desired (secure obfuscation) and/or required.\
\
\
*Can this be used for compressible image formats?*\
\
At the current unfinished state, I would **expect** these format would work.\
Please review the next question.\
\
\
*If an encrypted image is modified, will I be able to unencrypt the image?*
\
Certain parts of an image are not encrypted. Modifying these parts will not prevent unencryption.\
Modifying the image parts that have been encrypted will result in degraded and/or effectively useless images (including post-encryption compression).\
\
\
*Does the encrypted image have to be viewable?*\
\
No. At the current unfinished state, I would **expect** them to be.\
\
\
*Can I submit pull requests to support image formats not yet supported?*\
\
Yes.
