#ifdef ALT_PROGRAM
#ifdef MAIN_TEMP_H

Nav n;
int16_t buf1[AUDIO_BLOCK_SAMPLES] = {0};
int16_t buf2[AUDIO_BLOCK_SAMPLES] = {0};

void setup() {
  HW::setup();
  HW::setup_serial(SERIAL_USB);
  Storage::setup();
  AudioEngine::setup();

  SP("--> Start ALT main_temp.h");
}
void loop() {
  HW::loop();
  AudioEngine::loop();

  if ( digitalRead(BA_EXPAND_LED1_PIN) ) {
    while ( !Buffers::sd_card[0].is_empty() ) {
      Buffers::sd_card[0].pop_block(buf1);
      Storage::append_open(0);
      Storage::append_add_block(buf1);
      Storage::append_close();
    }
  }
  else if ( digitalRead(BA_EXPAND_LED2_PIN) ) {
    Storage::get_open(0);
    Storage::get_block(buf2);
    Storage::get_close();
    Buffers::layer[0][0].insert_block(buf2);
  }

  if ( HW::led2_state() == OFF && HW::btn_left.is_pressed() ) {
    HW::led1(TOGGLE);
  }
  if ( HW::led1_state() == OFF && HW::btn_right.is_pressed() ) {
    HW::led2(TOGGLE);
  }
}

#endif
#endif
