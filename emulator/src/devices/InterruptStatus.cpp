#include "InterruptStatus.h"

InterruptStatus::InterruptStatus() {
    active = false;
}

bool InterruptStatus::get() {
    return active;
}

void InterruptStatus::set(bool val) {
    active = val;
}
