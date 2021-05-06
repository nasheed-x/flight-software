#include <Arduino.h>
#include <Wire.h>
#include <config.h>
#include <buzzer.h>
#include <pwm.h>
#include <blink.h>
#include <TaskScheduler.h>

// Instantiate objects
Buzzer *buzzer;
PWMControl *pwm;
Blink *blinker;

void setup()
{
    // Initialize communication
    Wire.begin();
    Serial.begin(115200);

    // Wait until serial console is open, remove if not tethered to computer
    while (!Serial)
    {
        delay(1);
    }

    // Define all needed submodules
    buzzer = new Buzzer();
    pwm = new PWMControl();
    blinker = new Blink(pwm);

    // Program start buzzer
    buzzer->signal(1000, 1000);
    blinker->blink(0, 2000, 2);
}

void loop()
{
    scheduler.execute();
}