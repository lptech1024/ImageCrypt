#list(APPEND CPACK_RPM_EXCLUDE_FROM_AUTO_FILELIST_ADDITION "/usr/share/man")
#set (CPACK_RPM_ADDITIONAL_MAN_DIRS "/***HELLOWORLD***")
#set(CPACK_RPM_EXCLUDE_FROM_AUTO_FILELIST_ADDITION "/usr/share/man/man1")
#list(APPEND CPACK_RPM_EXCLUDE_FROM_AUTO_FILELIST_ADDITION ${CMAKE_INSTALL_MANDIR})
#list(APPEND CPACK_RPM_EXCLUDE_FROM_AUTO_FILELIST_ADDITION "${CMAKE_INSTALL_MANDIR}/man1")
#list(APPEND CPACK_RPM_EXCLUDE_FROM_AUTO_FILELIST_ADDITION "/usr/share/man/man1")
#list(REMOVE_ITEM INSTALL_FILES_LIST "/usr/share/man/man1")
#set(CPACK_RPM_SPEC_MORE_DEFINE "${CPACK_RPM_SPEC_MORE_DEFINE}
#%define  \#")
#set(CPACK_RPM_SPEC_MORE_DEFINE "%undefine __brp_compress" PARENT_SCOPE)
#set(CPACK_RPM_SPEC_MORE_DEFINE "%undefine __os_install_post")
#set(CPACK_RPM_USER_FILELIST "%ignore \%{_mandir} \"/usr/share/man/man1\"")
#set(CPACK_RPM_USER_FILELIST "%ignore %__brp_compress")
#set(CPACK_RPM_USER_FILELIST "%ignore \"/usr/bin/imagecrypt\"")
#set(CPACK_RPM_imagecrypt_FILELIST "%ignore %dir /usr/bin/imagecrypt")
#set(CPACK_RPM_USER_FILELIST "%dir /HELLO")
#set(CPACK_RPM_imagecrypt_FILELIST "%dir /HELLO2")
#set(CPACK_RPM_SPEC_INSTALL_POST "%{nil}")
#set(CPACK_RPM_man_USER_FILELIST "%ignore /usr/share/man/man1")
#set(CPACK_RPM_USER_FILELIST "%ignore /usr/share/man/man1")
#set(CPACK_RPM_man_USER_FILELIST "%ignore ${CMAKE_INSTALL_MANDIR}")
#set(CPACK_RPM_imagecrypt_USER_FILELIST "%ignore ${CMAKE_INSTALL_MANDIR}")
#set(CPACK_RPM_USER_USER_FILELIST "%ignore ${CMAKE_INSTALL_MANDIR}")
# TODO
#SET(CPACK_RPM_SPEC_MORE_DEFINE "
#%define 
#")
#set(CPACK_RPM_USER_FILELIST "%exclude %dir /usr/share/man")
#set(CPACK_RPM_SPEC_MORE_DEFINE "%define ignore \#")
#set(CPACK_RPM_USER_FILELIST "%ignore %dir \"/usr/share/man\"")
find_program(GZIP gzip)
add_custom_command(
     OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/imagecrypt.1.gz
     COMMAND mkdir -p ${CMAKE_CURRENT_BINARY_DIR}
     #COMMAND ${GZIP} --to-stdout imagecrypt.1 > ${CMAKE_CURRENT_BINARY_DIR}/man1/imagecrypt.1.gz
     COMMAND ${GZIP} --to-stdout ${CMAKE_CURRENT_SOURCE_DIR}/imagecrypt.1 > ${CMAKE_CURRENT_BINARY_DIR}/imagecrypt.1.gz
     DEPENDS imagecrypt.1
#WORKING_DIRECTORY man1
)
#add_custom_command(
#OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/man1/imagecrypt.1.gz
#COMMAND mkdir -p ${CMAKE_CURRENT_BINARY_DIR}/man1
##COMMAND ${GZIP} --to-stdout imagecrypt.1 > ${CMAKE_CURRENT_BINARY_DIR}/man1/imagecrypt.1.gz
#COMMAND ${GZIP} --to-stdout ${CMAKE_CURRENT_SOURCE_DIR}/man1/imagecrypt.1 > ${CMAKE_CURRENT_BINARY_DIR}/man1/imagecrypt.1.gz
#DEPENDS man1/imagecrypt.1
##WORKING_DIRECTORY man1
#)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/imagecrypt.1.gz
        # 755
		  #PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE
		  #GROUP_READ GROUP_EXECUTE
		  #WORLD_READ WORLD_EXECUTE
		  DESTINATION ${CMAKE_INSTALL_MANDIR}/man1
		  # PATTERN "${CMAKE_INSTALL_MANDIR}" EXCLUDE
		  #PATTERN "${CMAKE_INSTALL_MANDIR}/man1" EXCLUDE
		  )
#install(CODE "cp ${CMAKE_CURRENT_BINARY_DIR}/imagecrypt.1.gz ${CMAKE_INSTALL_MANDIR}/man1/.")
add_custom_target(man ALL DEPENDS imagecrypt.1.gz)
#add_custom_target(man ALL DEPENDS man1/imagecrypt.1.gz)
#install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/man1
#        DESTINATION ${CMAKE_INSTALL_MANDIR}
#		  PATTERN "${CMAKE_INSTALL_MANDIR}" EXCLUDE)
	  #set(CPACK_RPM_EXCLUDE_FROM_AUTO_FILELIST_ADDITION ${CMAKE_INSTALL_MANDIR})
	  #set(CPACK_RPM_EXCLUDE_FROM_AUTO_FILELIST ${CMAKE_INSTALL_MANDIR})
	  #set(CPACK_RPM_ADDITIONAL_MAN_DIRS "/usr/share/man/man1/")
