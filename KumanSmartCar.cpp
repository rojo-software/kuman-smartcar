#include "Arduino.h"
#include "KumanSmartCar.h"
#include "IRremote.h"

/*********************************************************************
**
**  Port mappings taken from Kuman schematic
**
*********************************************************************/
#define IR_RX 2
#define LEFT_LED 3
#define RIGHT_LED 4
#define RIGHT_MOTOR_EN 5
#define LEFT_MOTOR_EN 10
#define RIGHT_MOTOR_IN1 6
#define RIGHT_MOTOR_IN2 7
#define LEFT_MOTOR_IN1 8
#define LEFT_MOTOR_IN2 9
#define BUZZER 12
#define KEY 13
#define ULTRASONIC_ECHO  A1
#define ULTRASONIC_TRIGGER A0
#define RIGHT_INFRARED_LINE A2
#define LEFT_INFRARED_LINE A3
#define RIGHT_INFRARED_TRACK A4
#define LEFT_INFRARED_TRACK A5

/*********************************************************************
**
**  PWM power levels for car control:
**  Motors specced to be 6V DC max
**  Motor supply Vcc is 7.4V
**  For 8 bit PWM 6V = 206
**  is there a loss across motor driver so could increase levels?
**
*********************************************************************/
#define FORWARD_SPEED_PWM 127
#define REVERSE_SPEED_PWM 127
#define SPIN_LEFT_SPEED_PWM 180
#define SPIN_RIGHT_SPEED_PWM 180
#define MOTOR_STOPPED_PWM 0

decode_results irRemoteResults; 
IRrecv irRemote(IR_RX);

void KumanSmartCar::begin(void)
{
    pinMode(LEFT_LED, OUTPUT);
    pinMode(RIGHT_LED, OUTPUT);

    pinMode(RIGHT_MOTOR_EN, OUTPUT);
    pinMode(LEFT_MOTOR_EN, OUTPUT);
    pinMode(RIGHT_MOTOR_IN1, OUTPUT);
    pinMode(RIGHT_MOTOR_IN2, OUTPUT);
    pinMode(LEFT_MOTOR_IN1, OUTPUT);
    pinMode(LEFT_MOTOR_IN2, OUTPUT);

    pinMode(BUZZER, OUTPUT);
    pinMode(KEY, INPUT_PULLUP);
    pinMode(LEFT_LED, OUTPUT);
    pinMode(RIGHT_LED, OUTPUT);

    pinMode(ULTRASONIC_ECHO, INPUT); 
    pinMode(ULTRASONIC_TRIGGER, OUTPUT);

    pinMode(LEFT_INFRARED_TRACK, INPUT);
    pinMode(RIGHT_INFRARED_TRACK, INPUT);

    irRemote.enableIRIn();

    this->lastFrontDistance =10; // Initialise with arbitrary value

}

void setMotorEnablesForward(void) {
  digitalWrite(RIGHT_MOTOR_IN1, HIGH);
  digitalWrite(RIGHT_MOTOR_IN2, LOW);

  digitalWrite(LEFT_MOTOR_IN1, HIGH);
  digitalWrite(LEFT_MOTOR_IN2, LOW);
}

void setMotorEnablesReverse(void) {
  digitalWrite(RIGHT_MOTOR_IN1, LOW);
  digitalWrite(RIGHT_MOTOR_IN2, HIGH);

  digitalWrite(LEFT_MOTOR_IN1, LOW);
  digitalWrite(LEFT_MOTOR_IN2, HIGH);
}

void KumanSmartCar::forward(void)
{
  setMotorEnablesForward();
  analogWrite(RIGHT_MOTOR_EN, FORWARD_SPEED_PWM);
  analogWrite(LEFT_MOTOR_EN, FORWARD_SPEED_PWM);
}

void KumanSmartCar::reverse(void)
{
  setMotorEnablesReverse();
  analogWrite(RIGHT_MOTOR_EN, REVERSE_SPEED_PWM);
  analogWrite(LEFT_MOTOR_EN, REVERSE_SPEED_PWM);
}

/*
* Hard turn to left
*/
void KumanSmartCar::spinLeft(void)
{
  setMotorEnablesForward();
  analogWrite(RIGHT_MOTOR_EN, SPIN_LEFT_SPEED_PWM);
  analogWrite(LEFT_MOTOR_EN, MOTOR_STOPPED_PWM);
}

/*
* Hard turn to right
*/ 
void KumanSmartCar::spinRight(void)
{
  setMotorEnablesForward();
  analogWrite(LEFT_MOTOR_EN, SPIN_RIGHT_SPEED_PWM);
  analogWrite(RIGHT_MOTOR_EN, MOTOR_STOPPED_PWM);
}

void KumanSmartCar::stop(void)
{
  digitalWrite(RIGHT_MOTOR_EN, LOW);
  digitalWrite(LEFT_MOTOR_EN, LOW);
}

/*
 * Sound horn for specified number of milliseconds
 */
void KumanSmartCar::soundHorn(unsigned int time)
{
  digitalWrite(BUZZER, HIGH);
  delay(time);
  digitalWrite(BUZZER, LOW);
}

