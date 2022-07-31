#ifdef INCLUDE_AUDIO_BUFFERS
#ifndef BUFFERS_H
#define BUFFERS_H

#define AUDIO_BUFF_SIZE 24
typedef Ring<audio_block_t*> BufferMono;
typedef BufferMono           BufferStereo[2];

struct Buffers {
  static BufferStereo sd_card;
  static BufferStereo input;
  static BufferStereo layer[NUM_LAYERS];
  static BufferStereo output;
};

#ifdef BUFFERS_H_TEST
void setup() {}
void loop() {}
#endif

#endif
#endif
