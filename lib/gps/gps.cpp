#include "gps.h"

GPS::GPS(long measurement_delay) : Task(TASK_MILLISECOND, TASK_FOREVER, &scheduler, false),
                                   measurement_delay(measurement_delay),
                                   previous_time(0),
                                   altitudeMSL(-1),
                                   latitude(-1),
                                   longitude(-1)
{
    this->driver = new SFE_UBLOX_GNSS();
}

GPS::~GPS() {}

int32_t GPS::getAltitude()
{
    return this->altitudeMSL;
}

int32_t GPS::getLatitude()
{
    return this->latitude;
}

int32_t GPS::getLongitude()
{
    return this->longitude;
}

uint16_t GPS::getYear()
{
    return this->year;
}

uint8_t GPS::getMonth()
{
    return this->month;
}

uint8_t GPS::getDay()
{
    return this->day;
}
uint8_t GPS::getHour()
{
    return this->driver->getHour();
}

uint8_t GPS::getMinute()
{
    return this->driver->getMinute();
}

uint8_t GPS::getSecond()
{
    return this->driver->getSecond();
}

bool GPS::measurementReady()
{

    long current_time = millis();
    if (current_time - this->previous_time >= this->measurement_delay)
    {
        this->previous_time = current_time;
        return true;
    }
    return false;
}

bool GPS::Callback()
{
    if (measurementReady() && this->driver->getPVT())
    {
        this->altitudeMSL = this->driver->getAltitudeMSL();
        this->latitude = this->driver->getLatitude();
        this->longitude = this->driver->getLongitude();
        this->driver->flushPVT();
        return true;
    }
    return false;
}

bool GPS::OnEnable()
{
    this->driver->setI2COutput(COM_TYPE_UBX);
    this->driver->setNavigationFrequency(20);
    this->day = this->driver->getDay();
    this->day = this->driver->getDay();
    this->month = this->driver->getMonth();
    this->year = this->driver->getYear();
    return true;
}

void GPS::OnDisable()
{
}

bool GPS::checkStatus()
{
    return this->driver->begin();
}