#ifndef HARDWARE_H
#define HARDWARE_H

#include "BTN.h"
#include "Definitions.h"

struct HW {
  static BTN btn_left, btn_right;
  //static Switch3Way sw3_volume, sw3_layers, sw3_stop sw3_record;

  static void setup() {
    pinMode(PinLEDBoard, OUTPUT);
    
    btn_left.setup(PinButton1);
    btn_right.setup(PinButton2);

    //sw3_volume.setup(PinSwitch1_1, PinSwitch1_2);
    //sw3_layers.setup(PinSwitch2_1, PinSwitch2_2);
      //sw3_stop.setup(PinSwitch3_1, PinSwitch3_2);
    //sw3_record.setup(PinSwitch4_1, PinSwitch4_2);
  }

  static void loop() {
    btn_left.loop();
    btn_right.loop();

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
