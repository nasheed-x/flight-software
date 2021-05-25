#ifndef RFM69_H
#define RFM69_H

#include <scheduler.h>
#include <RH_RF69.h>
#include <SPI.h>
#include <Arduino.h>
#include "chip.h"

class Transceiver : public Chip
{
private:
    RH_RF69 *driver;

public:
    Transceiver();
    ~Transceiver();

    // Chip virtual methods
    bool checkStatus();
};

#endif