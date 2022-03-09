cmake_minimum_required (VERSION 2.6)
project (SimpleLogger)

if (CMAKE_VERSION VERSION_LESS "3.1")
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  set (CMAKE_CXX_FLAGS "--std=gnu++14 ${CMAKE_CXX_FLAGS}")
endif ()
else ()
    set (CMAKE_CXX_STANDARD 14)
endif ()

# All warnings on
set (CMAKE_CXX_FLAGS "-Wall ${CMAKE_CXX_FLAGS}")

file(GLOB root_src *.cpp)
add_library(SimpleLogger STATIC ${root_src})
