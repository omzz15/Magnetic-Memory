#include <Arduino.h>

class SoftwareInterrupt
{
    private:
        uint8_t pin;
        bool lastState;
        void (*onChange)(uint8_t, bool);
        uint8_t mode;
    public:
        SoftwareInterrupt(uint8_t pin, void (*onChange)(uint8_t, bool) = NULL, uint8_t mode = CHANGE);
        void setCallback(void (*onChange)(uint8_t, bool));
        void setMode(uint8_t mode);
        void update();
};