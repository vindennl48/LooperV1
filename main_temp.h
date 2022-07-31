#ifdef ALT_PROGRAM
#ifdef MAIN_TEMP_H

Nav n;

void setup() {
  HW::setup();
  HW::setup_serial(SERIAL_USB);

  SP("--> Start ALT main_temp.h");
}
void loop() {
  HW::loop();
}

#endif
#endif
