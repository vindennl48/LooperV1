/* This header deals with pin definitions as well as higher level definitions of
 * the pedal.
 * */

#ifdef INCLUDE_STANDARD_DEFINITIONS
#ifndef DEFINITIONS_H
#define DEFINITIONS_H

// Helper macros
#define GET_MACRO(_1,_2,_3,NAME,...) NAME  // overload helper
#define SP(...) GET_MACRO(__VA_ARGS__,SP3,SP2,SP1)(__VA_ARGS__) // overload helper
#define SP1(x)     Serial.println(x)
#define SP2(x,y)   Serial.print(x); Serial.println(y)
#define SP3(x,y,z) Serial.print(x); Serial.print(y); Serial.println(z)

// Used for detecting MS time differences
typedef unsigned long Timer;

// Global vars
#define NUM_LAYERS 4  // max num of recordable layers

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
#endif
