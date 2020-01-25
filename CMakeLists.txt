cmake_minimum_required(VERSION 3.8...3.16)

if (${CMAKE_VERSION} VERSION_LESS 3.16)
	cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
endif()

project(ImageCrypt VERSION 0.1
                   DESCRIPTION "In-Place Image Encryption"
                   LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED True)

find_program(CCACHE_PROGRAM ccache)
if (CCACHE_PROGRAM)
	set(CMAKEC_COMPILER_LAUNCHER "${CCACHE_PROGRAM}")
endif()

option(CLANG_TIDY_FIX "Perform fixes for Clang-Tidy" OFF)
find_program(
	CLANG_TIDY_EXE
	NAMES "clang-tidy"
	DOC "Path to clang-tidy executable")
if (CLANG_TIDY_EXE)
	if (CLANG_TIDY_FIX)
		set(CMAKE_C_CLANG_TIDY "${CLANG_TIDY_EXE}" -checks=bugprone-*,cert-*,clang-analyzer-*,misc-*,modernize-*,performance-*,portability-*,readability-* -fix)
	else()
		set(CMAKE_C_CLANG_TIDY "${CLANG_TIDY_EXE}" -checks=bugprone-*,cert-*,clang-analyzer-*,misc-*,modernize-*,performance-*,portability-*,readability-*)
	endif()
else()
	message(STATUS "clang-tidy not found")
endif()

add_library(imagecryptlib STATIC src/tools/safety.c src/tools/safety.h src/tools/cryptography.c src/tools/cryptography.c src/tools/fpe.h src/tools/fpe.c src/tools/crc32.c src/tools/crc32.h src/tools/string_collection.c src/tools/string_collection.h src/tools/transform_details.c src/tools/transform_details.h src/png.c src/png.h src/user_input_handling.c src/user_input_handling.h src/determine_format.c src/determine_format.h)

# TODO: Add tests

add_executable(imagecrypt src/cli.c)
find_library(MATH_LIBRARY m)
if (MATH_LIBRARY)
	target_link_libraries(imagecryptlib PUBLIC ${MATH_LIBRARY})
endif()

find_package(OpenSSL REQUIRED)
target_link_libraries(imagecryptlib PRIVATE crypto)
target_link_libraries(imagecrypt PRIVATE imagecryptlib)

set(default_build_type "RelWithDebInfo")

if (NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
	message(STATUS "Setting build type to '${default_build_type}' as none was specified.")
	set (CMAKE_BUILD_TYPE "${default_built_type}" CACHE
		STRING "Choose the type of build." FORCE)

	set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS
		"Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()
