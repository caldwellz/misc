project(MAGES)
cmake_minimum_required(VERSION 3.0) # Just a random guess for now
set(MAGES_VERSION_MAJOR 0)
set(MAGES_VERSION_MINOR 1)
set(MAGES_VERSION_PATCH 5)

set(INCLUDES_DIR "${PROJECT_SOURCE_DIR}/include")
set(DEPS_DIR "${PROJECT_SOURCE_DIR}/depends")
set(SRC_DIR "${PROJECT_SOURCE_DIR}/src")
set(TESTS_DIR "${PROJECT_SOURCE_DIR}/tests")
set(DOCS_DIR "docs/MAGES"
	CACHE PATH "Directory to generate documentation in (if enabled).")
set(DEST_PREFIX "/usr/local"
	CACHE PATH "The base installation directory.")

add_subdirectory(${INCLUDES_DIR})
add_subdirectory(${DEPS_DIR})
add_subdirectory(${SRC_DIR})
add_subdirectory(${TESTS_DIR})

option(ASTYLE_RUN "Run code through AStyle." OFF)
set(ASTYLE_OPTIONS_FILE "${PROJECT_SOURCE_DIR}/AStyleRC"
	CACHE FILEPATH "The options file to use with AStyle.")
if(ASTYLE_RUN)
	find_program(ASTYLE_LOCATION astyle DOC "Location of the AStyle binary.")
	if(ASTYLE_LOCATION)
		add_custom_target(astyle ALL
			"${ASTYLE_LOCATION}" "--options=${ASTYLE_OPTIONS_FILE}" "--recursive" "${INCLUDES_DIR}/*" "${SRC_DIR}/*" "${TESTS_DIR}/*"
			WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}"
			COMMENT "Running code through AStyle..." VERBATIM)
	endif(ASTYLE_LOCATION)
endif(ASTYLE_RUN)


option(GENERATE_DOCS "Generate documentation." ON)
option(INSTALL_DOCS "Install the documentation." OFF)
set(DEST_SUBDIR_DOCS "doc"
	CACHE PATH "The destination subdirectory to install generated docs to (if enabled).")
if(GENERATE_DOCS)
	find_package(Doxygen)
	if(DOXYGEN_FOUND)
		configure_file("${PROJECT_SOURCE_DIR}/Doxyfile.in" "${DOCS_DIR}/Doxyfile" @ONLY)
		add_custom_target(doc ALL
			"${DOXYGEN_EXECUTABLE}" "${DOCS_DIR}/Doxyfile"
			WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}"
			COMMENT "Generating API documentation with Doxygen..." VERBATIM)
		if(INSTALL_DOCS)
			install(TARGET doc DIRECTORY "${DOCS_DIR}"
				DESTINATION "${DEST_PREFIX}/${DEST_SUBDIR_DOCS}/"
				EXCLUDE "${DOCS_DIR}/Doxyfile")
		endif(INSTALL_DOCS)
	endif(DOXYGEN_FOUND)
endif(GENERATE_DOCS)
