#include "flash.h"

Flash::Flash(uint8_t FLASH_CS, long measurement_delay, Buzzer *buzzer) : Task(TASK_MILLISECOND, TASK_FOREVER, &scheduler, false),
                                                                         measurement_delay(measurement_delay)
{
    this->driver = new SPIFlash(FLASH_CS, 0xEF40);
    this->buzzer = buzzer;
}

Flash::~Flash() {}

bool Flash::measurementReady()
{
    long current_time = millis();
    if (current_time - this->previous_time >= this->measurement_delay)
    {
        this->previous_time = current_time;
        return true;
    }
    return false;
}

void Flash::storeInBuffer(int8_t *packet, int size)
{
    memcpy(this->storage_buffer, packet, size);
}

bool Flash::Callback()
{
    if (measurementReady() && this->memory_unit_offset < MAX_DATA_UNITS)
    {
        // Write data on memory offset
        // Serial.println((char *)this->storage_buffer);
        // this->driver->writeBytes(FIRST_PAGE_OFFSET + (this->memory_unit_offset * 128), this->storage_buffer, this->unit_size);

        // char read_buffer[this->unit_size];
        // this->driver->readBytes(FIRST_PAGE_OFFSET + (this->memory_unit_offset * 128), read_buffer, this->unit_size);
        // Serial.println((char *)read_buffer);

        // Update memory offset
        // this->memory_unit_offset++;
        // this->driver->blockErase4K(0);
        // this->driver->writeBytes(0, &this->memory_unit_offset, 4);

        // uint32_t test;
        // this->driver->readBytes(0, &test, 4);
        // Serial.println(millis());
        // Serial.println(test);
        return true;
    }
    return false;
}

bool Flash::OnEnable()
{
    int32_t offset_read = 0;
    this->driver->readBytes(0, &offset_read, 4);
    this->memory_unit_offset = (offset_read > 0) ? offset_read : 0;
    Serial.print("Memory Offset: ");
    Serial.println(this->memory_unit_offset);
    // If memory is almost full
    if (this->memory_unit_offset > (int32_t)(MAX_DATA_UNITS * 0.9))
    {
        Serial.println("Memory full...cleaning memory!");
        this->buzzer->signal(200, 500);
        this->driver->chipErase();
        Serial.println("Memory emptied!");
    }
    return true;
}

void Flash::OnDisable()
{
    // this->driver->writeBytes(0, &this->memory_unit_offset, 4);
}

bool Flash::checkStatus()
{
    return this->driver->initialize();
}