# Simple-Logger
A very basic logger component that I use in my SNES Emulator project. It was somewhat inspired by the Android Log class.
It is really very simple and made for one purpose only, make logging of low level details about the emulator easy, I didn't feel like spending a lot of time learning how to use other existing loggers so I made this.

### Example 1
```c++
#include "Log.hpp"
#define LOG_TAG "MYTAG"
...
Log::dbg(LOG_TAG).str("Cpu is ready to run").show();
```
```
MYTAG: Cpu is ready to run
```
### Example 2
```c++
#include "Log.hpp"
#define LOG_TAG "MYTAG"
...
Log::dbg(LOG_TAG).str("Emulation mode RST vector at").sp().hex(someValue(), 4).show();
```
```
MYTAG: Emulation mode RST vector at 0x1234
```
### Example 3
```c++
#include "Log.hpp"
#define LOG_TAG "MYTAG"
...
Log::trc(LOG_TAG).str("DB: ").hex(mCpu.mDB, 2).sp().str("D: ").hex(mCpu.mD, 4).show();
```
```
MYTAG: DB: 0x12 D: 0x1234
```
It supports some more stuff, have a look at the code.
