#ifndef RFM69_H
#define RFM69_H

#define RFM69_RST PC1
#define RFM69_FREQ 915.0

#include <scheduler.h>
#include <RH_RF69.h>
#include <SPI.h>
#include <Arduino.h>
#include <lps25hb.h>
#include <gps.h>
#include <imu.h>
#include "chip.h"

/*** Factors ***/
#define PRESSURE_FACTOR 10
#define TEMPERATURE_FACTOR 1000
#define ACCELERATION_FACTOR 10000

class Transceiver : public Task, public Chip
{
private:
    RH_RF69 *driver;
    uint8_t *buffer;
    Barometer *barometer;
    GPS *gps;
    IMU *imu;
    long measurements_delay;
    long previous_time = 0;

public:
    Transceiver(int RFM69_CS, int RFM69_INT, IMU *imu, Barometer *barometer, GPS *gps, long measurements_delay);
    ~Transceiver();

    bool measurementsReady();

    // Task virtual methods
    bool Callback();
    bool OnEnable();
    void OnDisable();

    // Chip virtual methods
    bool checkStatus();
};

#endif