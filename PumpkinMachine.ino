#include <Arduino.h>

const int buttonPin = 8;
const int pumpPin = 3;
const int sensorPowerPin = 5;
const int sensorReadPin = A0;
int previousButtonState = LOW;
const unsigned long checkInterval = 60000;
const unsigned long pumpWorkTime = 5000;
const int humidityThreshold = 500;

void powerUpSensor()
{
    digitalWrite(sensorPowerPin, HIGH);
}

void powerDownSensor()
{
    digitalWrite(sensorPowerPin, LOW);
}

int readMoistureValue()
{
    powerUpSensor();
    delay(100);
    int sensorReadValue = analogRead(sensorReadPin);
    Serial.print("Humidity sensor read: ");
    Serial.println(sensorReadValue);
    powerDownSensor();
    return sensorReadValue;
}

bool isMoistureHighEnough()
{
    return readMoistureValue() < humidityThreshold;
}

void turnOnThePump()
{
    if (digitalRead(pumpPin) == HIGH) {
        digitalWrite(pumpPin, LOW);
        Serial.println("Turning the pump ON");
    }
}

void turnOffThePump()
{
    if (digitalRead(pumpPin) == LOW) {
        digitalWrite(pumpPin, HIGH);
        Serial.println("Turning the pump OFF");
    }
}

void setup() {
    Serial.begin(9600);
    delay(5000);
    Serial.println("Welcome to the ultimate pumpkin watering machine");
    pinMode(pumpPin, OUTPUT);
    pinMode(buttonPin, INPUT);
    pinMode(sensorPowerPin, OUTPUT);

    turnOffThePump();
}

bool buttonIsPressed()
{
    int buttonState = digitalRead(buttonPin);
    if (previousButtonState != buttonState) {
        if (buttonState == HIGH) {
            Serial.println("Manual watering active");
        } else {
            Serial.println("Manual watering inactive");
        }
    }
    previousButtonState = buttonState;
    return buttonState == HIGH;
}

bool isTimeForSensorCheck()
{
    unsigned long modulo = millis() % (checkInterval);
    return (modulo > checkInterval - pumpWorkTime);
}

void runPumpCycle()
{
    turnOnThePump();
    delay(pumpWorkTime);
    turnOffThePump();
}

void loop() {
    if (buttonIsPressed()) {
        turnOnThePump();
        return;
    }
    if (isTimeForSensorCheck() && !isMoistureHighEnough()) {
        runPumpCycle();
        return;
    }
    turnOffThePump();
}
