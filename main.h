#ifndef ALT_PROGRAM

#include <Audio.h>
#include "BALibrary.h"
#include "Hardware.h"
#include "Buffers.h"
#include "Nav.h"
#include "MyBlocks.h"
#include "SdFat.h"

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

#define SPI_CLOCK SD_SCK_MHZ(50)
// Try to select the best SD card configuration.
#if HAS_SDIO_CLASS
#define SD_CONFIG SdioConfig(FIFO_SDIO)
#elif ENABLE_DEDICATED_SPI
#define SD_CONFIG SdSpiConfig(SDCARD_SS_PIN, DEDICATED_SPI, SPI_CLOCK)
#else  // HAS_SDIO_CLASS
#define SD_CONFIG SdSpiConfig(SDCARD_SS_PIN, SHARED_SPI, SPI_CLOCK)
#endif  // HAS_SDIO_CLASS

SdExFat sd;
ExFile  file;

void setup() {
  HW::setup();
  HW::setup_serial(SERIAL_USB);
  sd.begin(SD_CONFIG);

  SP("--> Start");
}

bool is_recording = false;
int16_t buff[AUDIO_BLOCK_SAMPLES] = {0};

void loop() {  
  HW::loop();

  if ( HW::btn_left.is_pressed() )       { HW::led1(ON); }
  else if ( HW::btn_left.is_released() ) { HW::led1(OFF); }

  if ( Buffers::input_l.is_full() ) {
    audio_block_t *block_l;
    audio_block_t *block_r;

    while ( !Buffers::input_l.is_empty() ) {
      block_l = Buffers::input_l.pop();
      block_r = Buffers::input_r.pop();

      switch (n.e()) {
        case 0:
          if ( !file.open("test3.bin", O_RDWR | O_CREAT | O_TRUNC) ) {
          /*if ( !file.open("test.txt", O_RDWR) ) {*/
            Serial.println("##### Error opening file test3.bin");
          }
          file.seek(EOF);
          n.jump_to(1);
          break;


        case 1:
          if ( digitalRead(BA_EXPAND_LED1_PIN) ) {
            is_recording = true;

            // buff size is number of bytes, since buff is int16_t its 2 bytes per var
            if ( !file.write(block_l->data, AUDIO_BLOCK_SAMPLES*sizeof(int16_t)) ) {
              Serial.println("##### Error writing to test.txt");
            }
          }
          else if ( is_recording && !digitalRead(BA_EXPAND_LED1_PIN) ) {
            is_recording = false;
            n.jump_to(2);
          }
          break;

        case 2:
          if ( n.not_init() ) {
            file.sync();
            file.rewind();
            file.read(buff, AUDIO_BLOCK_SAMPLES*sizeof(int16_t));
            for (int i=0; i<128; i++) {
              SP(i, ": ", buff[i]);
            }
            file.close();
            break;
          }
          break;
      };

      Buffers::output_l.insert( block_l );
      Buffers::output_r.insert( block_r );
    }
  }



// some stuff to do with SD cards //////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#ifdef IGNORE_ME
  if ( n.not_init() ) {
//    for (int i=0; i<128; i++) buff[i] = i; // write some data to buff

    if ( !file.open("test2.txt", O_RDWR) ) {
    /*if ( !file.open("test.txt", O_RDWR) ) {*/
      Serial.println("##### Error opening file test.txt");
    }
    file.seek(EOF);

    // buff size is number of bytes, since buff is int16_t its 2 bytes per var
    if ( !file.write(buff, (128*2)) ) {
      Serial.println("##### Error writing to test.txt");
    }
    file.sync();
    file.rewind();

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
