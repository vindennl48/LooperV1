/* This header deals with pin definitions as well as higher level definitions of
 * the pedal.
 * */

#ifndef DEFINITIONS_H
#define DEFINITIONS_H

// Used for detecting MS time differences
typedef unsigned long Timer;

// State Defines
#define OFF    0
#define ON     1
#define TOGGLE 2

// Pin defines
#define PinButton1    12
#define PinButton2    10

#define PinVolume     14  // Analog pin for potentiometer
#define PinInput      15  // Analog pin for guitar input

#define PinSwitch1_1  11
#define PinSwitch1_2  11
#define PinSwitch2_1  11
#define PinSwitch2_2  11
#define PinSwitch3_1  11
#define PinSwitch3_2  11
#define PinSwitch4_1  11
#define PinSwitch4_2  11

#define PinLEDBoard   13 // LED on the Teensy/Arduino board
#define PinLED1_1     16
#define PinLED1_2     17
#define PinLED1_3     18
#define PinLED2_1     19
#define PinLED2_2     20
#define PinLED2_3     21


#endif
