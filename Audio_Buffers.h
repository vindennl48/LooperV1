#ifdef INCLUDE_AUDIO_BUFFERS
#ifndef BUFFERS_H
#define BUFFERS_H

#define AUDIO_BUFF_SIZE 12
#define LOOP_SAMPLES for(int qq=0; qq<AUDIO_BLOCK_SAMPLES; qq++)

typedef Ring<BLOCK_SIZE(AUDIO_BUFF_SIZE)> StdBuffer;

namespace Buffers {
  StdBuffer sd_card[2];
  StdBuffer input[2];
  StdBuffer layer[NUM_LAYERS][2];
  StdBuffer output[2];
};



#ifdef BUFFERS_H_TEST
void setup() {
  HW::setup();
  HW::setup_serial(SERIAL_USB);

  SP("--> Start ALT TEST Audio_Buffers.h");
}
void loop() {
  HW::loop();

  if ( !Buffers::input[0].is_empty() ) {
    for (int i=0; i<2; i++) {
      //Buffers::input[i].transfer_block(&Buffers::output[i]);
      LOOP_SAMPLES {
        Buffers::output[i].sum_insert_start();
        Buffers::output[i].sum_insert(Buffers::input[i].pop());
        for (int n=0; n<NUM_LAYERS; n++)
          Buffers::output[i].sum_insert(Buffers::layer[n][i].pop());
        Buffers::output[i].sum_insert_end();
      }
    }
  }
}
#endif





#ifdef IGNOREME
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

#endif//ifdef IGNOREME

#endif
#endif
