cmake_minimum_required(VERSION 2.8)
project(Overmind)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR}/cmake)

if(NOT CMAKE_BUILD_TYPE)
	message(STATUS "No build type selected, defaulting to Debug")
	set(CMAKE_BUILD_TYPE "Debug")
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
	set(DEBUG ON)
else()
	set(NDEBUG ON)
endif()

set(MAJOR_VERSION 0)
set(MINOR_VERSION 0)
set(PATCH_VERSION 1)
set(VERSION_STRING "${MAJOR_VERSION}.${MINOR_VERSION}.${PATCH_VERSION}")

add_subdirectory(Doc)
add_subdirectory(Source)
