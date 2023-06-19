#include "Log.h"
#include "Ram.h"
#include "Rom.h"
#include "Via.h"
#include "Uart.h"
#include "TerminalWrapper.h"
#include "SpeedLimit.h"
#include "DebugSymbols.h"

#include <boost/program_options.hpp>
#include <SystemBus.h>
#include <Cpu65816.h>
#include <Cpu65816Debugger.h>

#define LOG_TAG "MAIN"

std::shared_ptr<TerminalWrapper> getTerminalWrapper(bool mode);

namespace po = boost::program_options;

int executeTestRoutine(Cpu65816& cpu, Cpu65816Debugger &debugger, const Address &address, bool &breakPointHit) {
    breakPointHit = false; // reset
    // Track stack pointer so we know when this returns
    uint16_t startOffset = cpu.getStack()->getStackPointer();
    // Imitate jsr instruction
    cpu.getStack()->push16Bit(0x0000);
    cpu.setProgramAddress(address);
    while (cpu.getStack()->getStackPointer() < startOffset && !breakPointHit) {
        debugger.step();
    }
    if(breakPointHit) {
        return -1; // Test failure
    }
    // Return A register when subroutine returns
    return cpu.getA();
}

int executeTestMode(Cpu65816 &cpu, Cpu65816Debugger &debugger, DebugSymbols symbols) {
    bool breakPointHit = false;
    debugger.doBeforeStep([]() {});
    debugger.doAfterStep([]() {});
    debugger.onBreakPoint([&breakPointHit]() {
        breakPointHit = true;
    });
    debugger.onStp([&cpu, &breakPointHit]() {
        breakPointHit = true;
    });
    const int testWidth = 45;
    // Run test setup first if it is defined
    if(symbols.labels.contains("test_setup")) {
        Address testSetupRoutine = symbols.labels["test_setup"];
        std::cerr << std::left << std::setw(testWidth) << "test_setup";
        uint16_t setupResult = executeTestRoutine(cpu, debugger, testSetupRoutine, breakPointHit);
        if(setupResult != 0) {
            std::cerr << " [ FAIL ]" << std::endl;
            return 1;
        }
        std::cerr << " [  OK  ]" << std::endl << std::endl;
    }
    // Iterate labels, run any that look like test routines
    int testsExecuted = 0;
    int testsPassed = 0;
    int testsFailed = 0;
    for (auto & itr : symbols.labels) {
        auto label = itr.first;
        if(label == "test_setup" || !label.starts_with("test_")) {
            // Not a test
            continue;
        }
        std::cerr << std::left << std::setw(testWidth) << label;
        // Run the test
        int testResult = executeTestRoutine(cpu, debugger, itr.second, breakPointHit);
        // Record result
        testsExecuted++;
        if(testResult == 0) {
            testsPassed++;
            std::cerr << " [ PASS ]" << std::endl;
        } else if (testResult == -1) {
            testsFailed++;
            std::cerr << " [ ERROR ]" << std::endl;
        } else {
            testsFailed++;
            std::cerr << " [ FAIL ]" << std::endl;
        }
    }
    std::cerr << std::endl << testsExecuted << " tests executed. " << testsPassed << " passed, " << testsFailed
              << " failed/errored." << std::endl;
    return testsFailed;
}

int executeFromReset(Cpu65816& cpu, Cpu65816Debugger& debugger) {
    // Run full system from reset.
    Log::vrb(LOG_TAG).str("+++ Program starting +++").show();
    debugger.setBreakPoint(Address(0x00, 0x0005));
    debugger.doBeforeStep([]() {});
    debugger.doAfterStep([]() {});

    bool breakPointHit = false;
    debugger.onBreakPoint([&breakPointHit]() {
        breakPointHit = true;
    });
    debugger.onStp([&cpu, &breakPointHit]() {
        // Trigger clean exit when STP is used.
        breakPointHit = true;
    });
    SpeedLimit speedLimit(4000000);
    while (!breakPointHit) {
        debugger.step();
        speedLimit.apply(cpu.getTotalCyclesCounter());
    }
    debugger.dumpCpu();
    Log::vrb(LOG_TAG).str("+++ Program completed +++").show();
    return cpu.getA();
}

int main(int argc, char **argv) {
    std::string romPath;
    std::string kernelPath;
    std::string testPath;
    bool traceLog = false;
    bool testMode;
    // Parse command-line options
    try {
        po::options_description desc{"Options"};
        desc.add_options()
                ("help,h", "Show help")
                ("rom", po::value(&romPath),
                 "Set ROM image, 8192 byte file to be loaded at $00e000. This option is required unless a test image is specified.")
                ("kernel", po::value(&kernelPath),
                 "Set kernel image, 8192 byte file to be loaded at $010000. If this option is used, ROM must be set. SD card emulation is not implemented, so this is intended to be used with a ROM image which has been modified to boot directly into the kernel code instead of attempting to load it from disk.")
                ("test", po::value(&testPath),
                 "Set test image, 8192 byte file to be loaded into RAM at $00e000. This is a different execution mode, intended for running unit tests. No ROM or Kernel image is required, and the machine will not emulate a proper console. Instead, the emulator will load a labels file, execute the test setup subroutine ('test_setup'), then execute each test subroutine (labels beginning with 'test_'). If a test is successful, it should return with the value0 in the accumulator, any other value indicates test failure.")
                ("trace", po::bool_switch(&traceLog), "Enable detailed trace-level logging.");
        po::variables_map vm;
        store(parse_command_line(argc, argv, desc), vm);
        po::notify(vm);
        if (vm.count("help")) {
            // Show help
            std::cerr << desc << "\n";
            return 0;
        }
        if(traceLog) {
            Log::trc(LOG_TAG).enable();
        }
        // Error if we get incompatible options
        if(romPath.empty() && testPath.empty()) {
            std::cerr << "Either a ROM or test image must be specified." << '\n';
            return 1;
        }
        testMode = !testPath.empty();
    } catch (const boost::program_options::error &ex) {
        // Bad option
        std::cerr << ex.what() << '\n';
        return 1;
    }

    // Create the system
    Ram ram = Ram(16);
    Via via = Via();
    TerminalWrapper terminalWrapper = TerminalWrapper(testMode);
    Uart uart = Uart(&terminalWrapper);

    SystemBus systemBus = SystemBus();
    systemBus.registerDevice(&via);
    systemBus.registerDevice(&uart);

    // Always initialise this, though we don't always use it
    DebugSymbols symbols = DebugSymbols();
    Rom rom = Rom(testMode ? testPath : romPath);
    if(testMode) {
        // Load code into RAM in test mode, so we can test eg. self-modifying code
        ram.loadFromFile(testPath, Address(0x00, 0xe000), 8192);
        symbols.loadSymbolsForBinary(testPath);
    } else {
        systemBus.registerDevice(&rom);
        symbols.loadSymbolsForBinary(romPath);
    }
    if (!kernelPath.empty()) {
        ram.loadFromFile(kernelPath, Address(0x01, 0x0000), 8192);
        symbols.loadSymbolsForBinary(kernelPath);
    }
    systemBus.registerDevice(&ram);

    auto nmi = via.getIrq();
    auto irq = std::make_shared<InterruptStatus>();

    Cpu65816 cpu(systemBus, nmi, irq);
    Cpu65816Debugger debugger(cpu, symbols, traceLog);

    if(testMode) {
        return executeTestMode(cpu, debugger, symbols);
    } else {
        return executeFromReset(cpu, debugger);
    }

}
