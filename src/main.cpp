#include "config.h"
#include "util.h"
#include "SparkFun_u-blox_GNSS_Arduino_Library.h"

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

// Instantiate submodules
Buzzer *buzzer;           // Audio notification
PWMControl *pwm;          // PWM control
Barometer *barometer;     // Altitude + pressure
Transceiver *transceiver; // RF Communication
IMU *imu;                 // Orientation
Flash *flash;             // Memory
GPS *gps;                 // GPS
Servo main_chute_servo;   // Deployment
Servo drogue_chute_servo; // Deployment
Blink *blinker;           // Blink task with PWM control
state current_state;      // State machine

void setup()
{
    // Initialize communication
    Wire.begin();
    Serial.begin(115200);

    current_state = PRELAUNCH;

    // Wait until serial console is open, remove if not tethered to computer
    while (!Serial)
    {
        delay(1);
    }

    // Define all needed submodules
    buzzer = new Buzzer();
    pwm = new PWMControl();
    barometer = new Barometer(LPS_CS, 500);
    imu = new IMU(500);
    flash = new Flash(FLASH_CS);
    gps = new GPS(500);
    transceiver = new Transceiver(RFM69_CS, RFM69_INT, barometer, gps, 500);
    blinker = new Blink(pwm);

    // Run sensor check
    check_sensors(pwm, barometer, transceiver, imu, flash, gps)
        ? buzzer->signalSuccess()
        : buzzer->signalFail();

    // Enable chips
    barometer->enable();
    imu->enable();
    transceiver->enable();
}

void loop()
{
    // scheduler.execute();
}