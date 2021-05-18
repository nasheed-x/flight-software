#include <Arduino.h>
#include <buzzer.h>
#include <pwm.h>
#include <lps25hb.h>
#include <blink.h>
#include <TaskScheduler.h>

#include <config.h>

// Instantiate objects
Buzzer *buzzer;
PWMControl *pwm;
Barometer *barometer;
Blink *blinker;

SPIClass lps_spi(LPS_MOSI, LPS_MISO, LPS_SCK);

void check_sensors(PWMControl *pwm, Barometer *barometer)
{
    Serial.println("************************************");
    Serial.println("Conducting status check on all ICs...");
    Serial.println("************************************");

    // Check status of PCA9635 PWM driver
    pwm->checkStatus() ? Serial.println("PWM connection success! \xE2\x9C\x93") : Serial.println("PWM connection failed");

    // Check status of LPS25HB Barometer
    barometer->checkStatus() ? Serial.println("Barometer connection success! \xE2\x9C\x93") : Serial.println("Barometer connection failed");
}

void setup()
{
    // Initialize communication
    Wire.begin();
    Serial.begin(115200);
    lps_spi.begin();

    // Wait until serial console is open, remove if not tethered to computer
    while (!Serial)
    {
        delay(1);
    }

    // Define all needed submodules
    buzzer = new Buzzer();
    pwm = new PWMControl();
    blinker = new Blink(pwm);
    barometer = new Barometer(&lps_spi, LPS_CS);

    check_sensors(pwm, barometer);
}

void loop()
{
    scheduler.execute();
}