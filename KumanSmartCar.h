#ifndef KumanSmartCar_h
#define KumanSmartCar_h

#include "Arduino.h"

class KumanSmartCar
{
  public:
    void begin(void);
    void forward(void);
    void reverse(void);
    void spinLeft(void);
    void spinRight(void);
    void stop(void);
    void soundHorn(unsigned int time);
    void flashLeftAndRightLights(unsigned int numCycles);
    void waitForButtonPress(void);
    unsigned long measureFrontDistance(void);
  private:
    int _power;
};

#endif