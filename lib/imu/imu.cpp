#include <imu.h>

IMU::IMU()
{
    this->driver = new BMX055();
    this->driver->init();
};

IMU::~IMU(){};

bool IMU::checkStatus()
{
    uint8_t result = this->driver->getChipID();
    uint8_t expected = 0xFA;
    return (result == expected);
}
