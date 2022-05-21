#ifndef RFM69_H
#define RFM69_H

#define RFM69_RST PC1
#define RFM69_FREQ 915.0
#define PREAMBLE_LEN 8

#include <scheduler.h>
#include <RH_RF69.h>
#include <SPI.h>
#include <Arduino.h>
#include <lps25hb.h>
#include <gps.h>
#include <imu.h>
#include <state.h>
#include "chip.h"

enum Button
{
    NONE,
    PRELAUNCH_BUTTON,
    LAUNCH,
    TEST,
    DROGUE_SHOOT,
    MAIN_SHOOT,
    END_BUTTON,
    RESET_BUTTON
};

class Transceiver : public Task, public Chip
{
private:
    RH_RF69 *driver;
    uint8_t *output_buffer[128];
    uint8_t buffer[RH_RF69_MAX_MESSAGE_LEN];
    long measurements_delay;
    long previous_time = 0;
    uint16_t offset = 0;
    uint32_t packet_id = 0;
    Button button = NONE;

public:
    Transceiver(int RFM69_CS, int RFM69_INT, long measurements_delay);
    ~Transceiver();

    bool timeElapsed();
    void storeInBuffer(uint8_t *packet, int size);
    Button getButton();
    void buttonNone();

    // Task virtual methods
    bool Callback();
    bool OnEnable();
    void OnDisable();

    // Chip virtual methods
    bool checkStatus();
};

#endif