#include "BluePadBackground.h"
#include "calc-values.h"

// these will store raw values from the joysticks 
int xPos = 0;
int yPos = 0;

// changes max speed + curve 
static float speedModifier = 0.33;

// store for disconnection
static bool disconnected = false;

// this struct stores the motor speeds post-calculation 
speed MotorSpeeds;

// setup bluepad
void setupJoycons() {
    bluepadStartup();
    loopJoycons();
}

// delay
bool debounce (int delayTime){
  static unsigned long previousTime = millis();
  if (millis()-previousTime>delayTime)
  {
    previousTime = millis();
    return true;
  }
  else
  {
    return false;
  } 
}

// loop here
void loopJoycons() {
    bool dataUpdated = BP32.update();
    processControllers();

    // serial prints for speeds
    if (disconnected == false) {
        printf("Left Motor Speed is: %i and Right Motor Speed is: %i)\n", MotorSpeeds.lSpeed, MotorSpeeds.rSpeed);
    } else {
        printf("No Controller Connected");
    }

    moveMotors();

    // delay for this function (will end once user switches)
    vTaskDelay(20/portTICK_PERIOD_MS);
}

// debug joycons + run function if connected
void processControllers() {
  static unsigned long lastUpdate = millis();
    for (auto myController : myControllers) {
      if (myController && myController->isConnected()) {
        if (myController->hasData()) {
          disconnected=false;
          processGamepad(myController);
          lastUpdate=millis();
        }
        else {
            // No data received from the controller.
            if ((millis() - lastUpdate) > 1000) {
              if (!disconnected==true) {
                Serial.println("Controller out of range");
                lastUpdate = millis();
                disconnected = true;
              }
            }
        }
      } 
  }
}

void get_values(ControllerPtr ctl) {
    ControllerProperties properties = ctl->getProperties();
    xPos = ctl->axisY();
    yPos = ctl->axisX();
}

// read the raw values from the controller and send them over for calculations
void processGamepad(ControllerPtr ctl) {
  get_values(ctl);
  motorSpeeds = feetMovement(xPos,yPos,speedModifier);
}

void moveMotors() {
    for (int i; i < 3; i++) {
        // TODO: this is prob wrong, change it later
        analogWrite(motorPins[i][0], motorSpeeds.rSpeed);
        analogWrite(motorPins[i][1], motorSpeeds.lSpeed);
    }
}
