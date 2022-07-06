#ifndef BTN_H
#define BTN_H

#include <Bounce2.h>

#define INTERVAL       25  //ms
#define LONGPRESS_TIME 500 //ms

#define STATE_NEUTRAL   0
#define STATE_DOWN      1
#define STATE_DOWN_USED 2
#define STATE_UP        3
#define STATE_LONG      4
#define STATE_LONG_USED 5

struct BTN {
  Bounce  b;
  uint8_t state;

  void setup(uint8_t pin, uint8_t pullup) {
    this->state = STATE_NEUTRAL;

    b = Bounce();
    b.attach(pin, pullup ? INPUT_PULLUP : INPUT);
    b.interval(INTERVAL);
  }

  void loop() {
    b.update();

    // Update the state from last cycle
    if      ( state == STATE_UP )   state = STATE_NEUTRAL;
    else if ( state == STATE_DOWN ) state = STATE_DOWN_USED;
    else if ( state == STATE_LONG ) state = STATE_LONG_USED;

    if ( b.fell() ) {
      state = STATE_DOWN; 
    }
    else if ( b.rose() ) {
      if ( state == STATE_LONG || state == STATE_LONG_USED ) state = STATE_NEUTRAL;
      else state = STATE_UP;
    }

    if ( (state == STATE_DOWN || state == STATE_DOWN_USED) &&
        b.currentDuration() > LONGPRESS_TIME )
      state = STATE_LONG;
  }

  uint8_t isPressed()     { return state == STATE_DOWN ? true : false; }
  uint8_t isReleased()    { return state == STATE_UP   ? true : false; }
  uint8_t isLongPressed() { return state == STATE_LONG ? true : false; }
};

#undef INTERVAL
#undef LONGPRESS_TIME

#undef STATE_NEUTRAL
#undef STATE_DOWN
#undef STATE_DOWN_USED
#undef STATE_UP
#undef STATE_LONG
#undef STATE_LONG_USED

#endif
