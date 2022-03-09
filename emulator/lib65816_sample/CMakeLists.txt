cmake_minimum_required (VERSION 2.6)
project (Lib65816_Sample)

include(ExternalProject)

ExternalProject_Add(
        lib65816
        PREFIX "${PROJECT_SOURCE_DIR}/lib65816"
        GIT_REPOSITORY https://github.com/FrancescoRigoni/Lib65816.git
        INSTALL_COMMAND "")

ExternalProject_Add(
        simple-logger
        PREFIX "${PROJECT_SOURCE_DIR}/simple-logger"
        GIT_REPOSITORY https://github.com/FrancescoRigoni/Simple-Logger.git
        INSTALL_COMMAND "")

# Retrieve lib65816 sources location
ExternalProject_Get_Property(lib65816 source_dir)
set(LIB_65816_SRC ${source_dir})

# Retrieve lib65816 binaries location
ExternalProject_Get_Property(lib65816 binary_dir)
set(LIB_65816_BIN ${binary_dir})

# Retrieve simple-logger sources location
ExternalProject_Get_Property(simple-logger source_dir)
set(SIMPLE_LOGGER_SRC ${source_dir})

# Retrieve simple-logger binaries location
ExternalProject_Get_Property(simple-logger binary_dir)
set(SIMPLE_LOGGER_BIN ${binary_dir})

# Add all include directories
include_directories(src/** ${LIB_65816_SRC}/.. ${SIMPLE_LOGGER_SRC})

set(LIBS ${SIMPLE_LOGGER_BIN}/libSimpleLogger.a ${LIB_65816_BIN}/lib65816.a)
message(${LIBS})

if (CMAKE_VERSION VERSION_LESS "3.1")
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  set (CMAKE_CXX_FLAGS "--std=gnu++14 ${CMAKE_CXX_FLAGS}")
endif ()
else ()
    set (CMAKE_CXX_STANDARD 14)
endif ()

# All warnings on
set (CMAKE_CXX_FLAGS "-Wall ${CMAKE_CXX_FLAGS}")
file(GLOB sources src/*.cpp)

add_executable(Lib65816_Sample ${sources})
add_dependencies(Lib65816_Sample lib65816)
add_dependencies(Lib65816_Sample simple-logger)
target_link_libraries(Lib65816_Sample ${LIBS})

