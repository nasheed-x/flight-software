#ifndef BUZZER_H
#define BUZZER_H

#include <scheduler.h>
#include <Arduino.h>

class Buzzer : public Task
{
private:
    uint16_t pin;
    uint16_t frequency;

public:
    Buzzer();
    ~Buzzer();

    void signal(uint16_t frequency, uint16_t duration);
    void signalSuccess();
    void signalFail();
    void wishMe();

    bool Callback();
    bool OnEnable();
    void OnDisable();
};

#endif