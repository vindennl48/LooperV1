#ifdef ALT_PROGRAM
#ifdef MAIN_TEMP_H

Nav n;

void setup() {
  HW::setup();
  HW::setup_serial(SERIAL_USB);
  AudioEngine::setup();
  SP("--> Start ALT main_temp.h");
}
void loop() {
  HW::loop();
  AudioEngine::loop();

  if ( HW::btn_left.is_pressed() )  HW::led1(TOGGLE);
  if ( HW::btn_right.is_pressed() ) HW::led2(TOGGLE);

  switch (n.e()) {
    case 0:
      if (digitalRead(BA_EXPAND_LED2_PIN)) {
        n.jump_to(1);
      }
      break;

    case 1:
      if (!digitalRead(BA_EXPAND_LED2_PIN)) {
        n.jump_to(2);
      }
      break;

    case 2:
      if (n.not_init()) {
        for(int i=0; i<512; i++) {
          Serial.print(i); Serial.print(": ");
          SP("b1: ", testbuf1[i], " | b2: ", testbuf2[i]);
        }
      }
      break;
  };

}

#endif
#endif
