cmake_minimum_required(VERSION 3.14)

set(CMAKE_CXX_STANDARD 17)

project(Emulator
        LANGUAGES CXX)

enable_testing()

find_package(GTest REQUIRED)
find_package(Boost 1.79 COMPONENTS program_options)

add_library(cpu65816
        src/cpu/Addressing.cpp
        src/cpu/Binary.cpp
        src/cpu/Cpu65816.cpp
        src/cpu/Cpu65816Debugger.cpp
        src/cpu/CpuStatus.cpp
        src/cpu/opcodes/OpCode_ADC.cpp
        src/cpu/opcodes/OpCode_AND.cpp
        src/cpu/opcodes/OpCode_ASL.cpp
        src/cpu/opcodes/OpCode_BIT.cpp
        src/cpu/opcodes/OpCode_Branch.cpp
        src/cpu/opcodes/OpCode_CMP.cpp
        src/cpu/opcodes/OpCode_CPX_CPY.cpp
        src/cpu/opcodes/OpCode_EOR.cpp
        src/cpu/opcodes/OpCode_INC_DEC.cpp
        src/cpu/opcodes/OpCode_Interrupt.cpp
        src/cpu/opcodes/OpCode_JumpReturn.cpp
        src/cpu/opcodes/OpCode_LDA.cpp
        src/cpu/opcodes/OpCode_LDX.cpp
        src/cpu/opcodes/OpCode_LDY.cpp
        src/cpu/opcodes/OpCode_LSR.cpp
        src/cpu/opcodes/OpCode_Misc.cpp
        src/cpu/opcodes/OpCode_ORA.cpp
        src/cpu/opcodes/OpCode_ROL.cpp
        src/cpu/opcodes/OpCode_ROR.cpp
        src/cpu/opcodes/OpCode_SBC.cpp
        src/cpu/opcodes/OpCode_Stack.cpp
        src/cpu/opcodes/OpCode_STA.cpp
        src/cpu/opcodes/OpCode_StatusReg.cpp
        src/cpu/opcodes/OpCode_STX.cpp
        src/cpu/opcodes/OpCode_STY.cpp
        src/cpu/opcodes/OpCode_STZ.cpp
        src/cpu/opcodes/OpCodeTable.cpp
        src/cpu/opcodes/OpCode_Transfer.cpp
        src/cpu/opcodes/OpCode_TSB_TRB.cpp
        src/cpu/Stack.cpp
        src/cpu/SystemBus.cpp
        src/cpu/SystemBusDevice.cpp
        src/logging/Log.cpp
        src/devices/Ram.cpp
        src/devices/Rom.cpp
        src/devices/Via.cpp
        src/devices/InterruptStatus.cpp
        src/devices/Uart.cpp
        src/devices/TerminalWrapper.cpp
        src/cpu/SpeedLimit.cpp
        src/cpu/SpeedLimit.h)

add_executable(emulator
        src/main.cpp)

add_executable(emulator_test
        src/test.cpp)

include_directories(cpu65816 PRIVATE
        src/
        src/cpu
        src/devices
        src/logging
        ${Boost_INCLUDE_DIR}
        )

target_link_libraries(emulator
        cpu65816
        ${Boost_LIBRARIES})

target_include_directories(emulator_test PRIVATE
        ${GTest_INCLUDE_DIR})

target_link_libraries(emulator_test
        gtest
        gtest_main)

gtest_discover_tests(emulator_test)
