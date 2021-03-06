CMAKE_MINIMUM_REQUIRED( VERSION 3.10 )

SET_PROPERTY(GLOBAL PROPERTY USE_FOLDERS ON)

SET( BUILD_EXAMPLE_DISPLAYABLE OFF CACHE BOOL "Generate tests using OsgDisplayable and OsgGeometryManager" )
SET( BUILD_EXAMPLE_DISPLAY_DESCRIPTOR OFF CACHE BOOL "Generate tests using OsgDisplayDescriptor and OsgGenericVisualization" )

PROJECT( OsgVisualization )

FIND_PACKAGE( Qt5 COMPONENTS Core Gui OpenGL Widgets)

IF(EXISTS ${CMAKE_CURRENT_BINARY_DIR}/conanbuildinfo_multi.cmake)
    INCLUDE(${CMAKE_CURRENT_BINARY_DIR}/conanbuildinfo_multi.cmake)
ELSE()
    INCLUDE(${CMAKE_CURRENT_BINARY_DIR}/conanbuildinfo.cmake)
ENDIF()

CONAN_BASIC_SETUP(TARGETS)

SET( CMAKE_AUTOMOC ON )
SET( CMAKE_AUTOUIC ON )

SET( CMAKE_AUTOUIC_SEARCH_PATHS ../ui)

INCLUDE_DIRECTORIES(
	../include
)

SET( INCLUDE_FILES_GEOMETRY
	../include/CameraGeometry.h
	../include/GridGeometry.h
	../include/WallsGeometry.h
)

SET( INCLUDE_FILES_DISPLAYABLE 
	../include/OsgDisplayable.h
	../include/DisplayableAreaLights.h
	../include/DisplayableCameras.h
	../include/DisplayableCoordinateSystem.h
	../include/DisplayableGrid.h
	../include/DisplayableWalls.h
)

SET( INCLUDE_FILES_DISPLAY_DESCRIPTOR
	../include/AbstractOsgDisplayDescriptor.h
	../include/FunctionalOsgDisplayDescriptor.h
	../include/TypeErasedOsgDisplayDescriptor.h
)

SET( INCLUDE_FILES_UTILS
	../include/GeometryDisplayer.h
	../include/OsgGeometryManager.h
	../include/osg_stream_operators.h
	../include/osg_utils.h
)

SET( INCLUDE_FILES_WIDGETS
	../include/OsgDisplayDescriptorWidget.h
	../include/OsgGenericVisualizationWidget.h
	../include/QtOsgWidget.h
)

SET( INCLUDE_FILES 
	${INCLUDE_FILES_DISPLAYABLE}
	${INCLUDE_FILES_DISPLAY_DESCRIPTOR}
	${INCLUDE_FILES_GEOMETRY}
	${INCLUDE_FILES_UTILS}
	${INCLUDE_FILES_WIDGETS}
)

SET( SOURCE_FILES_GEOMETRY
	../src/CameraGeometry.cpp
	../src/GridGeometry.cpp
	../src/WallsGeometry.cpp
)

SET( SOURCE_FILES_DISPLAYABLE
	../src/OsgDisplayable.cpp
	../src/DisplayableAreaLights.cpp
	../src/DisplayableCameras.cpp
	../src/DisplayableCoordinateSystem.cpp
	../src/DisplayableGrid.cpp
	../src/DisplayableWalls.cpp
)

SET( SOURCE_FILES_DISPLAY_DESCRIPTOR
	../src/AbstractOsgDisplayDescriptor.cpp
)

SET( SOURCE_FILES_UTILS
	../src/OsgGeometryManager.cpp
	../src/osg_utils.cpp
)

SET( SOURCE_FILES_WIDGETS
	../src/OsgDisplayDescriptorWidget.cpp
	../src/OsgGenericVisualizationWidget.cpp
	../src/QtOsgWidget.cpp
)

SET( SOURCE_FILES 
	${SOURCE_FILES_DISPLAYABLE}
	${SOURCE_FILES_DISPLAY_DESCRIPTOR}
	${SOURCE_FILES_GEOMETRY}
	${SOURCE_FILES_UTILS}
	${SOURCE_FILES_WIDGETS}
)

SET( UI_FILES
	../ui/OsgDisplayDescriptorWidget.ui
	../ui/OsgGenericVisualizationWidget.ui
)

ADD_LIBRARY( ${PROJECT_NAME} STATIC
	${INCLUDE_FILES}
	${SOURCE_FILES}
	${UI_FILES}
)

TARGET_LINK_LIBRARIES( ${PROJECT_NAME}
	Qt5::Core
	Qt5::Gui
	Qt5::OpenGL
	Qt5::Widgets
	CONAN_PKG::Parametrizable
	CONAN_PKG::Roi3DF
	CONAN_PKG::openscenegraph
)

SOURCE_GROUP( "Header Files\\Displayable" FILES ${INCLUDE_FILES_DISPLAYABLE} )
SOURCE_GROUP( "Header Files\\Geometry" FILES ${INCLUDE_FILES_GEOMETRY} )
SOURCE_GROUP( "Header Files\\DisplayDescriptor" FILES ${INCLUDE_FILES_DISPLAY_DESCRIPTOR} )
SOURCE_GROUP( "Header Files\\Utils" FILES ${INCLUDE_FILES_UTILS} )
SOURCE_GROUP( "Header Files\\Widgets" FILES ${INCLUDE_FILES_WIDGETS} )

SOURCE_GROUP( "Source Files\\Displayable" FILES ${SOURCE_FILES_DISPLAYABLE} )
SOURCE_GROUP( "Source Files\\Geometry" FILES ${SOURCE_FILES_GEOMETRY} )
SOURCE_GROUP( "Source Files\\DisplayDescriptor" FILES ${SOURCE_FILES_DISPLAY_DESCRIPTOR} )
SOURCE_GROUP( "Source Files\\Utils" FILES ${SOURCE_FILES_UTILS} )
SOURCE_GROUP( "Source Files\\Widgets" FILES ${SOURCE_FILES_WIDGETS} )

SET_PROPERTY( TARGET ${PROJECT_NAME} PROPERTY FOLDER "Algorithms")

SET_PROPERTY( TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)
SET_PROPERTY( TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD_REQUIRED ON)

ADD_CUSTOM_COMMAND(TARGET ${PROJECT_NAME} POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory 
	${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}_autogen/include_$<CONFIG>
	${CMAKE_CURRENT_BINARY_DIR}/ui_include
)

SET( UI_INCLUDE_DIR "${CMAKE_CURRENT_BINARY_DIR}/ui_include")
FILE(MAKE_DIRECTORY "${UI_INCLUDE_DIR}")

IF( BUILD_EXAMPLE_DISPLAYABLE OR BUILD_EXAMPLE_DISPLAY_DESCRIPTOR )
	ADD_SUBDIRECTORY( ../test ${CMAKE_CURRENT_BINARY_DIR}/test )
ENDIF()