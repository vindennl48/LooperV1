/* This is the main() file.  The Teensy/Arduino runs void setup() first and then
 * runs void loop() at the given CPU clock speed.
 */

#include "Hardware.h"

Timer timer;

/* Used to setup hardware peripherals and default parameters */
void setup() {
  /*pinMode(13, OUTPUT);*/
  /*pinMode(12, INPUT_PULLUP);*/

  HW::setup();
  HW::setup_serial(SERIAL_USB);
}

bool init = false;  // testing values

/* This is the main loop of the program.  Everything that runs lives inside of
 * this loop. */
void loop() {
  /*  This runs all of the buttons/knobs/switches loop programs.  Just an easier
   *  way of keeping things tidy. */
  HW::loop();

  // Test code to make sure the Pot library works
  if ( !init ) {
    HW::pot_volume.set_value(100);

    timer = millis();

    init = true;
  } else {

    if ( timer + 250 < millis() ) {
      Serial.print("val is: ");
      Serial.println( HW::pot_volume.get_value() );
      timer = millis();
    }

  }

  // Test code to make sure the BTN library works
  /*if ( HW::btn_left.isLongPressed() ) HW::board_led(TOGGLE);*/
}
