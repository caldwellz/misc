set(SRC_LIST
	"${SRC_DIR}/Core.cxx"
	"${SRC_DIR}/PropertyDB.cxx"
	"${SRC_DIR}/PropertySerializer.cxx"
	"${SRC_DIR}/SerializerJSON.cxx"
	"${SRC_DIR}/Subsystem.cxx"
)

include_directories("${INCLUDES_DIR}")
add_library(MAGES ${SRC_LIST})
set_target_properties(MAGES PROPERTIES
	VERSION "${MAGES_VERSION_MAJOR}.${MAGES_VERSION_MINOR}.${MAGES_VERSION_PATCH}")


# Dependency list is set in the DEPS CMakeList
target_link_libraries(MAGES ${MAGES_DEPENDENCIES})


set(DEST_SUBDIR_BINARIES "bin"
	CACHE PATH
	"The destination subdirectory to install binaries to.")
set(DEST_SUBDIR_LIBRARIES "lib"
	CACHE PATH
	"The destination subdirectory to install libraries to.")
set(DEST_SUBDIR_ARCHIVES "lib"
	CACHE PATH
	"The destination subdirectory to install library archives to.")

install(TARGETS MAGES
	RUNTIME DESTINATION "${DEST_PREFIX}/${DEST_SUBDIR_BINARIES}"
	LIBRARY DESTINATION "${DEST_PREFIX}/${DEST_SUBDIR_LIBRARIES}"
	ARCHIVE DESTINATION "${DEST_PREFIX}/${DEST_SUBDIR_ARCHIVES}")
