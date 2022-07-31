#ifndef ALT_PROGRAM

Nav n;

void setup() {
  HW::setup();
  HW::setup_serial(SERIAL_USB);
  SP("--> Start main.h");
}

void loop() {  
  HW::loop();
}

#endif
