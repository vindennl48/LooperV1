#ifdef ALT_PROGRAM
#ifdef MAIN_TEMP_H

void setup() {
  HW::setup();
  HW::setup_serial(SERIAL_USB);
}
void loop() {
  HW::loop();
}

#endif
#endif