/*
 * Flash lights L&R for specified number of cycles
 */
void KumanSmartCar::flashLeftAndRightLights(unsigned int numCycles) 
{
  for (int cycles=0;cycles < numCycles; cycles++)
  {
    digitalWrite(LEFT_LED,HIGH);
    digitalWrite(RIGHT_LED,HIGH);
    delay(400);
    digitalWrite(LEFT_LED,LOW);
    digitalWrite(RIGHT_LED,LOW);
    delay(400);    
  } 
}

/*
 * Flash lights L for specified number of cycles
 */
void KumanSmartCar::flashLeftLights(unsigned int numCycles) 
{
  for (int cycles=0;cycles < numCycles; cycles++)
  {
    digitalWrite(LEFT_LED,HIGH);
    delay(400);
    digitalWrite(LEFT_LED,LOW);
    delay(400);    
  } 
}

/*
 * Flash lights R for specified number of cycles
 */
void KumanSmartCar::flashRightLights(unsigned int numCycles) 
{
  for (int cycles=0;cycles < numCycles; cycles++)
  {
    digitalWrite(RIGHT_LED,HIGH);
    delay(400);
    digitalWrite(RIGHT_LED,LOW);
    delay(400);    
  } 
}

/*
* Wait for PCA button to be pressed
*/ 
void KumanSmartCar::waitForButtonPress(void)
{

  // This is the wait
  while (digitalRead(KEY) == HIGH)
  {
    delay(10);
  }

  // Button debounce
  while (digitalRead(KEY) == LOW) // When the button is pressed
  {
    delay(10);
  }
}

/*
 * Measure front distance in cm
 */
unsigned int KumanSmartCar::measureFrontDistance(void)
{
  digitalWrite(ULTRASONIC_TRIGGER, LOW);    // set trig port low level for 5μs
  delayMicroseconds(5);
  digitalWrite(ULTRASONIC_TRIGGER, HIGH);  // set trig port high level for 10μs(at least 10μs)
  delayMicroseconds(10);
  digitalWrite(ULTRASONIC_TRIGGER, LOW);    // set trig port low level

  unsigned long pulseTime = pulseIn(ULTRASONIC_ECHO, HIGH); // Returns value in microseconds

  // Use formula: 2*Distance(m) =(time(s) * 344(m/s)) 
  // ==> 2 *Distance(cm) = time(μs) * 0.0344(cm/μs)
  // ==> Distance(cm) = time(μs) * 0.0172
  // Express 0.0172 as fixed point value 1124 / 65536 (2^16)
  unsigned long distance = (pulseTime * 1124) >> 16; // This gives the distance in cm

   // Realistically sensor can read upto ~200cm range.  Sometimes we get spurious results which are much larger...
  //  If this happens then use previous measurement value.
  if (distance > 1000)
	distance = this->lastFrontDistance;

  unsigned int boundedDistance = (int) distance;
  this->lastFrontDistance = boundedDistance;
  return boundedDistance;
}

/* 
* Read state of left IR sensor
*/ 
unsigned int KumanSmartCar::readLeftIRSensor(void)   
{
	unsigned int sl;
	sl = digitalRead(LEFT_INFRARED_TRACK);
	return sl;
}

/* 
* Read state of right IR sensor
*/ 
unsigned int KumanSmartCar::readRightIRSensor(void)   
{
	unsigned int sr;
	sr = digitalRead(RIGHT_INFRARED_TRACK);
	return sr;
}

/* 
* IR remote handling
*/ 
remoteControlKeys KumanSmartCar::readIRRemote(void)
{
        remoteControlKeys decodedValue = NO_KEY_PRESSED; 
        if (irRemote.decode(&irRemoteResults)) //Infrared received result
        {
	       switch(irRemoteResults.value) {
                   case 0x00FDB04F: decodedValue = KEY_0; break;
		   case 0x00FD00FF: decodedValue =  KEY_1; break;
		   case 0x00FD807F: decodedValue =  KEY_2; break;
		   case 0x00FD40BF: decodedValue =  KEY_3; break;
		   case 0x00FD20DF: decodedValue =  KEY_4; break;
                   case 0x00FDA05F: decodedValue =  KEY_5; break;
                   case 0x00FD609F: decodedValue = KEY_6; break;
                   case 0x00FD10EF: decodedValue =  KEY_7; break;
                   case 0x00FD906F: decodedValue =  KEY_8; break;
                   case 0x00FD50AF: decodedValue =  KEY_9; break;
		   case 0x00FD30CF: decodedValue = KEY_STAR; break;   
                   case 0x00FD708F: decodedValue = KEY_HASH; break;
                   case 0x00FD8877: decodedValue = KEY_UP; break;
                   case 0x00FD9867: decodedValue = KEY_DOWN; break;
                   case 0x00FD28D7: decodedValue = KEY_LEFT; break;
                   case 0x00FD6897: decodedValue = KEY_RIGHT; break;
                   case 0x00FDA857: decodedValue = KEY_OK; break;
                   default: break;
              }
             irRemote.resume(); // receive next Infrared decode
     }
	return decodedValue;
}
