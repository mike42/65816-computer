#include "Log.h"
#include "Ram.h"
#include "Rom.h"
#include "Via.h"
#include "Uart.h"
#include "TerminalWrapper.h"
#include "SpeedLimit.h"

#include <SystemBus.h>
#include <Cpu65816.h>
#include <Cpu65816Debugger.h>

#define LOG_TAG "MAIN"

int main(int argc, char **argv) {
    Log::vrb(LOG_TAG).str("+++ Program starting +++").show();

    Ram ram = Ram(16);
    std::string romFilename = argc > 1 ? argv[1] : "../rom/rom.bin";
    Rom rom = Rom(romFilename);
    if(argc > 2) {
        std::string ramfsFilename = argv[2];
        ram.loadFromFile(ramfsFilename, Address(0x08, 0x000000), 524288);
    }
    Via via = Via();
    TerminalWrapper terminalWrapper = TerminalWrapper();
    Uart uart = Uart(&terminalWrapper);

    SystemBus systemBus = SystemBus();
    systemBus.registerDevice(&via);
    systemBus.registerDevice(&uart);
    systemBus.registerDevice(&rom);
    systemBus.registerDevice(&ram);

    auto nmi = via.getIrq();
    auto irq = std::make_shared<InterruptStatus>();

    Cpu65816 cpu(systemBus, nmi, irq);
    Cpu65816Debugger debugger(cpu);
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
