#include <Arduino.h>
#include <Wire.h>
#include <config.h>
#include <buzzer.h>
#include <TaskScheduler.h>

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

    Buzzer *buzzer = new Buzzer();
    buzzer->signal(1000, 1000);
}

void loop()
{
    scheduler.execute();
}