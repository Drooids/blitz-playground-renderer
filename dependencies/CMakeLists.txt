execute_process( COMMAND git submodule update --init --recursive )

set_directory_properties( PROPERTIES CLEAN_NO_CUSTOM 1)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/whereami/src/)

IF (WIN32)
add_library (whereami STATIC ${CMAKE_CURRENT_SOURCE_DIR}/whereami/src/whereami.c ${CMAKE_CURRENT_SOURCE_DIR}/whereami/src/whereami.h)
ELSE ()
add_library (whereami SHARED ${CMAKE_CURRENT_SOURCE_DIR}/whereami/src/whereami.c ${CMAKE_CURRENT_SOURCE_DIR}/whereami/src/whereami.h)
ENDIF ()

set( SDL_STATIC OFF CACHE BOOL "Build the static SDL library" )
set( SDL_SHARED ON CACHE BOOL "Build the shared SDL library" )

# add_subdirectory( SDL )
