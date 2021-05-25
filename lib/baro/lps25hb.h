#ifndef LPS25HB_H
#define LPS25HB_H

#include <scheduler.h>
#include <Adafruit_LPS2X.h>
#include <Adafruit_Sensor.h>
#include <Arduino.h>
#include "chip.h"

class Barometer : public Task, public Chip
{
private:
    Adafruit_LPS25 *lps_driver;
    Adafruit_Sensor *temp_driver, *pressure_driver;
    SPIClass *spi_dev;
    int LPS_CS;
    long measurement_delay;
    long previous_time;
    float pressure;
    float temperature;

public:
    Barometer(int LPS_CS, SPIClass *LPS_SPI, long measurement_delay);
    ~Barometer();

    bool measurementReady();
    float getTemperature();
    float getPressure();

    // Task virtual methods
    bool Callback();
    bool OnEnable();
    void OnDisable();

    // Chip virtual methods
    bool checkStatus();
};

#endif