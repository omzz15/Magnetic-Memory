#include <Arduino.h>

class SmoothChangePin
{
private:
    uint8_t pin;
    uint8_t risePerCycle;

    uint8_t target;
    uint8_t current;
    u_long lastUpdate;

public:
    static uint8_t minDelay; // The minimum delay between updates in microseconds (so that it can generare 1 PWM cycle before updating again)
    
    SmoothChangePin(uint8_t pin, uint8_t risePerCycle = 1)
    {
        this->pin = pin;
        this->risePerCycle = risePerCycle;
        pinMode(pin, OUTPUT);
    }
    void update()
    {
        if(this->isDone())
            return;

        u_long current = micros();
        if(lastUpdate + minDelay > current)
            delayMicroseconds(lastUpdate + minDelay - current);
        lastUpdate = current;

        if (current < target)
        {
            current += risePerCycle;
            if (current > target)
                current = target;
        }
        else if (current > target)
        {
            current -= risePerCycle;
            if (current < target)
                current = target;
        }
        analogWrite(pin, current);
    }
    void setRisePerCycle(uint8_t risePerCycle)
    {
        this->risePerCycle = risePerCycle;
    }
    void setTarget(uint8_t target)
    {
        this->target = target;
    }
    void setTargetAndWait(uint8_t target)
    {
        this->setTarget(target);
        while(!this->isDone())
            this->update();
    }
    uint8_t getTarget()
    {
        return target;
    }
    void forceSetCurrent(uint8_t current)
    {
        this->current = current;
        this->target = current;
        analogWrite(pin, current);
    }
    uint8_t getCurrent()
    {
        return current;
    }
    bool isDone()
    {
        return current == target;
    }
};