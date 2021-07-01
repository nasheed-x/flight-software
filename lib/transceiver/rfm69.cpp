#include "rfm69.h"

Transceiver::Transceiver(int RFM69_CS, int RFM69_INT, long measurement_delay) : Task(TASK_MILLISECOND, TASK_FOREVER, &scheduler, false),
                                                                                measurements_delay(measurement_delay),
                                                                                previous_time(0)
{
    this->driver = new RH_RF69(RFM69_CS, RFM69_INT);
}

Transceiver::~Transceiver() {}

bool Transceiver::timeElapsed()
{

    long current_time = millis();
    if (current_time - this->previous_time >= this->measurements_delay)
    {
        this->previous_time = current_time;
        return true;
    }
    return false;
}

void Transceiver::storeInBuffer(uint8_t *packet, int size)
{
    memcpy(this->output_buffer, packet, size);
}

bool Transceiver::Callback()
{

    long current_time = millis();

    // sending first packet
    if (timeElapsed())
    {
        if (this->offset == 0)
        {
            char message[RH_RF69_MAX_MESSAGE_LEN];
            snprintf(message, sizeof(message), "#%.4ld:1\n", this->packet_id);
            memcpy(message + PREAMBLE_LEN, (char *)this->output_buffer + this->offset, 45);
            // Serial.write(message, 45 + PREAMBLE_LEN);
            // Serial.println();
            this->driver->send((uint8_t *)message, 45 + PREAMBLE_LEN);
            this->driver->waitPacketSent();
            this->offset = 46;
        }
        else
        {
            char message[RH_RF69_MAX_MESSAGE_LEN];
            snprintf(message, sizeof(message), "#%.4ld:2\n", this->packet_id);
            memcpy(message + PREAMBLE_LEN, (char *)this->output_buffer + this->offset, 32);
            // Serial.write(message, 32 + PREAMBLE_LEN);
            this->driver->send((uint8_t *)message, 32 + PREAMBLE_LEN);
            this->driver->waitPacketSent();
            this->offset = 0;
            this->packet_id++;
        }
    }

    if (this->driver->available())
    {
        uint8_t buf[RH_RF69_MAX_MESSAGE_LEN];
        uint8_t len = sizeof(buf);
        if (this->driver->recv(buf, &len))
        {
            if (!len)
                return false;
            Serial.println((char *)buf);
            buf[len] = 0;
        }

        if (strcmp((char *)buf, "launch") == 0)
        {
            if (current_state == PRELAUNCH)
            {
                current_state = LAUNCH_READY;
                Serial.println("Setting state to Launch Ready...");
                Serial.println(current_state);
            }
        }

        else if (strcmp((char *)buf, "end") == 0)
        {
            if (current_state == PRELAUNCH || current_state == POST_MAIN)
            {
                current_state = END;
                Serial.println("Setting state to End Flight...");
                Serial.println(current_state);
            }
        }

        else if (strcmp((char *)buf, "reset") == 0)
        {
            current_state = PRELAUNCH;
            NVIC_SystemReset();
            Serial.println("Reset!");
            Serial.println(current_state);
        }
    }
    return true;
}

bool Transceiver::OnEnable()
{
    if (!(this->driver->setFrequency(RFM69_FREQ)))
    {
        return false;
    }

    // range from 14-20 for power, 2nd arg must be true for 69HCW
    this->driver->setTxPower(20, true);

    // The encryption key has to be the same as the one in the server
    uint8_t key[] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
                     0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08};

    this->driver->setEncryptionKey(key);

    return true;
}

void Transceiver::OnDisable()
{
    delete this->buffer;
    delete this->driver;
}

bool Transceiver::checkStatus()
{

    return this->driver->init();
}