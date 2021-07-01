#include "config.h"
#include "util.h"

// SERVO USES STM32F4 TIMER 1 THAT OPERATES AT TWICE THE EXPECTED FREQUENCY
// HENCE WRITE ALL MICROSECONDS IN DOUBLE
// 2000 MICROSECONDS --> 1 MILLISECOND
// 4000 MICROSECONDS --> 2 MILLISECOND

// Instantiate submodules
Buzzer *buzzer;           // Audio notification
PWMControl *pwm;          // PWM control
Barometer *barometer;     // Altitude + pressure
Transceiver *transceiver; // RF Communication
IMU *imu;                 // Orientation
Flash *flash;             // Flash Memory
GPS *gps;                 // GPS
Servo main_chute_servo;   // Deployment
Servo drogue_chute_servo; // Deployment
Blink *blinker;           // Blink task with PWM control
Packet *packet;           // Packet handler class
// State *current_state;     // State machine

void setup()
{
    // Initialize communication
    Wire.begin();
    Wire.setClock(400000); // Increase I2C clock speed to 400kHz
    Serial.begin(115200);

    State current_state = PRELAUNCH;

    // Wait until serial console is open, remove if not tethered to computer
    while (!Serial)
    {
        delay(1);
    }

    // Define all needed submodules
    buzzer = new Buzzer();
    pwm = new PWMControl();
    barometer = new Barometer(LPS_CS, 50);
    imu = new IMU(50);
    gps = new GPS(200);
    flash = new Flash(FLASH_CS, 50, buzzer);
    transceiver = new Transceiver(RFM69_CS, RFM69_INT, 1000);
    packet = new Packet(transceiver, flash, imu, barometer, gps, 50);
    blinker = new Blink(pwm);

    // Run sensor check
    check_sensors(pwm, barometer, transceiver, imu, flash, gps)
        ? buzzer->signalSuccess()
        : buzzer->signalFail();

    // Enable chips
    barometer->enable();
    imu->enable();
    gps->enable();
    packet->enable();
    // flash->enableDelayed(1000);
    transceiver->enableDelayed(1000);
}

void loop()
{
    scheduler.execute();
}