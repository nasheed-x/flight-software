#ifndef UBLOX_H
#define UBLOX_H

#include <scheduler.h>
#include <SparkFun_u-blox_GNSS_Arduino_Library.h>
#include <Arduino.h>
#include <SPI.h>
#include "chip.h"

class GPS : public Task, public Chip
{
private:
    SFE_UBLOX_GNSS *driver;
    long measurement_delay;
    long previous_time = 0;
    int32_t altitudeMSL;
    int32_t latitude;
    int32_t longitude;
    uint16_t year;
    uint8_t month;
    uint8_t day;
    uint8_t hour;
    uint8_t minute;
    uint8_t second;

public:
    GPS(long measurement_delay);
    ~GPS();

    bool measurementReady();

    int32_t getAltitude();
    int32_t getLatitude();
    int32_t getLongitude();

    uint16_t getYear();
    uint8_t getMonth();
    uint8_t getDay();
    uint8_t getHour();
    uint8_t getMinute();
    uint8_t getSecond();

    // Task virtual methods
    bool
    Callback();
    bool OnEnable();
    void OnDisable();

    // Chip virtual methods
    bool checkStatus();
};

#endif