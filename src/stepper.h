#ifndef stepper_h
#define stepper_h

#include <Arduino.h>

enum DIRECTION {
    FORWARD,
    BACKWARD
};

class Stepper {
    private:
        int _pulsePin;
        int _directionPin;
        int _interruptPin;
        void move();
        void setMotorDirection(DIRECTION toDirection);
    public:
        Stepper(int pulsePin, int directionPin, int interruptPin, double frequency);
        void setFrequency(double frequency);
        void move(DIRECTION toDirection, int steps);
        void handle();
        void stop();
        void goToZero();
        int32_t getPosition();
        bool moving();
        void resetZeroPosition();
};

#endif