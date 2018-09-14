# Generate and install documentation

find_package(Doxygen)
if(DOXYGEN_FOUND)
	set(GENERATE_DOC ON CACHE BOOL "Generate documentation")
	if(GENERATE_DOC)
		configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)
		add_custom_target(gendoc ALL
			${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
			WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
			COMMENT "Generating documentation with Doxygen" VERBATIM)

		set(INSTALL_DOC ON CACHE BOOL "Install documentation")
		if(INSTALL_DOC)
			if(WIN32)
				set(INSTALL_DOC_DIR Doc CACHE PATH "Installation subdirectory for documentation")
			#elseif(APPLE) # I don't know where OSX likes to store things, so for now the UNIX case handles it
			elseif(UNIX)
				set(INSTALL_DOC_DIR share/doc/Overmind-${VERSION_STRING} CACHE PATH "Installation directory for documentation")
			#else() # Most systems should fit one of the above cases, so I don't know what to do here...
			endif()
			install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/html DESTINATION ${CMAKE_INSTALL_PREFIX}/${INSTALL_DOC_DIR})
		endif()
	endif()
endif()
