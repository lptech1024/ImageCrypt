**Indentation and Alignment**

Use tabs for indentation.

Use spaces for alignment.

Don't include trailing whitespace at the end of a file or line.

Rationale: Allows for customized indentation representation size and alignment regardless of tab representation size.

**Include Statements**

Library includes should proceed local include.

Rationale: Project functions should take precedence.

**Header Files**

If a ".c" file will be used by another, create a header file with the same name, but with the file extension as ".h".

Header files should be minimized to intended external usage. E.g. Internally-used functions in a ".c" file should not be included in the header file.

Header files must have #define guards. The guards should take the format of IMAGECRYPT_$FILENAME.

Include header files for forward declarations.

**Line Endings**

Use Unix line endings (LF, \n, 0x0A).

**Variables**

Avoid global variables when possible.

Avoid magic values when possible.

Create meaningful variable names.

**Naming**

Filenames and variables: Lowercase, separated by underscores.

Macros and enums: Uppercase, separated by underscores.

**Commits**

Commits should be small and related.

Commits must have a description.

Signing commits with a GitLab-verified GPG key is preferred.

**Comments**

// or /* */ is acceptable.

Comments in ".c" files should focus on implementation details. Comments in ".h" files should provide an overview of intention and usage.

Commented code is discouraged. If used with "//", don't include a space after "//".

General text comments should include a space at the beginning. E.g.: "// This is a general text comment".

"TODO" should be in upper case.

Code comments should be in English. Feel free to use a translator as needed.
