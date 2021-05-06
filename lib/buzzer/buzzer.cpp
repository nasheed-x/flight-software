#include "buzzer.h"

Buzzer::Buzzer() : Task(TASK_IMMEDIATE, TASK_ONCE, &scheduler, false), pin(BUZZER)
{
    pinMode(pin, OUTPUT);
}

Buzzer::~Buzzer() {}

void Buzzer::signal(uint16_t frequency, uint16_t duration)
{
    this->frequency = frequency;
    this->setInterval(duration);
    this->restartDelayed();
}

bool Buzzer::Callback() { return true; }

bool Buzzer::OnEnable()
{
    tone(this->pin, this->frequency);
    return true;
}

void Buzzer::OnDisable()
{
    noTone(this->pin);
    return;
}
