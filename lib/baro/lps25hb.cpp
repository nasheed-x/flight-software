#include "lps25hb.h"

Barometer::Barometer(SPIClass *lps_spi, int LPS_CS)
{
    this->driver = new LPS25HBSensor(lps_spi, LPS_CS);
    this->driver->Enable();
}

Barometer::~Barometer() {}

bool Barometer::Callback()
{
    return true;
}

bool Barometer::checkStatus()
{
    uint8_t lps_id;
    lps_id = 69;
    this->driver->begin();
    LPS25HBStatusTypeDef return_status = this->driver->ReadReg(0x0F, &lps_id);
    this->driver->end();
    if ((lps_id == 189) && (return_status == 0))
    {
        return true;
    }
    return false;
}