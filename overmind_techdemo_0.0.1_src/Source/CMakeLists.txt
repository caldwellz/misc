
# Offer the user the choice of overriding the installation directories
if(WIN32)
	set(INSTALL_BIN_DIR "" CACHE PATH "Installation subdirectory for game executable")
	set(INSTALL_DATA_DIR "" CACHE PATH "Installation subdirectory for data")
#elseif(APPLE) # I don't know where OSX likes to store things, so for now the UNIX case handles it
elseif(UNIX)
	set(INSTALL_BIN_DIR games CACHE PATH "Installation directory for game executable")
	set(INSTALL_DATA_DIR share/games/overmind CACHE PATH "Installation directory for data")
	set(INSTALL_APPMENU_ENTRIES ON CACHE BOOL "Install Linux application menu entries")
	if(INSTALL_APPMENU_ENTRIES)
		set(INSTALL_APPMENU_DIR share/applications CACHE PATH "Installation directory for .desktop file")
		set(INSTALL_APPMENU_PIXMAP_DIR share/pixmaps CACHE PATH "Installation directory for pixmap files")
	endif()
#else() # Most systems should fit one of the above cases, so I don't know what to do here...
endif()

# Find required libraries, using cmake/ scripts
find_package(POCO 1.4.3 REQUIRED)
find_package(OGRE 1.7.2 REQUIRED RenderSystem_GL)
find_package(OIS 1.2.0 REQUIRED)
find_package(CEGUI 0.7.6 REQUIRED)
find_package(OgreOggSound 1.21 REQUIRED)
include_directories(${POCO_INCLUDE_DIRS} ${OGRE_INCLUDE_DIRS} ${OIS_INCLUDE_DIRS} ${CEGUI_INCLUDE_DIRS} ${OGREOGGSOUND_INCLUDE_DIR})

# Configure the config file... that sounds funny :P
configure_file(OvermindConfig.h.in OvermindConfig.h)
include_directories(${CMAKE_CURRENT_BINARY_DIR})

# Set initial source file list
set(SOURCE_FILES
	${CMAKE_CURRENT_SOURCE_DIR}/Overmind.cpp

	CACHE INTERNAL ""
)

# Process source subdirectories
add_subdirectory(Engine)

# Compile and link
add_executable(overmind ${SOURCE_FILES})
target_link_libraries(overmind
	${POCO_LIBRARIES}
	${OGRE_LIBRARIES}
	${OIS_LIBRARIES}
	${CEGUI_LIBRARIES}
	${OGREOGGSOUND_LIBRARY}
)

# Install
install(TARGETS overmind RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/${INSTALL_BIN_DIR})
install(DIRECTORY ${CMAKE_SOURCE_DIR}/Data DESTINATION ${CMAKE_INSTALL_PREFIX}/${INSTALL_DATA_DIR})

# Install Linux application menu entries
if(INSTALL_APPMENU_ENTRIES)
	install(FILES ${CMAKE_SOURCE_DIR}/Data/overmind.desktop DESTINATION ${INSTALL_APPMENU_DIR})
	install(FILES ${CMAKE_SOURCE_DIR}/Data/overmind.png ${CMAKE_SOURCE_DIR}/Data/overmind.xpm DESTINATION ${INSTALL_APPMENU_PIXMAP_DIR})
endif()
