#ifdef INCLUDE_AUDIO_MYBLOCKS
#ifndef MY_BLOCKS_H
#define MY_BLOCKS_H

BiBuffer<BUFFER_SAMPLES> buf[2];

class InputBlock : public AudioStream
{
public:
	InputBlock(void): AudioStream(2, inputQueueArray) {}

	virtual void update(void) {
    audio_block_t *block[2];

    for (int i=0; i<2; i++) {
      block[i] = receiveReadOnly(i);
      if (!block[i]) {
        if (i==1) { release(block[0]); return; }
        else      { return; }
      }

      buf[i].insert(block[i]);

      //Buffers::sd_card[i].insert_block(block[i]->data);
      //Buffers::input[i].insert_block(block[i]->data);
        transmit(block[i], i);
      release(block[i]);
    }
  }
	
private:

	audio_block_t *inputQueueArray[2]; // MUST be 2 for stereo, 1 for mono

};


class OutputBlock : public AudioStream
{
public:
	OutputBlock(void): AudioStream(2, inputQueueArray) {}

	virtual void update(void) {
    audio_block_t *block[2];

    for (int i=0; i<2; i++) {
      block[i] = allocate();
      if (!block[i]) {
        if (i==1) { release(block[0]); return; }
        else      { return; }
      }

      //buf[i].pop(block[i]);
      transmit(block[i], i);
      release(block[i]);
    }
  }
	
private:

	audio_block_t *inputQueueArray[2]; // MUST be 2 for stereo, 1 for mono

};

#ifdef AUDIO_MYBLOCKS_H_TEST
Nav n;

void setup() {
  HW::setup();
  HW::setup_serial(SERIAL_USB);
  Storage::setup();

  SP("--> Start ALT TEST Audio_MyBlocks.h");
}

void loop() {
  HW::loop();

  if ( HW::btn_left.is_pressed() )  HW::led1(TOGGLE);
  if ( HW::btn_right.is_pressed() ) HW::led2(TOGGLE);

  switch (n.e()) {
    case 0:
      if ( digitalRead(BA_EXPAND_LED1_PIN) ) {
        Storage::append_open(0);
        n.jump_to(1);
      }
      break;

    case 1:
      if ( !digitalRead(BA_EXPAND_LED1_PIN) ) {
        Storage::append_close();
        n.jump_to(2);
        break;
      }
      if ( !buf[0].is_empty() ) {
        Storage::append_add(buf[0].get_buffer(), buf[0].max_bytes());
        buf[0].clear_buffer();
      }
      break;

    case 2:
      if ( n.not_init() ) {
        int16_t data[1024] = {0};
        Storage::get_open(0);
        Storage::get_data(data, 1024*sizeof(int16_t));
        Storage::get_close();

        for(int i=0; i<1024; i++) {
          SP(i, ". data: ", data[i]);
        }
      }
      break;
  };
}
#endif

#endif
#endif
