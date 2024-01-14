#include <SoftwareInterrupt.h>

SoftwareInterrupt::SoftwareInterrupt(uint8_t pin, void (*onChange)(uint8_t, bool), uint8_t mode){
    this->pin = pin;
    this->onChange = onChange;
    this->mode = mode;
}

void SoftwareInterrupt::setCallback(void (*onChange)(uint8_t, bool)){
    this->onChange = onChange;
}

void SoftwareInterrupt::setMode(uint8_t mode){
    this->mode = mode;
}

void SoftwareInterrupt::update(){
    bool currentState = digitalRead(pin);
    if (currentState == lastState)
        return;

    lastState = currentState;

    switch (mode)
    {
        case RISING:
            if(currentState)
                onChange(pin, true);
            break;
        case FALLING:
            if(!currentState)
                onChange(pin, false);
            break;
        case CHANGE:    
            onChange(pin, currentState);
            break;
        default:
            break;
    }
}
