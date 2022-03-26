#ifndef EMULATOR_INTERRUPTSTATUS_H
#define EMULATOR_INTERRUPTSTATUS_H

/*
 * Wrap active/inactive status of an interrupt, to allow CPU to poll.
 */
class InterruptStatus {
public:
    InterruptStatus();

    bool get();

    void set(bool val);

private:
    bool active;
};


#endif //EMULATOR_INTERRUPTSTATUS_H
