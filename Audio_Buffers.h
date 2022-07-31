#ifdef INCLUDE_AUDIO_BUFFERS
#ifndef BUFFERS_H
#define BUFFERS_H

#define AUDIO_BUFF_SIZE 24

// #define LAYER(n) layer##n
// namespace Buffers {
// 
typedef Ring<audio_block_t*,AUDIO_BUFF_SIZE> BufferMono;
typedef BufferMono     BufferStereo[2];
typedef audio_block_t* BlockStereo[2];
// 
// BufferStereo sd_card = {BufferMono(), BufferMono()};
// BufferStereo input   = {BufferMono(), BufferMono()};
// BufferStereo output  = {BufferMono(), BufferMono()};
// 
// };

struct Buffers {
  static BufferStereo sd_card;
  static BufferStereo input;
  static BufferStereo layer[NUM_LAYERS];
  static BufferStereo output;
};

BufferStereo Buffers::sd_card = { BufferMono(), BufferMono() };
BufferStereo Buffers::input   = { BufferMono(), BufferMono() };
BufferStereo Buffers::output  = { BufferMono(), BufferMono() };

#ifdef BUFFERS_H_TEST
void setup() {}
void loop() {}
#endif

#endif
#endif
