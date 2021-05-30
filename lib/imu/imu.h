#include <Arduino.h>
#include <chip.h>
#include <SPI.h>
#include <bmx055.h>

#define BMX_SCK PB13
#define BMX_MISO PB14
#define BMX_MOSI PB15

#define ACC_CS PC7
#define GYR_CS PC4

class IMU : public Chip
{

private:
public:
    IMU();
    ~IMU();
    BMX055 *driver;

    // Chip virtual methods
    bool checkStatus();
};
