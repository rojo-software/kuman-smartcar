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

void KumanSmartCar::begin(void)
{
    IRrecv irrecv( IR_RX);

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

    pinMode( ULTRASONIC_ECHO, INPUT); 
    pinMode(ULTRASONIC_TRIGGER, OUTPUT);

}

void KumanSmartCar::forward(void)
{
  digitalWrite(RIGHT_MOTOR_IN1, HIGH);
  digitalWrite(RIGHT_MOTOR_IN2, LOW);

  digitalWrite(LEFT_MOTOR_IN1, HIGH);
  digitalWrite(LEFT_MOTOR_IN2, LOW);

  analogWrite(RIGHT_MOTOR_EN, FORWARD_SPEED_PWM);
  analogWrite(LEFT_MOTOR_EN, FORWARD_SPEED_PWM);
}

void KumanSmartCar::reverse(void)
{
  digitalWrite(RIGHT_MOTOR_IN1, LOW);
  digitalWrite(RIGHT_MOTOR_IN2, HIGH);

  digitalWrite(LEFT_MOTOR_IN1, LOW);
  digitalWrite(LEFT_MOTOR_IN2, HIGH);

  analogWrite(RIGHT_MOTOR_EN, REVERSE_SPEED_PWM);
  analogWrite(LEFT_MOTOR_EN, REVERSE_SPEED_PWM);
}

void KumanSmartCar::spinLeft(void)
{
  digitalWrite(RIGHT_MOTOR_IN1, HIGH);
  digitalWrite(RIGHT_MOTOR_IN2, LOW);

  analogWrite(RIGHT_MOTOR_EN, SPIN_LEFT_SPEED_PWM);
}

void KumanSmartCar::spinRight(void)
{
  digitalWrite(LEFT_MOTOR_IN1, HIGH);
  digitalWrite(LEFT_MOTOR_IN2, LOW);

  analogWrite(LEFT_MOTOR_EN, SPIN_RIGHT_SPEED_PWM);
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
void KumanSmartCar::flashLeftAndRightLights(unsigned int numCycles) {
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
unsigned long KumanSmartCar::measureFrontDistance(void)
{
  digitalWrite(ULTRASONIC_TRIGGER, LOW);    // set trig port low level for 5μs
  delayMicroseconds(5);
  digitalWrite(ULTRASONIC_TRIGGER, HIGH);  // set trig port high level for 10μs(at least 10μs)
  delayMicroseconds(10);
  digitalWrite(ULTRASONIC_TRIGGER, LOW);    // set trig port low level

  unsigned long pulseTime = pulseIn(ULTRASONIC_ECHO, HIGH);
  unsigned long distance = (pulseTime * 1124) >> 16; // This gives the distance in cm

  // TODO bound distance to be realistic, sample & hold in case of spurious measurement?

  return distance;
}