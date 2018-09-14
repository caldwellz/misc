
set(INCLUDE_PrimeSift CACHE BOOL OFF)
if(INCLUDE_PrimeSift)
	set(SOURCE_FILES
		${SOURCE_FILES}

		${CMAKE_CURRENT_SOURCE_DIR}/primesift.c
		${CMAKE_CURRENT_SOURCE_DIR}/primesift.h
		${CMAKE_CURRENT_SOURCE_DIR}/PrimeSift-dlg.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/PrimeSift-dlg.hpp

		CACHE INTERNAL ""
	)
endif()
