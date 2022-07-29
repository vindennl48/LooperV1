
#include <Audio.h>
#include "BALibrary.h"
#include "Hardware.h"
#include "Nav.h"
#include "MyFirstAudioBlock.h"
#include "Storage.h"

using namespace BALibrary;

AudioInputI2S        i2sIn;
AudioOutputI2S       i2sOut;
MyFirstAudioBlock    dumbBlock;
Nav                  n;

// Audio Connections: name(channel)
// - Setup a mono signal chain, send the mono signal to both output channels in case you are using headphone
// i2sIn(0) --> i2sOut(0)
// i2sIn(1) --> i2sOut(1)

/*AudioConnection      patch0(i2sIn, 0, i2sOut, 0);*/
/*AudioConnection      patch1(i2sIn, 0, i2sOut, 1);*/

AudioConnection      patch0(i2sIn, 0, dumbBlock, 0);
AudioConnection      patch1(dumbBlock, 0, i2sOut, 0);
AudioConnection      patch2(dumbBlock, 0, i2sOut, 1);

// ExFile file;
// int16_t buff[128];

void setup() {
  HW::setup();
  HW::setup_serial(SERIAL_USB);

  Storage::setup();
}

void loop() {  
  HW::loop();

  if ( HW::btn_left.is_pressed() )  { HW::led1(TOGGLE); }
  if ( HW::btn_right.is_pressed() ) { HW::led2(TOGGLE); }

//    if ( n.not_init() ) {
//  //    for (int i=0; i<128; i++) buff[i] = i; // write some data to buff
//  
//      if ( !file.open("test2.txt", O_RDWR) ) {
//      /*if ( !file.open("test.txt", O_RDWR) ) {*/
//        Serial.println("##### Error opening file test.txt");
//      }
//  //    file.seek(EOF);
//  
//      // buff size is number of bytes, since buff is int16_t its 2 bytes per var
//  //    if ( !file.write(buff, (128*2)) ) {
//  //      Serial.println("##### Error writing to test.txt");
//  //    }
//  //    file.sync();
//  //    file.rewind();
//  
//      for (int i=0; i<128; i++) buff[i] = 0; // reset buffer data
//  
//      for (int j=0; j<2; j++) {
//        file.read(buff, (128*2));  // need error handling for this
//        for (int i=0; i<128; i++) {
//          Serial.print(i+(128*j));
//          Serial.print(": ");
//          Serial.println(buff[i]);
//        }
//      }
//  
//      file.close();
//  
//      return;
//    }

}




// /* This is the main() file.  The Teensy/Arduino runs void setup() first and then
//  * runs void loop() at the given CPU clock speed.
//  */
// 
// #include "TimerOne.h"
// #include "Hardware.h"
// #include "Ring.h"
// #include "Nav.h"
// 
// Nav n;
// 
// Timer    timer, timer2;
// Ring     ring;
// int max_size       = 0;
// int temp_size      = 0;
// int max_final_size = 0;
// 
// #define MAX_SAMP 22050
// int wav[MAX_SAMP] = {0};
// int count         = 0;
// 
// void setup() {
//   HW::setup();
//   HW::setup_serial(SERIAL_USB);
// 
//   Timer1.initialize(22);
//   Timer1.attachInterrupt(isr);
// }
// 
// void loop() {
//   HW::loop();
// 
//   switch ( n.e() ) {
//     case 0:
//       if ( n.not_init() ) {
//         timer = millis();
//       } else {
// 
//         if ( !ring.is_empty() ) {
//           wav[count] = ring.get();
//           count++;
//         }
// 
//         if ( count >= MAX_SAMP ) n.jump_to(1);
//       }
//       break;
// 
//     case 1:
//       if ( n.not_init() ) {
//         timer2 = millis() - timer;
//         max_final_size = max_size;
// 
//         for ( int i=0; i<MAX_SAMP; i++ ) {
//           Serial.print(i);
//           Serial.print(": ");
//           Serial.println(wav[i]);
//         }
// 
//         Serial.println("--------------------");
//         Serial.print(timer2);
//         Serial.println("MS");
//         Serial.println("--------------------");
//         Serial.print(max_final_size);
//         Serial.println(" = Max Buff Size");
//       } else { }
//       break;
//   };
// }
// 
// void isr() {
//   ring.insert( analogRead(PinInput) );
// 
//   // find largest size buffer
//   temp_size = ring.size();
//   max_size  = temp_size > max_size ? temp_size : max_size;
// }
