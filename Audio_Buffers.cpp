#ifdef INCLUDE_AUDIO_BUFFERS

#include "Includes.h"

BufferStereo Buffers::sd_card = {
  BufferMono(AUDIO_BUFF_SIZE),
  BufferMono(AUDIO_BUFF_SIZE)
}

BufferStereo Buffers::input = {
  BufferMono(AUDIO_BUFF_SIZE),
  BufferMono(AUDIO_BUFF_SIZE)
}

BufferStereo Buffers::layer[NUM_LAYERS];
for (int i=0; i<NUM_LAYERS; i++) {
  for (int j=0; j<2; j++) {
    Buffers::layer[i][j] = BufferMono(AUDIO_BUFF_SIZE);
  }
}

BufferStereo Buffers::output = {
  BufferMono(AUDIO_BUFF_SIZE),
  BufferMono(AUDIO_BUFF_SIZE)
}

#endif
