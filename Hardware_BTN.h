#ifndef BTN_H
#define BTN_H

#define INTERVAL       25  //ms of debounce time
#define LONGPRESS_TIME 500 //ms of long-press activation

/* The states that say 'used' have ran thru a complete clock cycle.  We do not
 * want double triggers so they only stay active for 1 cycle. */
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
      // Button was pressed
      state = STATE_DOWN; 
    }
    else if ( b.rose() ) {
      /* Button was depressed.  If the button was held past the longpress_time,
       * then reset the button to neutral. We do not want to trigger an 'up'
       * event.  If the button was not held past the longpress_time, trigger an
       * 'up' event. */
      if ( state == STATE_LONG || state == STATE_LONG_USED ) state = STATE_NEUTRAL;
      else state = STATE_UP;
    }

    if ( (state == STATE_DOWN || state == STATE_DOWN_USED) &&
        b.currentDuration() > LONGPRESS_TIME )
      // if button was held down past the longpress_time
      state = STATE_LONG;
  }

  uint8_t is_pressed()      { return state == STATE_DOWN ? true : false; }
  uint8_t is_released()     { return state == STATE_UP   ? true : false; }
  uint8_t is_long_pressed() { return state == STATE_LONG ? true : false; }
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
