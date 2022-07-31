#ifdef INCLUDE_AUDIO_ENGINE
#ifndef AUDIO_ENGINE_H
#define AUDIO_ENGINE_H

namespace AudioEngine {

AudioInputI2S  i2sIn;
AudioOutputI2S i2sOut;
InputBlock     input_b;
OutputBlock    output_b;

AudioConnection patch1(i2sIn,    0, input_b, 0);
AudioConnection patch2(i2sIn,    1, input_b, 1);

AudioConnection patch3(output_b, 0, i2sOut,  0);
AudioConnection patch4(output_b, 1, i2sOut,  1);

void setup() {
  SP("--> Audio Engine Start");
}
void loop() {}

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
