option(BUILD_TESTS
	"Build the tests and examples." ON)
option(INSTALL_TESTS
	"Install the tests and examples." OFF)

if(BUILD_TESTS)
	if(LIB_POCOFOUNDATION AND LIB_POCOUTIL)
		include_directories("${INCLUDES_DIR}")
		add_executable(TCore "${TESTS_DIR}/TCore.cxx")
		target_link_libraries(TCore MAGES "${LIB_POCOFOUNDATION}" "${LIB_POCOUTIL}" "${LIB_CUSTOM}")
		if(INSTALL_TESTS)
			install(TARGETS TCore
				DESTINATION "${DEST_PREFIX}/${DEST_SUBDIR_BINARIES}")
		endif(INSTALL_TESTS)
	endif(LIB_POCOFOUNDATION AND LIB_POCOUTIL)
endif(BUILD_TESTS)
