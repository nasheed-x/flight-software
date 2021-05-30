#include <Arduino.h>
#include <buzzer.h>
#include <pwm.h>
#include <lps25hb.h>
#include <rfm69.h>
#include <blink.h>
#include <imu.h>
#include <TaskScheduler.h>

#include <config.h>

// Instantiate objects
Buzzer *buzzer;
PWMControl *pwm;
Barometer *barometer;
Blink *blinker;
Transceiver *transceiver;
IMU *imu;

SPIClass LPS_SPI(LPS_MOSI, LPS_MISO, LPS_SCK);

enum state
{
    PRELAUNCH,
    LAUNCH_READY,
    MOTOR_BURN,
    BURNOUT,
    APOGEE,
    POST_DROGUE,
    POST_MAIN,
    END
};

bool check_sensors(PWMControl *pwm, Barometer *barometer, Transceiver *transceiver, IMU *imu)
{
    Serial.println("************************************");
    Serial.println("Conducting status check on all ICs...");
    Serial.println("************************************");

    bool error = true;

    // Check status of PCA9635 PWM driver
    if (pwm->checkStatus())
    {
        Serial.println("PWM connection success! \xE2\x9C\x93");
    }

    else
    {
        Serial.println("PWM connection failed \xE2\x9C\x97");
        error = false;
    }

    // Check status of LPS25HB Barometer
    if (barometer->checkStatus())
    {
        Serial.println("Barometer connection success! \xE2\x9C\x93");
    }
    else
    {
        Serial.println("Barometer connection failed \xE2\x9C\x97");
        error = false;
    }

    // Check status of RFM69HW Transceiver
    if (transceiver->checkStatus())
    {
        Serial.println("Transceiver connection success! \xE2\x9C\x93");
    }
    else
    {
        Serial.println("Transceiver connection failed \xE2\x9C\x97");
        error = false;
    }

    // Check status of BMX055 IMU
    if (imu->checkStatus())
    {
        Serial.println("IMU connection success! \xE2\x9C\x93");
    }
    else
    {
        Serial.println("IMU connection failed \xE2\x9C\x97");
        error = false;
    }

    return error;
}

void setup()
{
    // Initialize communication
    Wire.begin();
    Serial.begin(115200);
    LPS_SPI.begin();

    // Wait until serial console is open, remove if not tethered to computer
    while (!Serial)
    {
        delay(1);
    }

    // Define all needed submodules
    buzzer = new Buzzer();
    pwm = new PWMControl();
    blinker = new Blink(pwm);
    barometer = new Barometer(LPS_CS, &LPS_SPI, 500);
    transceiver = new Transceiver(RFM69_CS, RFM69_INT);
    imu = new IMU();

    // Run sensor check
    check_sensors(pwm, barometer, transceiver, imu) ? buzzer->signalSuccess() : buzzer->signalFail();

    // // Enable chips
    // barometer->enable();
    // transceiver->enable();
}

void loop()
{
    scheduler.execute();
}