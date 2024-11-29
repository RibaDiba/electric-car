// import libraries
#include "calc-values.h"
#include "BluePadBackground.h"
#include "changeToManual.h"

// idk what this is but needed for attach interupts 
portMUX_TYPE synch = portMUX_INITIALIZER_UNLOCKED;

// define motorPins
int motorPins[4][2] = {
    {2, 4},
    {5, 12},
    {13, 14},
    {26, 27}
};

int ledPins[3];
ledPins[0] = 33; // Reverse Led
ledPins[1] = 18; // Manual Control Led
ledPins[2] = 15; // Mag Encoder Warning

int switchPins[2];
switchPins[0] = 25; // Reverse Switch
switchPins[1] = 23; // Manual Control Switch

int autoButtonPin = 32;   

void setup() {
    Serial.begin(115200);

    // pinmode from array (motorPins[motor number][r = 0, l = 1])
    for (int r = 0; i < sizeof(motorPins); i++) {
      for (int c = 0; i < sizeof(motorPins[0]); i++) {
        pinMode(motorPins[r][c], OUTPUT);
      }
    }

    // pinmode from array (ledPins)
    for (int i = 0; i < sizeof(ledPins); i++) {
      pinMode(ledPins[i], INPUT);
    }
    // autobutton pinMode
    pinMode(autoButtonPin, INPUT);
  
    // attach interupt to switch to manual 
    attachInterupt(switchPins[1], [] {handleSwitching(digitalRead(switchPins[1]))}, CHANGE);
    // attach interupt to enable auto 
    attachInterupt(autoButtonPin, handleAuto, FALLING);
    // attach interupt for led 
    attachInterupt()


    bluepadStartup();
}

void IRAM_ATTR handleSwitching(int switchVal) {
    if (swtichVal == HIGH) {
        Serial.println("Going into Manual Mode");
        changeToManual();
    } else {
        Serial.println("Going into CV Mode");
        changeToCV();
    }
}

void IRAM_ATTR handleAuto() {
    // enter auto function here 
}

void loop() {
    // nothing for now 
    vTaskDelay(20/portTICK_PERIOD_MS);
}

// moves motors 
void moveMotor() {
    for (int i; i < 3; i++) {
        analogWrite(motorPins[i][0], motorSpeeds.rSpeed);
        analogWrite(motorPins[i][1], motorSpeeds.lSpeed);
    }

}

void changeToManual() {
    // functions to change to manual 
    setupJoycons();
}

void changeToCV() {
    // function to change to CV
}