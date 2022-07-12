/* This is the main() file.  The Teensy/Arduino runs void setup() first and then
 * runs void loop() at the given CPU clock speed.
 */

#include "TimerOne.h"
#include "Hardware.h"
#include "Ring.h"
#include "Nav.h"

Nav n;

Timer    timer, timer2;
Ring     ring;
int max_size       = 0;
int temp_size      = 0;
int max_final_size = 0;

#define MAX_SAMP 22050
int wav[MAX_SAMP] = {0};
int count         = 0;

void setup() {
  HW::setup();
  HW::setup_serial(SERIAL_USB);

  Timer1.initialize(22);
  Timer1.attachInterrupt(isr);
}

void loop() {
  HW::loop();

  switch ( n.e() ) {
    case 0:
      if ( n.not_init() ) {
        timer = millis();
      } else {

        if ( !ring.is_empty() ) {
          wav[count] = ring.get();
          count++;
        }

        if ( count >= MAX_SAMP ) n.jump_to(1);
      }
      break;

    case 1:
      if ( n.not_init() ) {
        timer2 = millis() - timer;
        max_final_size = max_size;

        for ( int i=0; i<MAX_SAMP; i++ ) {
          Serial.print(i);
          Serial.print(": ");
          Serial.println(wav[i]);
        }

        Serial.println("--------------------");
        Serial.print(timer2);
        Serial.println("MS");
        Serial.println("--------------------");
        Serial.print(max_final_size);
        Serial.println(" = Max Buff Size");
      } else { }
      break;
  };
}

void isr() {
  ring.insert( analogRead(PinInput) );

  // find largest size buffer
  temp_size = ring.size();
  max_size  = temp_size > max_size ? temp_size : max_size;
}
