#include "fram.h"

FRAM::FRAM(uint8_t FRAM_CS, long measurement_delay) : Task(TASK_MILLISECOND, TASK_FOREVER, &scheduler, false),
                                                      measurement_delay(measurement_delay)
{
}

FRAM::~FRAM() {}

bool FRAM::measurementReady()
{
    long current_time = millis();
    if (current_time - this->previous_time >= this->measurement_delay)
    {
        this->previous_time = current_time;
        return true;
    }
    return false;
}

bool FRAM::Callback()
{
    if (measurementReady())
    {
    }
    return false;
}

bool FRAM::OnEnable()
{
    return true;
}

void FRAM::OnDisable()
{
}

bool FRAM::checkStatus()
{
    return false;
}