
set(LIB_CUSTOM ""
	CACHE FILEPATH
	"Location of a custom library to link to if needed, e.g. a special C standard lib.")


option(BUILD_DEPENDENCIES "Build dependencies (vs. using system-provided ones)." OFF)
if(BUILD_DEPENDENCIES)

	option(BUILD_DEP_POCO "Build Poco" ON)
	if(BUILD_DEP_POCO)
		# TODO: Package and add build support for Poco
		add_definitions(-DPOCO_NO_AUTOMATIC_LIBS)
	endif(BUILD_DEP_POCO)
	# set(MAGES_DEPENDENCIES "${LIB_CUSTOM}")

else(BUILD_DEPENDENCIES)

	find_library(LIB_POCOFOUNDATION "PocoFoundation"
		DOC "Location of the Poco Foundation library.")
	find_library(LIB_POCOUTIL "PocoUtil"
		DOC "Location of the Poco Util library.")
	set(MAGES_DEPENDENCIES "${LIB_POCOFOUNDATION} ${LIB_POCOUTIL} ${LIB_CUSTOM}"
		CACHE STRING "Final dependency list for linking MAGES. Can be modified if necessary.")

endif(BUILD_DEPENDENCIES)
