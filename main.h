#ifndef ALT_PROGRAM

#include <Audio.h>
#include "BALibrary.h"
#include "Hardware.h"
#include "Nav.h"
#include "MyBlocks.h"

using namespace BALibrary;

AudioInputI2S        i2sIn;
AudioOutputI2S       i2sOut;
InputBlock           input_b;
OutputBlock          output_b;
Nav                  n;

AudioConnection      patch1(i2sIn,    0, input_b, 0);
AudioConnection      patch2(i2sIn,    1, input_b, 1);

AudioConnection      patch3(output_b, 0, i2sOut,  0);
AudioConnection      patch4(output_b, 1, i2sOut,  1);

void setup() {
  HW::setup();
  HW::setup_serial(SERIAL_USB);

  Serial.println("Start");
}

void loop() {  
  HW::loop();

  if ( HW::btn_left.is_pressed() )  { HW::led1(TOGGLE); }
  if ( HW::btn_right.is_pressed() ) { HW::led2(TOGGLE); }


  delay(100);

// some stuff to do with SD cards //////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#ifdef IGNORE_ME
  if ( n.not_init() ) {
//    for (int i=0; i<128; i++) buff[i] = i; // write some data to buff

    if ( !file.open("test2.txt", O_RDWR) ) {
    /*if ( !file.open("test.txt", O_RDWR) ) {*/
      Serial.println("##### Error opening file test.txt");
    }
//    file.seek(EOF);

    // buff size is number of bytes, since buff is int16_t its 2 bytes per var
//    if ( !file.write(buff, (128*2)) ) {
//      Serial.println("##### Error writing to test.txt");
//    }
//    file.sync();
//    file.rewind();

    for (int i=0; i<128; i++) buff[i] = 0; // reset buffer data

    for (int j=0; j<2; j++) {
      file.read(buff, (128*2));  // need error handling for this
      for (int i=0; i<128; i++) {
        Serial.print(i+(128*j));
        Serial.print(": ");
        Serial.println(buff[i]);
      }
    }

    file.close();

    return;
#endif
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

}

#endif
