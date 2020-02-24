cmake_minimum_required(VERSION 3.13...3.16)

if (${CMAKE_VERSION} VERSION_LESS 3.16)
	cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
endif()

project(ImageCrypt VERSION 0.1
                   DESCRIPTION "In-Place Image Encryption"
                   HOMEPAGE_URL "https://gitlab.com/lptech1024/ImageCrypt"
                   LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED True)

include(GNUInstallDirs)

find_program(CCACHE_PROGRAM ccache)
if (CCACHE_PROGRAM)
	set(CMAKE_C_COMPILER_LAUNCHER "${CCACHE_PROGRAM}")
endif()

option(CLANG_TIDY_FIX "Perform fixes for Clang-Tidy" OFF)
find_program(
	CLANG_TIDY_EXE
	NAMES "clang-tidy"
	DOC "Path to clang-tidy executable")
if (CLANG_TIDY_EXE)
	if (CLANG_TIDY_FIX)
		set(CMAKE_C_CLANG_TIDY "${CLANG_TIDY_EXE}" -fix)
	else()
		set(CMAKE_C_CLANG_TIDY "${CLANG_TIDY_EXE}")
	endif()
else()
	message(STATUS "clang-tidy not found")
endif()

add_executable(imagecrypt "")
include(InstallRequiredSystemLibraries)

set(CMAKE_SOURCE_DIR "src")
add_subdirectory(src)
add_subdirectory(doc)

# TODO: Replace with unit testing framework
add_subdirectory(tests)

# Works with GCC, Clang. What other compilers will be supported (and is the flag supported)?
string(APPEND CMAKE_C_FLAGS_DEBUG " -Wall")

set(default_build_type "RelWithDebInfo")

if (NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
	message(STATUS "Setting build type to '${default_build_type}' as none was specified.")
	set (CMAKE_BUILD_TYPE "${default_built_type}" CACHE
		STRING "Choose the type of build." FORCE)

	set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS
		"Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

set(CPACK_PACKAGE_VENDOR "Clayton Casciato")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
set(CPACK_RESOURCE_FILE_README "${CMAKE_CURRENT_SOURCE_DIR}/README.md")
set(CPACK_PACKAGE_CHECKSUM "SHA256")

# Fedora - https://docs.fedoraproject.org/en-US/quick-docs/creating-rpm-packages/
set(CPACK_RPM_PACKAGE_DESCRIPTION "Encrypt images while still remaining a valid image format.\n\nSupported file formats: PNG")
set(CPACK_RPM_PACKAGE_RELEASE 1)
set(CPACK_RPM_PACKAGE_RELEASE_DIST ON)
set(CPACK_RPM_PACKAGE_LICENSE "MIT")
set(CPACK_RPM_FILE_NAME "RPM-DEFAULT")
set(CPACK_RPM_PACKAGE_URL "${CMAKE_PROJECT_HOMEPAGE_URL}")
set(CPACK_RPM_CHANGELOG_FILE "${CMAKE_CURRENT_SOURCE_DIR}/doc/rpm-changelog")
# TODO: Other RPM-based distribution recommendations?
set(CPACK_RPM_PACKAGE_RELOCATABLE "OFF")
# TODO: Set per OS
#set(CPACK_GENERATOR "RPM")
#set(CPACK_RPM_GENERATE_USER_BINARY_SPECFILE "imagecrypt.spec")
#set(CPACK_RPM_USER_BINARY_SPECFILE "${CMAKE_CURRENT_SOURCE_DIR}/imagecrypt.spec.in")
#TODO: Include license and README in RPM files section
include(CPack)
