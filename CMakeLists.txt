# Copyright by Michal Majczak & Krzysztof Taperek, 2016
# Licensed under the MIT license:
# http://www.opensource.org/licenses/mit-license.php
#
# Author: Michal Majczak <michal.majczak92@gmail.com>

cmake_minimum_required (VERSION 2.8)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR})

##########################################
## Project info
##########################################

set( PROJECT_NAME deferred-renderer-demo )
set( TARGET_NAME "${PROJECT_NAME}.out" )
project( ${PROJECT_NAME} )

##########################################
## EXE info
##########################################
add_definitions( -std=c++14 )
add_definitions( -O2 )

file(GLOB_RECURSE TARGET_SRC src/*.cpp)
file(GLOB_RECURSE TARGET_HEADERS src/*.h src/*.hpp )

include_directories( src/ )

# compiler specific flags
if( CMAKE_CXX_COMPILER_ID MATCHES "Clang" )
    # using clang
	add_definitions( -Wall  -Wextra )
elseif( CMAKE_CXX_COMPILER_ID MATCHES "GNU" )
    # using gcc
	add_definitions( -Wall  -Wextra )
elseif( CMAKE_CXX_COMPILER_ID MATCHES "MSVC" )
    # using msvc
endif()

# Windows workarounds
if( WIN32 )
  # location of master lib folder
  set( WIN_LIB_DIR "C:/libs" )
  # lib locations
  set( CMAKE_PREFIX_PATH "${WIN_LIB_DIR}/glew")
  set( GLM_ROOT_DIR "${WIN_LIB_DIR}/glm")
  set( ENV{SDL2DIR} "${WIN_LIB_DIR}/SDL2")
  set( ASSIMP_ROOT_DIR "${WIN_LIB_DIR}/assimp")
  set( GLI_ROOT "${WIN_LIB_DIR}/GLI")
  # libraries needed when linking SDL2 statically
  set( LIBRARIES "version.lib;imm32.lib;winmm.lib;")
endif( WIN32 )

##########################################
## Linking dynamic libraries
##########################################

# Find SDL2
find_package(SDL2 REQUIRED)
if( SDL2_FOUND )
  include_directories( ${SDL2_INCLUDE_DIR} )
  set( LIBRARIES ${LIBRARIES} ${SDL2_LIBRARY} )
endif( SDL2_FOUND )

# Find GLEW
set(GLEW_USE_STATIC_BUILDS ON)
find_package(GLEW REQUIRED)
if (GLEW_FOUND)
    include_directories( ${GLEW_INCLUDE_DIRS} )
    set( LIBRARIES ${LIBRARIES} ${GLEW_LIBRARIES} )
endif(GLEW_FOUND)

# Find GLM
find_package(GLM REQUIRED)
if( GLM_FOUND )
  include_directories( ${GLM_INCLUDE_DIRS} )
  set( LIBRARIES ${LIBRARIES} ${GLM_LIBRARIES} )
endif( GLM_FOUND )

# Find OpenGL
find_package( OpenGL 3.3 REQUIRED )
if( OPENGL_FOUND )
  set( LIBRARIES ${LIBRARIES} ${OPENGL_LIBRARIES} )
  include_directories( ${OPENGL_INCLUDE_DIR} )
endif( OPENGL_FOUND )

# Find assimp
find_package( ASSIMP REQUIRED )
if( ASSIMP_FOUND )
  set( LIBRARIES ${LIBRARIES} ${ASSIMP_LIBRARIES} )
  include_directories( ${ASSIMP_INCLUDE_DIR} )
endif( ASSIMP_FOUND )

# Find GLI
find_package( GLI REQUIRED )
if( GLI_FOUND )
  include_directories( ${GLI_INCLUDE_DIR} )
endif( GLI_FOUND )

##########################################
## Add Executable
##########################################

add_executable(${TARGET_NAME} ${TARGET_SRC} ${TARGET_HEADERS})
target_link_libraries(${TARGET_NAME} ${LIBRARIES})
