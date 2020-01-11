#ifndef KumanSmartCar_h
#define KumanSmartCar_h

#include "Arduino.h"

typedef enum {KEY_0, KEY_1, KEY_2, KEY_3, KEY_4, KEY_5, KEY_6, KEY_7, KEY_8, KEY_9, KEY_STAR, KEY_HASH, KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT, KEY_OK, NO_KEY_PRESSED} remoteControlKeys;

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
    void flashLeftLights(unsigned int numCycles);
    void flashRightLights(unsigned int numCycles);
    void waitForButtonPress(void);
    unsigned int measureFrontDistance(void);
    unsigned int readLeftIRSensor(void);
    unsigned int readRightIRSensor(void);     
    remoteControlKeys readIRRemote(void);
  private:
    unsigned int lastFrontDistance;
};

#endif