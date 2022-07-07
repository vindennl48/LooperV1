/* This library is for using 3 way toggle switches with an on/on/on pattern */

#ifndef SWITCH_H
#define SWITCH_H

#define LEFT   0
#define CENTER 1
#define RIGHT  2

struct Switch3Way {
  uint8_t pin1, pin2;

  /* We only need 2 pins to decide what position the switch is in.  Position 1
   * has pin 1 on, position 2 has pin 2 on, position 3 has pin 1 and 2 off */
  void setup(uint8_t pin1, uint8_t pin2, uint8_t pullup=true) {
    this->pin1 = pin1;
    this->pin2 = pin2;
    pinMode(pin1, pullup ? INPUT_PULLUP : INPUT);
    pinMode(pin2, pullup ? INPUT_PULLUP : INPUT);
  }

  /* Return true or false if the provided state equals the current switch state */
  uint8_t isState(uint8_t state) {
    if ( state == getState() ) return true;
    return false;
  }

  /* Because midi and presets will be able to virtually change the switch
   * position, this function will need to be re-written similar to how the Pot
   * library is written.  However, for now this will at least get the job done.
   * */
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
