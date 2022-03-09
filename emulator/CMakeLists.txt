cmake_minimum_required(VERSION 3.14)

set(CMAKE_CXX_STANDARD 14)

project(Emulator
        LANGUAGES CXX)

add_executable(emulator
        src/lib65816/Addressing.cpp
        src/lib65816/Binary.cpp
        src/lib65816/Cpu65816.cpp
        src/lib65816/Cpu65816Debugger.cpp
        src/lib65816/CpuStatus.cpp
        src/lib65816/opcodes/OpCode_ADC.cpp
        src/lib65816/opcodes/OpCode_AND.cpp
        src/lib65816/opcodes/OpCode_ASL.cpp
        src/lib65816/opcodes/OpCode_BIT.cpp
        src/lib65816/opcodes/OpCode_Branch.cpp
        src/lib65816/opcodes/OpCode_CMP.cpp
        src/lib65816/opcodes/OpCode_CPX_CPY.cpp
        src/lib65816/opcodes/OpCode_EOR.cpp
        src/lib65816/opcodes/OpCode_INC_DEC.cpp
        src/lib65816/opcodes/OpCode_Interrupt.cpp
        src/lib65816/opcodes/OpCode_JumpReturn.cpp
        src/lib65816/opcodes/OpCode_LDA.cpp
        src/lib65816/opcodes/OpCode_LDX.cpp
        src/lib65816/opcodes/OpCode_LDY.cpp
        src/lib65816/opcodes/OpCode_LSR.cpp
        src/lib65816/opcodes/OpCode_Misc.cpp
        src/lib65816/opcodes/OpCode_ORA.cpp
        src/lib65816/opcodes/OpCode_ROL.cpp
        src/lib65816/opcodes/OpCode_ROR.cpp
        src/lib65816/opcodes/OpCode_SBC.cpp
        src/lib65816/opcodes/OpCode_Stack.cpp
        src/lib65816/opcodes/OpCode_STA.cpp
        src/lib65816/opcodes/OpCode_StatusReg.cpp
        src/lib65816/opcodes/OpCode_STX.cpp
        src/lib65816/opcodes/OpCode_STY.cpp
        src/lib65816/opcodes/OpCode_STZ.cpp
        src/lib65816/opcodes/OpCodeTable.cpp
        src/lib65816/opcodes/OpCode_Transfer.cpp
        src/lib65816/opcodes/OpCode_TSB_TRB.cpp
        src/lib65816/Stack.cpp
        src/lib65816/SystemBus.cpp
        src/lib65816/SystemBusDevice.cpp
        src/logging/Log.cpp
        src/main.cpp
        src/Ram.cpp
        )

target_include_directories(emulator PRIVATE
        include/
        include/logging
        include/lib65816)


#
#cmake_minimum_required (VERSION 2.6)
#project (65816)
#
#include(ExternalProject)
#
#ExternalProject_Add(
#        simple-logger
#        PREFIX "${PROJECT_SOURCE_DIR}/simple-logger"
#        GIT_REPOSITORY https://github.com/FrancescoRigoni/Simple-Logger.git
#        INSTALL_COMMAND "")
#
## Retrieve simple-logger sources location
#ExternalProject_Get_Property(simple-logger source_dir)
#set(SIMPLE_LOGGER_SRC ${source_dir})
#
## Retrieve simple-logger binaries location
#ExternalProject_Get_Property(simple-logger binary_dir)
#set(SIMPLE_LOGGER_BIN ${binary_dir})
#
#if (CMAKE_VERSION VERSION_LESS "3.1")
#    if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
#        set (CMAKE_CXX_FLAGS "--std=gnu++14 ${CMAKE_CXX_FLAGS}")
#    endif ()
#else ()
#    set (CMAKE_CXX_STANDARD 14)
#endif ()
#
## Add paths for linker
#link_directories(${SIMPLE_LOGGER_BIN})
#
## All warnings on
#set (CMAKE_CXX_FLAGS "-Wall ${CMAKE_CXX_FLAGS}")
#file(GLOB sources_65816 src/*.cpp src/opcodes/*.cpp)
#
#add_library(65816 ${sources_65816})
#add_dependencies(65816 simple-logger)
#target_link_libraries(65816 simpleLogger)
#target_include_directories(65816 PRIVATE ${PROJECT_SOURCE_DIR}/include ${SIMPLE_LOGGER_SRC})
