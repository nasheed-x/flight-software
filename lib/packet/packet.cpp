#include "packet.h"

Packet::Packet(Transceiver *transceiver,
               Flash *flash, IMU *imu,
               Barometer *barometer,
               GPS *gps, State *current_state,
               long measurements_delay) : Task(TASK_MILLISECOND, TASK_FOREVER, &scheduler, false),
                                          measurements_delay(measurements_delay),
                                          previous_time(0)
{
    this->transceiver = transceiver;
    this->flash = flash;
    this->barometer = barometer;
    this->gps = gps;
    this->imu = imu;
    this->state = current_state;
}

Packet::~Packet() {}

bool Packet::measurementsReady()
{

    long current_time = millis();
    if (current_time - this->previous_time >= this->measurements_delay)
    {
        this->previous_time = current_time;
        return true;
    }
    return false;
}

bool Packet::Callback()
{
    long current_time = millis();
    if (measurementsReady())
    {
        char packet[PACKET_SIZE];
        snprintf(packet, PACKET_SIZE, "%.4d%.2d%.2dT%.2d:%.2d:%.2dZ\n%.5d,%.5d,%.4d,%.4d,%.4d,S%.1d,%ld,%ld,%ld\n",
                 this->gps->getYear(),
                 this->gps->getMonth(),
                 this->gps->getDay(),
                 this->gps->getHour(),
                 this->gps->getMinute(),
                 this->gps->getSecond(),
                 (uint16_t)(this->barometer->getPressure() * PRESSURE_FACTOR),
                 (uint16_t)(this->barometer->getTemperature() * TEMPERATURE_FACTOR),
                 (uint16_t)(this->imu->getAccelerationX() * ACCELERATION_FACTOR),
                 (uint16_t)(this->imu->getAccelerationY() * ACCELERATION_FACTOR),
                 (uint16_t)(this->imu->getAccelerationZ() * ACCELERATION_FACTOR),
                 (int8_t)(*this->state),
                 (this->gps->getAltitude()),
                 (this->gps->getLatitude()),
                 (this->gps->getLongitude()));
        this->transceiver->storeInBuffer((uint8_t *)packet, PACKET_SIZE);
        this->flash->storeInBuffer((int8_t *)packet, PACKET_SIZE);
        return true;
    };
    return false;
}

bool Packet::OnEnable()
{
    return true;
}

void Packet::OnDisable()
{
}