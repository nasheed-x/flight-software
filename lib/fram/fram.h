#ifndef MB85_FRAM_H
#define MB85_FRAM_H

#include <Arduino.h>
#include <SPI.h>
#include "chip.h"
#include "scheduler.h"

class FRAM : public Chip, public Task
{
private:
    MB85_FRAM_Class *driver;
    long measurement_delay;
    long previous_time = 0;

public:
    FRAM(uint8_t FRAM_CS, long measurement_delay);
    ~FRAM();

    bool measurementReady();

    // Task virtual methods
    bool Callback();
    bool OnEnable();
    void OnDisable();

    // Chip virtual method
    bool checkStatus();
};

#endif