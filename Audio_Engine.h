#ifdef INCLUDE_AUDIO_ENGINE
#ifndef AUDIO_ENGINE_H
#define AUDIO_ENGINE_H

namespace AudioEngine {

AudioInputI2S  i2sIn;
AudioOutputI2S i2sOut;
InputBlock     input_b;
OutputBlock    output_b;

//AudioConnection patch1(i2sIn,    0, i2sOut, 0);
//AudioConnection patch2(i2sIn,    1, i2sOut, 1);

AudioConnection patch1(i2sIn,    0, input_b, 0);
AudioConnection patch2(i2sIn,    1, input_b, 1);

AudioConnection patch3(output_b, 0, i2sOut,  0);
AudioConnection patch4(output_b, 1, i2sOut,  1);

void setup() {
  SP("--> Audio Engine Start");
}
void loop() {
#ifdef IGNOREME
  /* Sum all data from layers and input buffer and send to output buffer */
  while ( !Buffers::input[0].is_empty() ) {
  //if ( !Buffers::input[0].is_empty() ) {
    for (int i=0; i<2; i++) { // run both left and right channel
      /* Make sure to copy all samples for a full block */
      for(int qq=0; qq<AUDIO_BLOCK_SAMPLES; qq++) {
        Buffers::output[i].sum_insert_start(); // Open sum function
        Buffers::output[i].sum_insert(Buffers::input[i].pop()); // add input buf
        for (int n=0; n<NUM_LAYERS; n++) // add each layer buffer
          Buffers::output[i].sum_insert(Buffers::layer[n][i].pop());
        Buffers::output[i].sum_insert_end(); // sum all layers together
      }
    }
  }
#endif //IGNOREME
}

};


#ifdef AUDIO_ENGINE_H_TEST
void setup() {
  HW::setup();
  HW::setup_serial(SERIAL_USB);
  AudioEngine::setup();

  SP("--> Start ALT TEST Audio_Engine.h");
}
void loop() {
  HW::loop();
  AudioEngine::loop();
}
#endif

#endif
#endif
