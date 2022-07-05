#ifndef BTN_H
#define BTN_H

#include <Bounce2.h>

#define INTERVAL       25  //ms
#define LONGPRESS_TIME 500 //ms

#define ACTIVATE_ON_DOWN 0
#define ACTIVATE_ON_UP   1

#define BSTATE_NEUTRAL   0
#define BSTATE_DOWN      1
#define BSTATE_DOWN_USED 2
#define BSTATE_UP        3
#define BSTATE_LONG      4
#define BSTATE_LONG_USED 5

struct BTN {
  Bounce  btn;
  uint8_t state;

  void setup(uint8_t pin, uint8_t pullup=true) {
    this->state = BSTATE_NEUTRAL

    btn = Bounce();
    btn.attach(pin, pullup ? INPUT_PULLUP : INPUT);
    btn.interval(INTERVAL);
  }

  void loop() {
    btn.update();

    // Update the state from last cycle
    if      ( state == BSTATE_UP )   state = BSTATE_NEUTRAL;
    else if ( state == BSTATE_DOWN ) state = BSTATE_DOWN_USED;
    else if ( state == BSTATE_LONG ) state = BSTATE_LONG_USED;

    if ( btn.fell() ) state = BSTATE_DOWN;
    else { // btn.rose()
      if ( state == BSTATE_LONG || state == BSTATE_LONG_USED ) state = BSTATE_NEUTRAL;
      else state = BSTATE_UP;
    }

    if ( (state == BSTATE_DOWN || state == BSTATE_DOWN_USED) &&
        btn.currentDuration() > LONGPRESS_TIME )
      state = BSTATE_LONG;
  }

  void isPressed()     state == BSTATE_DOWN ? true : false;
  void isReleased()    state == BSTATE_UP   ? true : false;
  void isLongPressed() state == BSTATE_LONG ? true : false;
};

#endif
