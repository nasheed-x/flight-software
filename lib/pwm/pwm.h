#ifndef PCA9635_H
#define PCA9635_H

#include <scheduler.h>
#include <Arduino.h>
#include "PCA9635.h"

class PWMControl : public Task
{
private:
    PCA9635 *driver;
    uint8_t channel;

public:
    PWMControl();
    ~PWMControl();

    void switchOn(uint8_t channel);
    void switchOff();

    bool Callback();
    bool OnEnable();
    void OnDisable();
};

#endif