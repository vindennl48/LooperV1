#ifdef INCLUDE_AUDIO_MYBLOCKS
#ifndef MY_BLOCKS_H
#define MY_BLOCKS_H

#define BUF_SIZE 64
BiBuffer<BUF_SIZE> buf[2];

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

      buf[i].pop(block[i]);
      transmit(block[i], i);
      release(block[i]);
    }
  }
	
private:

	audio_block_t *inputQueueArray[2]; // MUST be 2 for stereo, 1 for mono

};

#ifdef AUDIO_MYBLOCKS_H_TEST
void setup() {
  HW::setup();
  HW::setup_serial(SERIAL_USB);
  Storage::setup();

  SP("--> Start ALT TEST Audio_MyBlocks.h");
}

void loop() {
  HW::loop();
}
#endif

#endif
#endif
