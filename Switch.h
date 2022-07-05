#ifndef SWITCH_H
#define SWITCH_H

#include "Definitions.h"

struct Switch3Way {
  uint8_t pin1, pin2;

  void setup(uint8_t pin1, uint8_t pin2, uint8_t pullup=true) {
    this->pin1 = pin1;
    this->pin2 = pin2;
    pinMode(pin1, pullup ? INPUT_PULLUP : INPUT);
    pinMode(pin2, pullup ? INPUT_PULLUP : INPUT);
  }

  uint8_t isState(uint8_t state) {
    if ( state == getState() ) return true;
    return false;
  }

  uint8_t getState() {
    // needs an updated map after testing
    uint8_t pin1_state = digitalRead(pin1);
    uint8_t pin2_state = digitalRead(pin2);

    if ( !pin1_state &&  pin2_state ) return CENTER;
    if ( !pin1_state && !pin2_state ) return RIGHT;

    //if ( pin1_state && !pin2_state )  return LEFT;
    return LEFT; // the only other possible state (default state)
  }
};

#endif
