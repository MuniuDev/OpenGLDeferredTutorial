# - Try to find Assimp
# Once done, this will define
#
#  ASSIMP_FOUND - system has Assimp
#  ASSIMP_INCLUDE_DIR - the Assimp include directories
#  ASSIMP_LIBRARIES - link these to use Assimp

FIND_PATH( ASSIMP_INCLUDE_DIR assimp/mesh.h
  ${ASSIMP_ROOT_DIR}/include
  /usr/include
  /usr/local/include
  /opt/local/include
)

FIND_LIBRARY( ASSIMP_LIBRARY assimp
  ${ASSIMP_ROOT_DIR}/lib
  /usr/lib64
  /usr/lib
  /usr/local/lib
  /opt/local/lib
)

# fix for static linking of assimp in windows
IF( WIN32 )
  FIND_LIBRARY( ZLIB_LIBRARY zlibstatic
    ${ASSIMP_ROOT_DIR}/lib
    /usr/lib64
    /usr/lib
    /usr/local/lib
    /opt/local/lib
  )

  IF(ASSIMP_INCLUDE_DIR AND ASSIMP_LIBRARY AND ZLIB_LIBRARY)
    SET( ASSIMP_FOUND TRUE )
    SET( ASSIMP_LIBRARIES ${ZLIB_LIBRARY} )
  ENDIF(ASSIMP_INCLUDE_DIR AND ASSIMP_LIBRARY AND ZLIB_LIBRARY)
ENDIF( WIN32 )

IF(ASSIMP_INCLUDE_DIR AND ASSIMP_LIBRARY)
  SET( ASSIMP_FOUND TRUE )
  SET( ASSIMP_LIBRARIES ${ASSIMP_LIBRARIES} ${ASSIMP_LIBRARY} )
ENDIF(ASSIMP_INCLUDE_DIR AND ASSIMP_LIBRARY)

IF(ASSIMP_FOUND)
   IF(NOT ASSIMP_FIND_QUIETLY)
      MESSAGE(STATUS "Found ASSIMP: ${ASSIMP_LIBRARY}")
   ENDIF(NOT ASSIMP_FIND_QUIETLY)
ELSE(ASSIMP_FOUND)
   IF(ASSIMP_FIND_REQUIRED)
      MESSAGE(FATAL_ERROR "Could not find libASSIMP")
   ENDIF(ASSIMP_FIND_REQUIRED)
ENDIF(ASSIMP_FOUND)
