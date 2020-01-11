#include <KumanSmartCar.h>

KumanSmartCar myCar;

typedef enum {STOPPED, FORWARD, REVERSE} carState_t;
carState_t carState = STOPPED;

#define MOVING_TURN_PERIOD 200
#define STATIC_TURN_PERIOD 500

void setup()
{
  Serial.begin(9600);  
  myCar.begin();
}

void drivingModeRemoteControl(remoteControlKeys keyInput) {
    switch(keyInput) {
    case KEY_STAR:
      myCar.flashLeftAndRightLights(2);
      break;
    case KEY_HASH:
      myCar.soundHorn(100);
      delay(300);
      myCar.soundHorn(100);
      break;
    case KEY_UP:
      myCar.stop();
      myCar.forward();
      carState = FORWARD;
      break;
    case KEY_OK:
      myCar.stop();
      carState = STOPPED;
      break;
    case KEY_DOWN:
      myCar.stop();
      myCar.reverse();
      carState = REVERSE;
      break;
    case KEY_LEFT:
      if (carState == FORWARD) {
        myCar.spinLeft();
        delay(MOVING_TURN_PERIOD);
        myCar.forward();
      }

      if (carState == STOPPED) {
        myCar.flashLeftLights(1);
        myCar.spinLeft();
        delay(STATIC_TURN_PERIOD);
        myCar.stop();
      }
      
      break;
    case KEY_RIGHT:
      if (carState == FORWARD) {
        myCar.spinRight();
        delay(MOVING_TURN_PERIOD);
        myCar.forward();
      }

      if (carState == STOPPED) {
        myCar.flashRightLights(1);
        myCar.spinRight();
        delay(STATIC_TURN_PERIOD);
        myCar.stop();
      }
      break;
    
    default:
      break;              
  }
}

void drivingModeAutomatedControl(void) {

  unsigned int dist = myCar.measureFrontDistance();
  unsigned int leftObject = myCar.readLeftIRSensor();
  unsigned int rightObject = myCar.readRightIRSensor();

  if (carState == FORWARD) {
    if (dist < 20 || leftObject == LOW || rightObject == LOW){
      myCar.stop();
      myCar.soundHorn(100);
      myCar.flashLeftAndRightLights(1);
      myCar.reverse();
      delay(500);
      myCar.stop();
  
      carState = STOPPED;  
    }
  }
}

void loop()
{

  myCar.waitForButtonPress();

  while(1) { 
    remoteControlKeys remoteInput = myCar.readIRRemote(); 

    if (remoteInput != NO_KEY_PRESSED) {
      drivingModeRemoteControl(remoteInput);
    }

    drivingModeAutomatedControl();    
  }

}
