find_program(GZIP gzip)
add_custom_command(
     OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/imagecrypt.1.gz
     COMMAND mkdir -p ${CMAKE_CURRENT_BINARY_DIR}
     COMMAND ${GZIP} --to-stdout ${CMAKE_CURRENT_SOURCE_DIR}/imagecrypt.1 > ${CMAKE_CURRENT_BINARY_DIR}/imagecrypt.1.gz
     DEPENDS imagecrypt.1
)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/imagecrypt.1.gz
		  DESTINATION ${CMAKE_INSTALL_MANDIR}/man1
		  )
