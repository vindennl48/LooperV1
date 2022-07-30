#ifdef INCLUDE_AUDIO_BUFFERS
#ifndef BUFFERS_H
#define BUFFERS_H

#define AUDIO_BUFF_SIZE 24

struct Buffers {
  static Ring<audio_block_t*> input_l;
  static Ring<audio_block_t*> input_r;

  static Ring<audio_block_t*> output_l;
  static Ring<audio_block_t*> output_r;
};

#endif
#endif
