#ifndef POT_H
#define POT_H


/* The actual max value from an analog input is 1024, however, if we want to use
 * a different max value, say 126, we need to do 126/1024 to get the conversion
 * factor.  This also allows a bit of debounce when reducing the resolution like
 * this. 126 was chosen because this is the max MIDI value as well. */
#define CONVERT          0.12305  // conversion factor from 126/1024
#define GET_MAX_VALUE(v) (uint8_t)round(v * CONVERT)
#define SET_MAX_VALUE(v) (uint16_t)round(v / CONVERT)
/* WINDOW filters out noise in the ADC when a value is virtaully set by MIDI or
 * by a preset. We need a bit of leeway before the user turns the knob and
 * changes the value from a virtual one. */
#define WINDOW 10


struct Pot {
  /* The value 'is_set' refers to if the Pot was virtually set by MIDI or by a
   * preset.  Obviously if this is the case, the physical knob position does not
   * reflect the actual value of the Pot.  */
  uint16_t val, old_val, new_val;
  uint8_t  pin, is_set;

  void setup(uint8_t pin) {
    this->pin = pin;
    val = old_val = analogRead(pin);
    is_set = false;
  }

  void loop() {
    new_val = analogRead(pin);

    if ( is_set ) {
      /* The knob needs to be turned past the WINDOW setpoint in order to break
       * from the virtual value. */
      if ( old_val > new_val + WINDOW || old_val < new_val - WINDOW ) {
        is_set = false;
      }
      //digitalWrite(13, true);  // Used for testing
    }
    else {
      val = new_val; 
      //digitalWrite(13, false);  // Used for testing
    }

  }

  void set_value(uint8_t x) {
    val    = SET_MAX_VALUE(x);
    is_set = true;
  }

  uint16_t get_value() {
    return GET_MAX_VALUE(val);
  }
};


#undef CONVERT
#undef GET_MAX_VALUE
#undef SET_MAX_VALUE
#undef WINDOW

#endif
