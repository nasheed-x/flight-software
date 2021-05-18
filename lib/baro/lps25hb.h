#ifndef LPS25HB_H
#define LPS25HB_H

#include <scheduler.h>
#include <SPI.h>
#include <LPS25HBSensor.h>
#include <Arduino.h>
#include "chip.h"

class Barometer : public Task, public Chip
{
private:
    LPS25HBSensor *driver;

public:
    Barometer(SPIClass *lps_spi, int LPS_CS);
    ~Barometer();

    // Task virtual methods
    bool Callback();

    // Chip virtual methods
    bool checkStatus();
};

#endif