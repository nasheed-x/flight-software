#include "pwm.h"

PWMControl::PWMControl() : Task(TASK_IMMEDIATE, TASK_FOREVER, &scheduler, false), channel(0)
{
    this->driver = new PCA9635(0x70);
    // Inversion necessary!
    // See https://www.nxp.com/docs/en/data-sheet/PCA9635.pdf#page=17
    this->driver->writeMode(PCA9635_MODE2, 21);
    // Initialize
    this->driver->begin();
    // Set all channels off
    for (int channel = 0; channel < 16; channel++)
    {
        this->driver->setLedDriverMode(channel, PCA9635_LEDOFF);
    }
}

PWMControl::~PWMControl() {}

void PWMControl::switchOn(uint8_t channel)
{
    this->enable();
    return;
}

void PWMControl::switchOff()
{
    this->disable();
    return;
}

bool PWMControl::Callback()
{
    return true;
}

bool PWMControl::OnEnable()
{
    this->driver->setLedDriverMode(channel, PCA9635_LEDON);
    return true;
}

void PWMControl::OnDisable()
{
    this->driver->setLedDriverMode(channel, PCA9635_LEDOFF);
    return;
}
