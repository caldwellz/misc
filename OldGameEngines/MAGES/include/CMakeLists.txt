configure_file(
	"${INCLUDES_DIR}/MAGES/Globals.hxx.in"
	"${INCLUDES_DIR}/MAGES/Globals.hxx")
set(CONFIGURED_INCLUDES "${INCLUDES_DIR}/Globals.hxx")

set(DEST_SUBDIR_INCLUDES "include"
	CACHE PATH
	"The destination subdirectory to install includes to.")

install(DIRECTORY "${INCLUDES_DIR}/MAGES"
	DESTINATION "${DEST_PREFIX}${DEST_SUBDIR_INCLUDES}/"
	FILES_MATCHING PATTERN "*.hxx")
