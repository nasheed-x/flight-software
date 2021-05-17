#include <Arduino.h>
#include <Wire.h>
#include <buzzer.h>
#include <pwm.h>
#include <blink.h>
#include <TaskScheduler.h>

#include <config.h>

// Instantiate objects
Buzzer *buzzer;
PWMControl *pwm;
Blink *blinker;

void check_sensors(PWMControl *pwm)
{
    Serial.println("Conducting status check on all ICs...");

    // Check status of PWM driver
    pwm->checkStatus() ? Serial.println("PWM connection success!") : Serial.println("PWM connection failed");

    // Check status of pCA
}

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

    check_sensors(pwm);
}

void loop()
{
    scheduler.execute();
}