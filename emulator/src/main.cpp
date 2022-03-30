#include "Log.h"
#include "Ram.h"
#include "Rom.h"
#include "Via.h"
#include "Uart.h"
#include "TerminalWrapper.h"

#include <SystemBus.h>
#include <Cpu65816.h>
#include <Cpu65816Debugger.h>

#define LOG_TAG "MAIN"

int main(int argc, char **argv) {
    Log::vrb(LOG_TAG).str("+++ Lib65816 Sample Programs +++").show();

    Ram ram = Ram(16);
    Rom rom = Rom("../rom/rom.bin");
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

    while (!breakPointHit) {
        debugger.step();
    }

    debugger.dumpCpu();

    Log::vrb(LOG_TAG).str("+++ Program completed +++").show();
}
