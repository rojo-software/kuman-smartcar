# kuman-smartcar
Helper library for Arduino based Kuman Smartcar.  Provides a single class, `KumanSmartCar` with the following methods:

## `begin()`
Class initialisation

##  `forward()`
Enables motors to move car forward at a fixed speed 

##  `reverse()`
Enables motors to move car backwards at a fixed speed 

##  `spinLeft()`
Enables RH motors to turn car left at a fixed speed

##  `spinRight()`
Enables LH motors to turn car right at a fixed speed

## `stop()`
Disables both motors to stop car

## `soundHorn(unsigned int time)`
Sounds horn (buzzer) for specified number of mS

## `flashLeftAndRightLights(unsigned int numCycles)`
Flashes car lights on/off for specified number of cycles

## `waitForButtonPress()`
Pauses loop execution until button pressed

## `measureFrontDistance()`
Uses ultrasonic detector to measure distance from object in front of car.
Returns value in cm.

# TODO
Add variable speed control for motors
Add sample & hold for ultrasonic measurement
Add support for IR detector
Add support for IR line following
