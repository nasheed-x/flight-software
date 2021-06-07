#include "config.h"
#include "util.h"

// Instantiate objects
Buzzer *buzzer;
PWMControl *pwm;
Barometer *barometer;
Blink *blinker;
Transceiver *transceiver;
IMU *imu;
Flash *flash;
Servo main_chute_servo;
Servo drogue_chute_servo;

// SERVO USES STM32F4 TIMER 1 THAT OPERATES AT TWICE THE EXPECTED FREQUENCY
// HENCE WRITE ALL MICROSECONDS IN DOUBLE
// 2000 MICROSECONDS --> 1 MILLISECOND
// 4000 MICROSECONDS --> 2 MILLISECOND

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
    main_chute_servo.attach(MAIN_CHUTE_SERVO_PIN);
    drogue_chute_servo.attach(DROGUE_CHUTE_SERVO_PIN);

    // Wait until serial console is open, remove if not tethered to computer
    while (!Serial)
    {
        delay(1);
    }

    // Define all needed submodules
    buzzer = new Buzzer();
    pwm = new PWMControl();
    barometer = new Barometer(LPS_CS, 500);
    transceiver = new Transceiver(RFM69_CS, RFM69_INT);
    imu = new IMU(500);
    flash = new Flash(FLASH_CS);

    blinker = new Blink(pwm);

    // Run sensor check
    check_sensors(pwm, barometer, transceiver, imu, flash)
        ? buzzer->signalSuccess()
        : buzzer->signalFail();

    // Enable chips
    barometer->enable();
    imu->enable();
    transceiver->enable();
}

void loop()
{
}