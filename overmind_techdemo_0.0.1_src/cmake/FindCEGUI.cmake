# TODO: Implement component-finding, to allow find_package(COMPONENTS ...)
# - Find CEGUI
# Find the CEGUI includes and libraries
#
# Following variables are provided:
# CEGUI_FOUND
#     True if CEGUI has been found
# CEGUI_INCLUDE_DIRS
#     The include directories of CEGUI
# CEGUI_LIBRARIES
#     CEGUI library list

# CEGUI has more libs, but we currently only find the ones Overmind uses
find_library(CEGUI_BASE CEGUIBase)
find_library(CEGUI_OGRERENDERER CEGUIOgreRenderer)
find_library(CEGUI_LUA CEGUILuaScriptModule)
set(CEGUI_LIBRARIES
	${CEGUI_BASE}
	${CEGUI_OGRERENDERER}
	${CEGUI_LUA}
)

include(CheckIncludeFileCXX)
set(CMAKE_REQUIRED_INCLUDES)
CHECK_INCLUDE_FILE_CXX(CEGUI/CEGUI.h CEGUI_INCLUDE_DIRS)

# Determine CEGUI version
find_path(CEGUI_INCLUDE_DIR NAMES CEGUIVersion.h PATH_SUFFIXES include CEGUI)
file(STRINGS ${CEGUI_INCLUDE_DIR}/CEGUIVersion.h CEGUIVERSION REGEX CEGUI_VERSION)
string(REGEX MATCH "CEGUI_VERSION_MAJOR ([0-9]+)" _tmp ${CEGUIVERSION})
set(CEGUI_VERSION_MAJOR ${CMAKE_MATCH_1})
string(REGEX MATCH "CEGUI_VERSION_MINOR ([0-9]+)" _tmp ${CEGUIVERSION})
set(CEGUI_VERSION_MINOR ${CMAKE_MATCH_1})
string(REGEX MATCH "CEGUI_VERSION_PATCH ([0-9]+)" _tmp ${CEGUIVERSION})
set(CEGUI_VERSION_PATCH ${CMAKE_MATCH_1})
set(CEGUI_VERSION "${CEGUI_VERSION_MAJOR}.${CEGUI_VERSION_MINOR}.${CEGUI_VERSION_PATCH}")

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(CEGUI
	REQUIRED_VARS CEGUI_LIBRARIES CEGUI_INCLUDE_DIRS
	VERSION_VAR CEGUI_VERSION
)
