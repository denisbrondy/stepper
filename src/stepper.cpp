#include "stepper.h"

#ifdef STEPPER_DEBUG
#define STEPPER_DEBUG_LOG(...) Serial.printf(__VA_ARGS__);
unsigned long time;
#else
#define STEPPER_DEBUG_PRINTLN(x)
#endif

bool stopped = true;
volatile int32_t currentStepPositions = 0;
int32_t targetStepPosition = 0;
DIRECTION direction = FORWARD;

void stepCounterInterrupt()
{
#ifdef STEPPER_DEBUG
    unsigned long now = micros();
    double periodInMicros = now - time;
    double frequency = 1000000 / periodInMicros;
    Serial.print("Monitored pulse frequency is ");
    Serial.print(frequency);
    Serial.println(" Hz");
    time = now;
#endif
    if (direction == FORWARD)
    {
        currentStepPositions++;
    }
    else if (direction == BACKWARD)
    {
        currentStepPositions--;
    }
}

int32_t Stepper::getPosition()
{
    return currentStepPositions;
}

Stepper::Stepper(int pulsePin, int directionPin, int interruptPin, double frequency)
{
    this->_pulsePin = pulsePin;
    this->_directionPin = directionPin;
    this->_interruptPin = interruptPin;
    // Pulse
    pinMode(this->_pulsePin, OUTPUT);
    digitalWrite(this->_pulsePin, LOW);
    ledcAttachPin(this->_pulsePin, 1);
    this->setFrequency(frequency);

    // Direction
    pinMode(this->_directionPin, OUTPUT);
    digitalWrite(this->_directionPin, LOW);

    // Interrupt monitor
    pinMode(this->_interruptPin, INPUT);
    attachInterrupt(this->_interruptPin, stepCounterInterrupt, RISING);
}

void Stepper::setFrequency(double frequency)
{
    ledcSetup(1, frequency, 8);
}

void Stepper::move(DIRECTION toDirection, int steps)
{
    direction = toDirection;
    if (direction == FORWARD)
    {
        targetStepPosition = currentStepPositions + steps;
        digitalWrite(this->_directionPin, HIGH); // TBD to check
#ifdef STEPPER_DEBUG
        STEPPER_DEBUG_LOG("Stepping forward from step position : %d to %d (%d steps)\n", currentStepPositions, targetStepPosition, steps);
#endif
    }
    else if (direction == BACKWARD)
    {
        targetStepPosition = currentStepPositions - steps;
        digitalWrite(this->_directionPin, LOW); // TBD to check
#ifdef STEPPER_DEBUG
        STEPPER_DEBUG_LOG("Stepping backward from step position : %d to %d (%d steps)\n", currentStepPositions, targetStepPosition, steps);
#endif
    }
    ledcWrite(1, 128);
    stopped = false;
}

void Stepper::handle()
{
    if (direction == FORWARD and currentStepPositions >= targetStepPosition)
    {
        this->stop();
    }
    else if (direction == BACKWARD and currentStepPositions <= targetStepPosition)
    {
        this->stop();
    }
}

void Stepper::stop()
{
    if (!stopped)
    {
#ifdef STEPPER_DEBUG
        STEPPER_DEBUG_LOG("Stopping motor\n");
#endif
        ledcWrite(1, 0);
        stopped = true;
    }
}

void Stepper::goToZero()
{
    if (currentStepPositions > 0) {
        targetStepPosition = 0;
        digitalWrite(this->_directionPin, LOW); // TBD CHECK
        ledcWrite(1, 128);
        stopped = false;
    } else if (currentStepPositions < 0) {
        targetStepPosition = 0;
        digitalWrite(this->_directionPin, HIGH); // TBD CHECK
        ledcWrite(1, 128);
        stopped = false;
    }
}