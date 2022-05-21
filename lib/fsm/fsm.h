#ifndef FSM_H
#define FSM_H

#include <scheduler.h>
#include <SPI.h>
#include <Arduino.h>
#include <Servo.h>
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
#define LAUNCH_ACC_THRESHOLD 20
#define BURNOUT_ACC_THRESHOLD 2
#define AGL_THRESHOLD 1000
#define PRESSURE_BUFFER_SIZE 5
#define G_FACTOR 9.80665

class FSM : public Task
{
private:
    uint8_t *buffer;
    float pressure_buffer[PRESSURE_BUFFER_SIZE] ={2000};
    Barometer *barometer;
    Transceiver *transceiver;
    GPS *gps;
    IMU *imu;
    Servo drogue_chute_servo;
    // State *state;
    int pressure_index = 0;
    long measurements_delay;
    long previous_time = 0;

public:
    FSM(IMU *imu,
        Barometer *barometer,
        GPS *gps,
        Transceiver *transceiver,
        Servo drogue_chute_servo,
        //    State *current_state,
        long measurements_delay);
    ~FSM();

    bool measurementsReady();
    bool storePressure(float pressure_5);

    // Task virtual methods
    bool Callback();
    bool OnEnable();
    void OnDisable();
};

enum Event
{
    NO_EVENT,
    LIFTOFF,
    BURNOUT,
    APOGEE,
    DESCEND,
    AGL_REACHED,
    LAND
};

#endif