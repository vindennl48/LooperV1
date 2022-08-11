#ifdef INCLUDE_HARDWARE_STORAGE
#ifndef HARDWARE_STORAGE_H
#define HARDWARE_STORAGE_H

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

struct Storage {
  static SdExFat sd;

  static void setup() {
    /* Setup SD card */
    sd.begin(SD_CONFIG);
  }

/*
  static void append_open_new(uint8_t n) {
    file_num = n;
    audio_file[file_num] = AudioFile();
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

  static void append_add(int16_t *block, uint16_t size) {
    file.write(block, size);
    file_headers[file_num].file_size += size;
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
  static void get_data(int16_t *block, uint16_t size) {
    if ( !file.available() ) {
      file.seek(sizeof(FileHeader));
      file_headers[file_num].playhead = 0;
    }
    file.read(block, size);
    file_headers[file_num].playhead += size;
  }
  static void get_close() {
    file.seek(0);
    file.write(&file_headers[file_num], sizeof(FileHeader));
    file.close();
  }
*/
};

SdExFat   Storage::sd;
//ExFile    Storage::file;
//uint8_t   Storage::status;
//uint8_t   Storage::file_num;
//AudioFile Storage::file_headers[NUM_LAYERS+1];
//char       Storage::file_names[][7] = {
  //"00.bin",
  //"01.bin",
  //"02.bin"
//};



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
