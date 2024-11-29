#include <math.h>
#include <Arduino.h> // Include Arduino functions like constrain and map

int lSpeed = 0;
int rSpeed = 0;

struct speed {
  int lSpeed;
  int rSpeed;
};

float Cubic(const float x, const float weight) { // By Mimir Games
    return weight * x * x * x + (1.0 - weight) * x;
}

int joystickCubicScaledDeadband(int pos, int limit, float limitF) { // By Mimir Games
    float x = pos / limitF;
    const float deadbandCutoff = 0.3921;
    const float weight = 0.8;

    if (abs(x) <= deadbandCutoff) {
        return 0;
    } else {
        return limit * ((Cubic(x, weight) - (abs(x) / x) * Cubic(deadbandCutoff, weight)) / 
                        (1.0 - Cubic(deadbandCutoff, weight)));
    }
}

speed feetMovement(int xPos, int yPos, float speedMod) {
    static int limit = 440;
    static float limitF = 440.0;

    // Create a variable of type speed
    speed motorSpeeds;

    yPos = constrain(yPos, -limit, limit);
    xPos = constrain(xPos, -limit, limit);
    xPos = joystickCubicScaledDeadband(xPos, limit, limitF);
    yPos = joystickCubicScaledDeadband(yPos, limit, limitF);

    int yPoint = yPos * yPos;
    int xPoint = xPos * xPos;
    long double py = xPoint + yPoint;
    int magnitude = sqrt(py);
    magnitude = constrain(magnitude, 0, 440);

    int degree = abs(-180 + round((180 * atan2(-1 * (yPos + 36), xPos)) / 3.1415));

    if (degree >= 0 && degree <= 180) {
        if (degree <= 90) {
            lSpeed = magnitude;
            float rad = (degree * 3.1415) / 180;
            rSpeed = magnitude * sin((2 * rad) - 1.5708);
        } else {
            rSpeed = magnitude;
            float rad = (degree * 3.1415) / 180;
            lSpeed = magnitude * sin((2 * rad) - 1.5708);
        }
    } else {
        if (degree >= 270) {
            lSpeed = -magnitude;
            float rad = (degree * 3.1415) / 180;
            rSpeed = -magnitude * sin((2 * rad) - 1.5708);
        } else {
            rSpeed = -magnitude;
            float rad = (degree * 3.1415) / 180;
            lSpeed = -magnitude * sin((2 * rad) - 1.5708);
        }
    }

    lSpeed = speedMod * map(lSpeed, -limit, limit, -255, 255);
    rSpeed = speedMod * map(rSpeed, -limit, limit, -255, 255);

    // Assign values to the struct
    motorSpeeds.lSpeed = lSpeed;
    motorSpeeds.rSpeed = rSpeed;

    return motorSpeeds;
}
