cmake_minimum_required(VERSION 3.5...3.16)

if (${CMAKE_VERSION} VERSION_LESS 3.16)
	cmake_policy(VERSION ${CMAKE_MAJOR_VERSION.${CMAKE_MINOR_VERSION})
endif()

project(ImageCrypt VERSION 0.1
                   DESCRIPTION "In-Place Image Encryption"
                   LANGUAGES C)

set(CMAKE_C_STANDARD 11)
#set(CMAKE_C_STANDARD_REQUIRED True)

add_library(imagecryptlib STATIC src/tools/safety.c src/tools/safety.h src/tools/cryptography.c src/tools/cryptography.c src/tools/fpe.h src/tools/fpe.c src/tools/crc32.c src/tools/crc32.h src/tools/string_collection.c src/tools/string_collection.h src/tools/transform_details.c src/tools/transform_details.h src/png.c src/png.h src/user_input_handling.c src/user_input_handling.h src/determine_format.c src/determine_format.h)

# TODO: Add tests

add_executable(imagecrypt src/cli.c)
find_package(OpenSSL REQUIRED)
target_link_libraries(imagecrypt PRIVATE imagecryptlib crypto m)
