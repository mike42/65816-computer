cmake_minimum_required (VERSION 2.6)
project (65816)

include(ExternalProject)

ExternalProject_Add(
        simple-logger
        PREFIX "${PROJECT_SOURCE_DIR}/simple-logger"
        GIT_REPOSITORY https://github.com/FrancescoRigoni/Simple-Logger.git
        INSTALL_COMMAND "")

# Retrieve simple-logger sources location
ExternalProject_Get_Property(simple-logger source_dir)
set(SIMPLE_LOGGER_SRC ${source_dir})

# Retrieve simple-logger binaries location
ExternalProject_Get_Property(simple-logger binary_dir)
set(SIMPLE_LOGGER_BIN ${binary_dir})

if (CMAKE_VERSION VERSION_LESS "3.1")
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  set (CMAKE_CXX_FLAGS "--std=gnu++14 ${CMAKE_CXX_FLAGS}")
endif ()
else ()
    set (CMAKE_CXX_STANDARD 14)
endif ()

# Add paths for linker
link_directories(${SIMPLE_LOGGER_BIN})

# All warnings on
set (CMAKE_CXX_FLAGS "-Wall ${CMAKE_CXX_FLAGS}")
file(GLOB sources_65816 src/*.cpp src/opcodes/*.cpp)

add_library(65816 ${sources_65816})
add_dependencies(65816 simple-logger)
target_link_libraries(65816 simpleLogger)
target_include_directories(65816 PRIVATE ${PROJECT_SOURCE_DIR}/include ${SIMPLE_LOGGER_SRC})
