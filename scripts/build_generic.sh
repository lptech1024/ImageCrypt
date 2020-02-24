# This script is used to build on platforms without custom instructions.

# TODO: Add shebang

# TODO: Check for tools used
# TODO: Improve structure
# TODO: Consider adding error codes
# TODO: Consider adding man page

# TODO: Check command outputs

set -e
unset IFS

mkdir build
cd build
cmake ..
make
