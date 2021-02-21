#include <Arduino.h>

void setup()
{
    Serial.begin(9600);
}

byte counter = 0;

int lastValue = 0;

void loop()
{
    int value = analogRead(A2);

    if (lastValue - 10 <= value && value <= lastValue + 10)
    {
        lastValue = value;
        return;
    }

    lastValue = value;

    if (counter < 10)
    {
        Serial.print(value);
        Serial.print(" ");
        counter++;
    }
    else
    {
        Serial.println(value);
        counter = 0;
    }
}