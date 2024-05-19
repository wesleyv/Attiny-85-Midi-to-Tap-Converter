#include <Arduino.h>

uint32_t MicrosArray[4];

#define TRIGPULSE 10000

void initializePin(uint8_t pin)
{
    pinMode(pin, OUTPUT);
    digitalWrite(pin, LOW);
}

void PinOn(uint8_t pin, uint8_t microsarrayaddress, uint32_t now)
{
    digitalWrite(pin, HIGH);
    MicrosArray[microsarrayaddress] = now + TRIGPULSE;
}

void PinOff(uint8_t pin, uint8_t microsarrayaddress, uint32_t now)
{
    if (MicrosArray[microsarrayaddress] < now)
    {
        digitalWrite(pin, LOW);
    }
}