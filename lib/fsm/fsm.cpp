#include "fsm.h"

FSM::FSM(IMU *imu,
         Barometer *barometer,
         GPS *gps,
         Transceiver *transceiver,
         Servo drogue_chute_servo,
         long measurements_delay) : Task(TASK_MILLISECOND, TASK_FOREVER, &scheduler, false),
                                    measurements_delay(measurements_delay),
                                    previous_time(0)
{
    this->transceiver = transceiver;
    this->barometer = barometer;
    this->gps = gps;
    this->imu = imu;
    this->drogue_chute_servo = drogue_chute_servo;
    // this->state = current_state;
}

FSM::~FSM() {}

bool FSM::measurementsReady()
{

    long current_time = millis();
    if (current_time - this->previous_time >= this->measurements_delay)
    {
        this->previous_time = current_time;
        return true;
    }
    return false;
}

bool FSM::storePressure(float pressure)
{

    float sum = 0;
    for (int i = 0; i < PRESSURE_BUFFER_SIZE; i++)
    {
        sum += this->pressure_buffer[i];
    }

    this->pressure_buffer[this->pressure_index] = pressure;
    this->pressure_index++;
    this->pressure_index = (this->pressure_index >= PRESSURE_BUFFER_SIZE) ? 0 : this->pressure_index;

    return (sum / PRESSURE_BUFFER_SIZE < pressure) ? true : false;
}

bool FSM::Callback()
{
    long current_time = millis();
    if (measurementsReady())
    {
        Event event = NO_EVENT;
        float height, accZ, pressure, temperature;
        bool descent;

        // Data Collection
        pressure = this->barometer->getPressure();
        temperature = this->barometer->getTemperature();
        height = this->gps->getAltitude();
        accZ = this->imu->getAccelerationZ() * G_FACTOR;

        descent = storePressure(pressure);

        // State Updates
        switch (current_state)
        {
        case PRELAUNCH:
            if (this->transceiver->getButton() == LAUNCH)
            {
                this->drogue_chute_servo.writeMicroseconds(1000);
                current_state = LAUNCH_READY;
                this->transceiver->buttonNone();
            }
            else if (this->transceiver->getButton() == TEST)
            {
                this->drogue_chute_servo.writeMicroseconds(1000);
                current_state = TEST_READY;
                this->transceiver->buttonNone();
            }
            else if (this->transceiver->getButton() == END_BUTTON)
            {
                current_state = END;
                this->transceiver->buttonNone();
            }
            break;

        case LAUNCH_READY:
            if (this->transceiver->getButton() == PRELAUNCH_BUTTON)
            {
                current_state = PRELAUNCH;
                this->transceiver->buttonNone();
            }
            else if (this->transceiver->getButton() == TEST)
            {
                current_state = TEST_READY;
                this->transceiver->buttonNone();
            }
            else if (this->transceiver->getButton() == END_BUTTON)
            {
                current_state = END;
                this->transceiver->buttonNone();
            }
            else if (accZ > LAUNCH_ACC_THRESHOLD)
            {
                current_state = POWERED_FLIGHT;
            }
            break;

        case POWERED_FLIGHT:
            if (accZ < BURNOUT_ACC_THRESHOLD)
            {
                /* Change the state */
                current_state = UNPOWERED_FLIGHT;
            }
            break;

        case UNPOWERED_FLIGHT:
            if (descent)
            {
                current_state = POST_DROGUE;
            }
            break;

        case POST_DROGUE:
            if (height < AGL_THRESHOLD)
            {
                /* Change the state */
                current_state = POST_MAIN;
            }
            else if (this->transceiver->getButton() == END_BUTTON)
            {
                current_state = END;
                this->transceiver->buttonNone();
            }
            break;

        case POST_MAIN:
            if (!descent)
            {
                current_state = END;
            }
            else if (this->transceiver->getButton() == END_BUTTON)
            {
                current_state = END;
                this->transceiver->buttonNone();
            }
            break;

        case TEST_READY:
            if (this->transceiver->getButton() == DROGUE_SHOOT)
            {
                this->drogue_chute_servo.writeMicroseconds(5000);
                current_state = POST_DROGUE;
                this->transceiver->buttonNone();
            }
            else if (this->transceiver->getButton() == MAIN_SHOOT)
            {
                current_state = POST_MAIN;
                this->transceiver->buttonNone();
            }
            else if (this->transceiver->getButton() == END_BUTTON)
            {
                current_state = END;
                this->transceiver->buttonNone();
            }
            break;

        case END:
            if (this->transceiver->getButton() == PRELAUNCH_BUTTON)
            {
                current_state = PRELAUNCH;
                this->transceiver->buttonNone();
            }
            break;
        }
    }

    return false;
}

bool FSM::OnEnable()
{
    return true;
}

void FSM::OnDisable()
{
}