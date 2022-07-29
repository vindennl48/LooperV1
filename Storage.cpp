#include "Storage.h"

SdExFat       Storage::sd;
ExFile        Storage::file;
audio_block_t *Storage::block;
Nav           Storage::n;
uint8_t       Storage::rec = 0;
