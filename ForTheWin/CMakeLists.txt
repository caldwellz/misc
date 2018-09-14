cmake_minimum_required(VERSION 2.8)
project(ForTheWin)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR}/cmake)

if(NOT CMAKE_BUILD_TYPE)
	message(STATUS "No build type selected, defaulting to Release")
	set(CMAKE_BUILD_TYPE "Release")
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
	set(DEBUG ON)
else()
	set(NDEBUG ON)
endif()

set(SOURCE_FILES
	${CMAKE_CURRENT_SOURCE_DIR}/Res/ForTheWin.rc
	${CMAKE_CURRENT_SOURCE_DIR}/WinMain.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Chooser.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Chooser.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Module.hpp

	CACHE INTERNAL ""
)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
add_subdirectory(Modules)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/IncludedModules.in ${CMAKE_CURRENT_SOURCE_DIR}/IncludedModules.hpp)

add_executable(ForTheWin ${SOURCE_FILES})
set_target_properties(ForTheWin PROPERTIES WIN32_EXECUTABLE ON)
install(TARGETS ForTheWin RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX})
