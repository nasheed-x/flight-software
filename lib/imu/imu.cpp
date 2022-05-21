#include <imu.h>

IMU::IMU(long measurement_delay) : Task(TASK_MILLISECOND, TASK_FOREVER, &scheduler, false),
                                   measurement_delay(measurement_delay)
{
    this->driver = new BMX055();
};

IMU::~IMU(){};

bool IMU::measurementReady()
{
    long current_time = millis();
    if (current_time - this->previous_time >= this->measurement_delay)
    {
        this->previous_time = current_time;
        return true;
    }
    return false;
}

float IMU::getAccelerationX()
{
    return acc_x + this->acc_offset_x;
}

float IMU::getAccelerationY()
{
    return acc_y + this->acc_offset_x;
}

float IMU::getAccelerationZ()
{
    return acc_z + this->acc_offset_x;
}

void IMU::calibrateAccelerometer()
{
    Serial.println("Calibrating IMU...");
    float accel[3] = {this->acc_x, this->acc_y, this->acc_z};
    float offsets[3] = {0, 0, 0};
    // Take 800 readings of accelerometer and calculate
    for (int i = 1; i < 500; i++)
    {
        this->driver->getAccelerometer(&this->acc_x, &this->acc_y, &this->acc_z);
        for (int j = 0; j < 3; j++)
        {
            // if value is greather than 0.5, its probably the dominant orientation
            if (accel[j] > 0.5)
            {
                offsets[j] = ((1.0 - accel[j]) + offsets[j]) / i;
            }

            // else it is not the dominant orientation and should be set to 0
            else
            {
                offsets[j] = ((0.0 - accel[j]) + offsets[j]) / i;
            }
        }
    }

    this->acc_offset_x = offsets[0];
    this->acc_offset_y = offsets[1];
    this->acc_offset_z = offsets[2];
    Serial.println("Calibrated!");
    Serial.println(this->acc_offset_z);
}

bool IMU::Callback()
{
    if (measurementReady())
    {
        this->driver->update();
        this->driver->getAccelerometer(&this->acc_x, &this->acc_y, &this->acc_z);
        return true;
    }
    return false;
}

bool IMU::OnEnable()
{
    this->driver->init();
    this->calibrateAccelerometer();
    return true;
}

void IMU::OnDisable()
{
}

bool IMU::checkStatus()
{
    uint8_t result = this->driver->getChipID();
    uint8_t expected = 0xFA;
    return (result == expected);
}
