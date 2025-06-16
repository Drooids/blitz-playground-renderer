execute_process(COMMAND git submodule update --init --recursive)

set_directory_properties(PROPERTIES CLEAN_NO_CUSTOM 1)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/whereami/src/)

if(WIN32)
    add_library(whereami STATIC ${CMAKE_CURRENT_SOURCE_DIR}/whereami/src/whereami.c)
else()
    add_library(whereami SHARED ${CMAKE_CURRENT_SOURCE_DIR}/whereami/src/whereami.c)
endif()

# Explicitly set the language if needed
set_target_properties(whereami PROPERTIES LINKER_LANGUAGE C)

set(SDL_STATIC OFF CACHE BOOL "Build the static SDL library")
set(SDL_SHARED ON CACHE BOOL "Build the shared SDL library")

# add_subdirectory(SDL)
