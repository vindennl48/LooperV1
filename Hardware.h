/* I believe this library is fairly self-explanatory. Used to keep all of the
 * hardware peripherals in one location */

#ifndef HARDWARE_H
#define HARDWARE_H

#include "BTN.h"
#include "Pot.h"
#include "Definitions.h"

#define SERIAL_USB  0
#define SERIAL_MIDI 1

struct HW {
  static BTN btn_left;
  static BTN btn_right;
  static Pot pot_volume;
  //static Switch3Way sw3_volume, sw3_layers, sw3_stop sw3_record;

  static void setup() {
    pinMode(PinLEDBoard, OUTPUT);
    board_led(OFF);
    
    HW::btn_left.setup(PinButton1, true);
    HW::btn_right.setup(PinButton2, true);

    HW::pot_volume.setup(PinVolume);

    //sw3_volume.setup(PinSwitch1_1, PinSwitch1_2);
    //sw3_layers.setup(PinSwitch2_1, PinSwitch2_2);
      //sw3_stop.setup(PinSwitch3_1, PinSwitch3_2);
    //sw3_record.setup(PinSwitch4_1, PinSwitch4_2);
  }

  static void setup_serial(uint8_t type) {
    if      ( type == SERIAL_USB  ) Serial.begin(38400);
    else if ( type == SERIAL_MIDI ) Serial.begin(31250);
  }

  static void loop() {
    btn_left.loop();
    btn_right.loop();

    pot_volume.loop();

    //sw3_volume.loop();
    //sw3_layers.loop();
    //sw3_stop.loop();
    //sw3_record.loop();
  }

  static void board_led(uint8_t state) {
    if ( state == TOGGLE ) digitalWrite(PinLEDBoard, !digitalRead(PinLEDBoard));
    else digitalWrite(PinLEDBoard, state);
  }
};

#endif
