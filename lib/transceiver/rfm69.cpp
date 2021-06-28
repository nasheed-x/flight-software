#include "rfm69.h"

Transceiver::Transceiver(int RFM69_CS, int RFM69_INT, IMU *imu, Barometer *barometer, GPS *gps, long measurement_delay) : Task(TASK_MILLISECOND, TASK_FOREVER, &scheduler, false),
                                                                                                                          measurements_delay(measurement_delay),
                                                                                                                          previous_time(0)
{
    this->driver = new RH_RF69(RFM69_CS, RFM69_INT);
    this->barometer = barometer;
    this->gps = gps;
    this->imu = imu;
}

Transceiver::~Transceiver() {}

bool Transceiver::measurementsReady()
{

    long current_time = millis();
    if (current_time - this->previous_time >= this->measurements_delay)
    {
        this->previous_time = current_time;
        return true;
    }
    return false;
}

bool Transceiver::Callback()
{
    long current_time = millis();
    if (measurementsReady())
    {
        char radiopacket[RH_RF69_MAX_MESSAGE_LEN];
        snprintf(radiopacket, RH_RF69_MAX_MESSAGE_LEN, "%d\n%d\n%d\n%d\n%d\n%d\n%d\n%d\n",
                 (uint16_t)(this->barometer->getPressure() * PRESSURE_FACTOR),
                 (uint16_t)(this->barometer->getTemperature() * TEMPERATURE_FACTOR),
                 (uint16_t)this->gps->getAltitude(),
                 (uint16_t)this->gps->getLatitude(),
                 (uint16_t)this->gps->getLongitude(),
                 (uint16_t)(this->imu->getAccelerationX() * ACCELERATION_FACTOR),
                 (uint16_t)(this->imu->getAccelerationY() * ACCELERATION_FACTOR),
                 (uint16_t)(this->imu->getAccelerationZ() * ACCELERATION_FACTOR));
        // Send a message!
        Serial.println(radiopacket);
        this->driver->send((uint8_t *)radiopacket, sizeof(radiopacket));
        this->driver->waitPacketSent();
        this->previous_time = current_time;
        return true;
    };

    if (this->driver->available())
    {
        // Should be a message for us now
        uint8_t buf[RH_RF69_MAX_MESSAGE_LEN];
        uint8_t len = sizeof(buf);
        this->buffer = buf;

        if (this->driver->recv(this->buffer, &len))
        {
            if (!len)
                return false;
            buf[len] = 0;
        }

        return true;
    }

    return false;
}

bool Transceiver::OnEnable()
{

    if (!(this->driver->setFrequency(RFM69_FREQ)))
    {
        return false;
    }

    // range from 14-20 for power, 2nd arg must be true for 69HCW
    this->driver->setTxPower(20, true);

    // The encryption key has to be the same as the one in the server
    uint8_t key[] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
                     0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08};

    this->driver->setEncryptionKey(key);

    return true;
}

void Transceiver::OnDisable()
{
    delete this->buffer;
    delete this->driver;
}

bool Transceiver::checkStatus()
{

    return this->driver->init();
}