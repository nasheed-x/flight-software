#include "fsm.h"

FSM::FSM(IMU *imu,
         Barometer *barometer,
         GPS *gps,
         Transceiver *transceiver,
         long measurements_delay) : Task(TASK_MILLISECOND, TASK_FOREVER, &scheduler, false),
                                    measurements_delay(measurements_delay),
                                    previous_time(0)
{
    this->transceiver = transceiver;
    this->barometer = barometer;
    this->gps = gps;
    this->imu = imu;
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
    this->pressure_index = (this->pressure_index>=PRESSURE_BUFFER_SIZE) ? 0:this->pressure_index;

    return (sum/PRESSURE_BUFFER_SIZE < pressure) ? true:false;
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

        // Event Update
        if (current_state == LAUNCH_READY && accZ > LAUNCH_ACC_THRESHOLD)
        {
            Serial.println("Z Acceleration");
            Serial.println(accZ);
            event = LIFTOFF;
        }
        else if (current_state == POWERED_FLIGHT && accZ < BURNOUT_ACC_THRESHOLD)
        {
            event = BURNOUT;
        }
        else if (current_state == UNPOWERED_FLIGHT && descent)
        {
            event = APOGEE;
        }else if (current_state == POST_DROGUE && height<AGL_THRESHOLD)
        {
            event = AGL_REACHED;
        }else if (current_state == POST_MAIN && descent)
        {
            event = APOGEE;
        }


        // State Updates
        switch (current_state)
        {
        case PRELAUNCH:
            if (this->transceiver->getButton() == LAUNCH)
            {
                current_state = LAUNCH_READY;
                this->transceiver->buttonNone();
                Serial.println(current_state);
            }
            else if (this->transceiver->getButton() == END)
            {
                current_state = END;
            }
            break;

        case LAUNCH_READY:
            switch (event)
            {
            case LIFTOFF:
                /* Change the state */
                Serial.println("INSIDE LIFTOFF");
                Serial.println("LIFTOFF");
                current_state = POWERED_FLIGHT;
                break;
            }
            break;

        case POWERED_FLIGHT:
            switch (event)
            {
            case BURNOUT:
                /* Change the state */
                current_state = UNPOWERED_FLIGHT;
                break;
            }
            break;

        case UNPOWERED_FLIGHT:
            switch (event)
            {
            case APOGEE:
                /* Change the state */
                current_state = POST_DROGUE;
                break;
            }
            break;

        case POST_DROGUE:
            switch (event)
            {
            case AGL_REACHED:
                /* Change the state */
                current_state = POST_MAIN;
                break;
            }
            break;

        case POST_MAIN:
            switch (event)
            {
            case LAND:
                current_state = END;
                break;
            }
            break;

        case END:
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