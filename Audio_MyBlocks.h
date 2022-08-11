#ifdef INCLUDE_AUDIO_MYBLOCKS
#ifndef MY_BLOCKS_H
#define MY_BLOCKS_H

class InputBlock : public AudioStream
{
public:
	InputBlock(void): AudioStream(2, inputQueueArray) {}

	virtual void update(void) {
    audio_block_t *block[2];

    for (int i=0; i<2; i++) {
      /* need these lines to init audio_block_t */
      block[i] = receiveReadOnly(i);
      if (!block[i]) {
        if (i==1) { release(block[0]); return; }
        else      { return; }
      }
      /* -------------------------------------- */

      if ( i==0 && digitalRead(BA_EXPAND_LED1_PIN) ) {
        audio_temp.data.insert(block[i]);
      }

      //transmit(block[i], i);
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
      /* need these lines to init audio_block_t */
      block[i] = allocate();
      if (!block[i]) {
        if (i==1) { release(block[0]); return; }
        else      { return; }
      }
      for(int j=0; j<AUDIO_BLOCK_SAMPLES; j++) {
        block[i]->data[j] = 0;
      }
      /* -------------------------------------- */

      if ( i==0 && digitalRead(BA_EXPAND_LED2_PIN) ) {
        audio_temp.data.pop(block[i]);
//        SP("--> data.head[ib]:   ", audio_temp.data.head[audio_temp.data.ib]);
//        SP("    data.head[ib^1]: ", audio_temp.data.head[audio_temp.data.ib^1]);
//        SP("    block[i]:        ", block[i]->data[0]);
//        SP("");

//        for(int j=0; j<AUDIO_BLOCK_SAMPLES; j++) {
//          SP("data: ", block[i]->data[j]);
//        }
      }

      //buf[i].pop(block[i]);
      transmit(block[i], i);
      release(block[i]);
    }
  }
	
private:

	audio_block_t *inputQueueArray[1]; // MUST be 2 for stereo, 1 for mono

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
}
#endif //AUDIO_MYBLOCKS_H_TEST

#endif //MY_BLOCKS_H
#endif //INCLUDE_AUDIO_MYBLOCKS
