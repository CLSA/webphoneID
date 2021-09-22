cmake_minimum_required( VERSION 3.16.0 )

project( webphoneID VERSION 1.0.0 LANGUAGES CXX )

set( CMAKE_CXX_STANDARD 11 )
set( CMAKE_CXX_STANDARD_REQUIRED ON )

set( CMAKE_AUTOMOC ON )
set( CMAKE_AUTORCC ON )
set( CMAKE_AUTOUIC ON )

# Look in the build directory for cmake modules
SET( CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/" )

find_package( Qt5 COMPONENTS Widgets REQUIRED )
find_package( CURL REQUIRED )

add_executable( webphoneID
  IDEntryDialog.ui
  IDEntryDialog.cpp
  main.cpp
)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/webphone.jar
     DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/settings.ini
     DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

target_link_libraries( webphoneID PRIVATE Qt5::Widgets curl )
