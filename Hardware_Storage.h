#ifdef INCLUDE_HARDWARE_STORAGE
#ifndef STORAGE_H
#define STORAGE_H

// SDCARD_SS_PIN is defined for the built-in SD on some boards.
#ifndef SDCARD_SS_PIN
const uint8_t SD_CS_PIN = SS;
#else  // SDCARD_SS_PIN
// Assume built-in SD is used.
const uint8_t SD_CS_PIN = SDCARD_SS_PIN;
#endif  // SDCARD_SS_PIN

#define SPI_CLOCK SD_SCK_MHZ(50)

// Try to select the best SD card configuration.
#if HAS_SDIO_CLASS
#define SD_CONFIG SdioConfig(FIFO_SDIO)
#elif ENABLE_DEDICATED_SPI
#define SD_CONFIG SdSpiConfig(SDCARD_SS_PIN, DEDICATED_SPI, SPI_CLOCK)
#else  // HAS_SDIO_CLASS
#define SD_CONFIG SdSpiConfig(SDCARD_SS_PIN, SHARED_SPI, SPI_CLOCK)
#endif  // HAS_SDIO_CLASS

// USE:
// ExFile file;

//#define FILE_NAME(x) (x==0?"temp.bin":"layer" #x ".bin")
#define S_CLOSED     0;
#define S_OPEN       1;

struct FileHeader {
  uint32_t file_size = 0;
  uint32_t playhead  = 0;
  uint8_t  beats     = 0;
};

struct Storage {
  static SdExFat    sd;
  static ExFile     file;
  static uint8_t    status;
  static uint8_t    file_num;
  static FileHeader file_headers[NUM_LAYERS+1];
  static char       file_names[][7];

  static void setup() {
    status   = S_CLOSED;
    file_num = 0;

    /* Setup SD card */
    sd.begin(SD_CONFIG);

    /* Check to make sure all files are created */
    for (int n=0; n<=NUM_LAYERS; n++) {
      if ( !sd.exists(file_names[n]) ) {
        file.open(file_names[n], O_RDWR | O_CREAT | O_TRUNC);
          //file.write(&file_headers[file_num], sizeof(FileHeader));
        file.close();
      }
    }
  }

  static void append_open_new(uint8_t n) {
    file_num = n;
    file_headers[file_num] = FileHeader();
    file.open(file_names[file_num], O_RDWR | O_TRUNC); 
    file.write(&file_headers[file_num], sizeof(FileHeader));
    file.seek(EOF);
  }

  static void append_open(uint8_t n) {
    file_num = n;
    file.open(file_names[file_num], O_RDWR); 
    file.read(&file_headers[file_num], sizeof(FileHeader));
    file.seek(EOF);
  }

  static void append_add_block(int16_t *block) {
    file.write(block, AUDIO_BLOCK_SAMPLES*sizeof(int16_t));
    file_headers[file_num].file_size += AUDIO_BLOCK_SAMPLES*sizeof(int16_t);
  }

  static void append_close() {
    file.seek(0);
    file.write(&file_headers[file_num], sizeof(FileHeader));
    file.close();
  }

  static void get_open(uint8_t n) {
    file_num = n;
    file.open(file_names[file_num], O_RDWR); 
    file.read(&file_headers[file_num], sizeof(FileHeader));
    file.seek(sizeof(FileHeader)+file_headers[file_num].playhead);
  }
  static void get_block(int16_t *block) {
    file.read(block, AUDIO_BLOCK_SAMPLES*sizeof(int16_t));
    file_headers[file_num].playhead += AUDIO_BLOCK_SAMPLES*sizeof(int16_t);
  }
  static void get_close() {
    file.seek(0);
    file.write(&file_headers[file_num], sizeof(FileHeader));
    file.close();
  }
};

SdExFat    Storage::sd;
ExFile     Storage::file;
uint8_t    Storage::status;
uint8_t    Storage::file_num;
FileHeader Storage::file_headers[NUM_LAYERS+1];
char       Storage::file_names[][7] = {
  "00.bin",
  "01.bin",
  "02.bin"
};


#ifdef HARDWARE_STORAGE_H_TEST
#include "sdios.h"
ArduinoOutStream cout(Serial);

Nav n;

void setup() {
  //Serial.begin(38400);  // USB SERIAL
  Serial.begin(9600);  // USB SERIAL
  while (!Serial) { SysCall::yield(); } // Wait for USB Serial
  SP("--> Start ALT TEST Hardware_Storage.h");

  Storage::setup();
}

void loop() {
  switch (n.e()) {
    case 0:
      Storage::append_open(0);
      Storage::file_headers[0].beats = 5;
      Storage::append_close();
      Storage::file_headers[0].beats = 25;

      n.jump_to(1);
      break;

    case 1:
      if ( n.not_init() ) {
        SP("List of files on the SD.");
        Storage::sd.ls(LS_R);
        SP("========================");
        Storage::append_open(0);
        SP("beats: ", Storage::file_headers[0].beats);
        Storage::append_close();
        break;
      }
      break;
  };
}

#endif // HARDWARE_STORAGE_H_TEST


#endif
#endif







/* Exerpt from main for testing */
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


#ifdef IGNOREME

#define file_layer_temp(x) "temp#x.bin"

#define E_WAIT   0
#define E_RECORD 1

struct Storage {
  static SdExFat       sd;
  static ExFile        file;
  static audio_block_t *block;
  static Nav           n;
  static uint8_t       rec;  // 0 - 9  for 10 layers

  static uint8_t setup() { return sd.begin(SD_CONFIG); }

  static void loop() {
    switch (n.e()) {
      case E_WAIT:
        if ( n.not_init() ) {
          if ( file ) file.close(); // TODO: this needs to be checked for accuracy
          break;
        }
        break;

      case E_RECORD:
        if ( n.not_init() ) {
          // open as readwrite, create the file if it doesn't exist, truncate
          // the file to 0 since every record is a rewrite.
          file.open(FILE_LAYER_TEMP(rec), O_RDWR | O_CREAT | O_TRUNC);
          break;
        }
        if ( Buffers.input_not_empty() ) {
          file.write(Buffers.input(), AUDIO_BLOCK_SAMPLES*2);
        }
        break;
    };
  }

  static void record() {
    n.jump_to(E_RECORD);
  }

  static uint8_t is_recording() { return n.e(); }

  static void stop() {
    n.jump_to(E_WAIT);
    rec++;
  }
};

SdExFat       Storage::sd;
ExFile        Storage::file;
audio_block_t *Storage::block;
Nav           Storage::n;
uint8_t       Storage::rec = 0;
#endif // IGNOREME
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
