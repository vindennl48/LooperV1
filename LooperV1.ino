#include "Hardware.h"

Timer timer;

void setup() {
  /*pinMode(13, OUTPUT);*/
  /*pinMode(12, INPUT_PULLUP);*/

  HW::setup();
  HW::setup_serial(SERIAL_USB);
}

bool init = false;

void loop() {
  HW::loop();

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


  /*if ( HW::btn_left.isLongPressed() ) HW::board_led(TOGGLE);*/
}
