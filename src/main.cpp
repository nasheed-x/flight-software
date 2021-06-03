#include "config.h"
#include "util.h"

// Instantiate objects
Buzzer *buzzer;
PWMControl *pwm;
Barometer *barometer;
Blink *blinker;
Transceiver *transceiver;
IMU *imu;
Servo servo;

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

void setup()
{
    // Initialize communication
    Wire.begin();
    Serial.begin(115200);
    LPS_SPI.begin();
    servo.attach(SERVO_PIN);

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
    imu = new IMU(500);

    // Run sensor check
    check_sensors(pwm, barometer, transceiver, imu) ? buzzer->signalSuccess() : buzzer->signalFail();

    // Enable chips
    barometer->enable();
    imu->enable();
    transceiver->enable();

    servo.write(180);
}

void loop()
{
}