add_library(imagecryptlib "")

add_subdirectory(tools)

target_sources(imagecryptlib
	PRIVATE
		determine_format.c
		determine_format.h
		png.c
		png.h
		tools
		user_input_handling.c
		user_input_handling.h
)

find_library(MATH_LIBRARY m)
if (MATH_LIBRARY)
	target_link_libraries(imagecryptlib PUBLIC ${MATH_LIBRARY})
endif()

find_package(OpenSSL REQUIRED)
target_link_libraries(imagecryptlib PRIVATE crypto)

target_sources(imagecrypt PRIVATE cli.c)

target_link_libraries(imagecrypt PRIVATE imagecryptlib)

install(TARGETS imagecrypt DESTINATION ${CMAKE_INSTALL_BINDIR})
