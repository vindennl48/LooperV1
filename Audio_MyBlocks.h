#ifdef INCLUDE_AUDIO_MYBLOCKS
#ifndef MY_BLOCKS_H
#define MY_BLOCKS_H

Ring buffer_l;
Ring buffer_r;

class InputBlock : public AudioStream
{
public:
	InputBlock(void): AudioStream(2, inputQueueArray) {}

	virtual void update(void) {
    audio_block_t *block_l;
    audio_block_t *block_r;

    block_l = receiveReadOnly(0);
    block_r = receiveReadOnly(1);

    if (!block_l || !block_r) return;

    buffer_l.insert(block_l);
    buffer_r.insert(block_r);

    //transmit(block_l, 0);
    //transmit(block_r, 1);

    release(block_l);
    release(block_r);
  }
	
private:

	audio_block_t *inputQueueArray[2]; // MUST be 2 for stereo, 1 for mono

};


class OutputBlock : public AudioStream
{
public:
	OutputBlock(void): AudioStream(2, inputQueueArray) {}

	virtual void update(void) {
    audio_block_t *block_l;
    audio_block_t *block_r;

    if ( !buffer_l.is_empty() ) {
      block_l = buffer_l.pop();
      block_r = buffer_r.pop();
    }
//
//    if ( !block_l || !block_r ) {
//      transmit(block_l, 0);
//      transmit(block_r, 1);
//
//      release(block_l);
//      release(block_r);
//    }
  }
	
private:

	audio_block_t *inputQueueArray[1]; // MUST be 2 for stereo, 1 for mono

};

#ifdef AUDIO_MYBLOCKS_H_TEST
void setup() {
  HW::setup();
  HW::setup_serial(SERIAL_USB);

  SP("--> Start ALT TEST Audio_MyBlocks.h");
}
void loop() {
  HW::loop();
}
#endif

#endif
#endif
