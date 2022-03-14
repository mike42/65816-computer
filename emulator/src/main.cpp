#include "Log.h"
#include "Ram.h"
#include "Rom.h"
#include "Via.h"

#include <Interrupt.h>
#include <SystemBus.h>
#include <Cpu65816.h>
#include <Cpu65816Debugger.h>

#define LOG_TAG "MAIN"

int main(int argc, char **argv) {
    Log::vrb(LOG_TAG).str("+++ Lib65816 Sample Programs +++").show();

    Ram ram = Ram(16);
    ram.storeByte(Address(0x00, 0x0000), 0x18);
    ram.storeByte(Address(0x00, 0x0001), 0xFB);
    ram.storeByte(Address(0x00, 0x0002), 0xA9);
    ram.storeByte(Address(0x00, 0x0003), 0x65);
    ram.storeByte(Address(0x00, 0x0004), 0x12);

    Rom rom = Rom("../rom/rom.bin");

    Via via = Via();

    SystemBus systemBus = SystemBus();
    systemBus.registerDevice(&via);
    systemBus.registerDevice(&rom);
    systemBus.registerDevice(&ram);

    EmulationModeInterrupts emulationInterrupts = rom.getEmulationModeInterrupts();
    NativeModeInterrupts nativeInterrupts = rom.getNativeModeInterrupts();
    Cpu65816 cpu(systemBus, &emulationInterrupts, &nativeInterrupts);
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
