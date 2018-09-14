# - Find OgreOggSound
# Find the OgreOggSound includes and libraries
#
# Following variables are provided:
# OGREOGGSOUND_FOUND
#     True if OgreOggSound has been found
# OGREOGGSOUND_INCLUDE_DIR
#     The include directory of OgreOggSound
# OGREOGGSOUND_LIBRARY
#     OgreOggSound library


# Find include directory and library
find_path(OGREOGGSOUND_INCLUDE_DIR NAMES OgreOggSound.h
    PATH_SUFFIXES include OgreOggSound)

find_library(OGREOGGSOUND_LIBRARY NAMES OgreOggSound)

# Determine OgreOggSound version
file(STRINGS ${OGREOGGSOUND_INCLUDE_DIR}/OgreOggSound.h OGREOGGSOUND_H_VER REGEX @version)
string(REGEX MATCH "@version v([0-9]+.[0-9]+)" _tmp ${OGREOGGSOUND_H_VER})
set(OGREOGGSOUND_VERSION "${CMAKE_MATCH_1}")

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(OgreOggSound
    REQUIRED_VARS OGREOGGSOUND_LIBRARY OGREOGGSOUND_INCLUDE_DIR
    VERSION_VAR OGREOGGSOUND_VERSION)
