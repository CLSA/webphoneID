cmake_minimum_required( VERSION 3.16.0 )

project( webphoneID VERSION 1.0.0 LANGUAGES CXX )

set( CMAKE_CXX_STANDARD 11 )
set( CMAKE_CXX_STANDARD_REQUIRED ON )

set( CMAKE_AUTOMOC ON )
set( CMAKE_AUTORCC ON )
set( CMAKE_AUTOUIC ON )

# Look in the build directory for cmake modules
SET( CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/" )

option(WITH_DEBUG "Use qDebug in source for debugging" OFF)

option(BUILD_SHARED_LIBS "Build with shared libs" ON)

find_package( Qt5 COMPONENTS Widgets REQUIRED )

set(CMAKE_FIND_PACKAGE_PREFER_CONFIG TRUE)

find_package( CURL REQUIRED )

if(WIN32)
add_executable( webphoneID
  IDEntryDialog.ui
  IDEntryDialog.cpp
  main.cpp
  webphoneID.rc
)
else()
add_executable( webphoneID
  IDEntryDialog.ui
  IDEntryDialog.cpp
  main.cpp
  webphoneID.qrc
)
endif()



file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/webphone.jar
     DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/settings.ini
     DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

if(WITH_DEBUG)
  target_compile_definitions(webphoneID PRIVATE -DWITH_QDEBUG)
endif()

target_link_libraries( webphoneID PRIVATE Qt5::Widgets CURL::libcurl )

install(TARGETS webphoneID RUNTIME DESTINATION bin)
