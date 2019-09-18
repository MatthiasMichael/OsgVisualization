CMAKE_MINIMUM_REQUIRED(VERSION 3.0)

PROJECT( OsgVisualization_Tests )

ADD_CUSTOM_TARGET( OsgVisualization_Test_Common SOURCES
	common/osgCommonObjects_test.h
)
SET_PROPERTY( TARGET OsgVisualization_Test_Common PROPERTY FOLDER "Tests")

IF( BUILD_EXAMPLE_DISPLAYABLE )
	ADD_SUBDIRECTORY( test_displayable/cmake ${CMAKE_CURRENT_BINARY_DIR}/test_displayable)
ENDIF( )

IF( BUILD_EXAMPLE_DISPLAY_DESCRIPTOR )
	ADD_SUBDIRECTORY( test_displayDescriptor/cmake ${CMAKE_CURRENT_BINARY_DIR}/test_display_descriptor)
ENDIF( )