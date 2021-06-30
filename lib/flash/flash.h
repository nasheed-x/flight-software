#ifndef W25Q_FLASH_H
#define W25Q_FLASH_H

// Always offset by the number of
// bytes in the first page
#define FIRST_PAGE_OFFSET 256

#define MAX_DATA_UNITS 131072

#include <Arduino.h>
#include <SPIFlash.h>
#include "buzzer.h"
#include "chip.h"
#include "scheduler.h"

class Flash : public Task, public Chip
{
private:
    SPIFlash *driver;
    long measurement_delay;
    long previous_time = 0;
    char *storage_buffer[128];
    int32_t memory_unit_offset = 0;
    int unit_size = 128;
    Buzzer *buzzer;

public:
    Flash(uint8_t FLASH_CS, long measurement_delay, Buzzer *buzzer);
    ~Flash();

    bool measurementReady();

    void storeInBuffer(int8_t *packet, int size);

    // Task virtual methods
    bool Callback();
    bool OnEnable();
    void OnDisable();

    // Chip virtual method
    bool checkStatus();
};

#endif