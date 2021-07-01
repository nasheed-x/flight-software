#ifndef PACKET_H
#define PACKET_H

#include <scheduler.h>
#include <SPI.h>
#include <Arduino.h>
#include <rfm69.h>
#include <flash.h>
#include <lps25hb.h>
#include <gps.h>
#include <imu.h>
#include <state.h>

/*** Factors ***/
#define PRESSURE_FACTOR 10
#define TEMPERATURE_FACTOR 1000
#define ACCELERATION_FACTOR 10000

#define PACKET_SIZE 128

class Packet : public Task
{
private:
    RH_RF69 *driver;
    uint8_t *buffer;
    Transceiver *transceiver;
    Flash *flash;
    Barometer *barometer;
    GPS *gps;
    IMU *imu;
    // State *state;
    long measurements_delay;
    long previous_time = 0;

public:
    Packet(Transceiver *transceiver,
           Flash *flash, IMU *imu,
           Barometer *barometer,
           GPS *gps,
        //    State *current_state,
           long measurements_delay);
    ~Packet();

    bool measurementsReady();

    // Task virtual methods
    bool Callback();
    bool OnEnable();
    void OnDisable();
};

#endif