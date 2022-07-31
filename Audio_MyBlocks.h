#ifdef INCLUDE_AUDIO_MYBLOCKS
#ifndef MY_BLOCKS_H
#define MY_BLOCKS_H

Ring<BLOCK_SIZE(12)> buffer_l;
Ring<BLOCK_SIZE(12)> buffer_r;

class InputBlock : public AudioStream
{
public:
	InputBlock(void): AudioStream(2, inputQueueArray) {}

	virtual void update(void) {
    audio_block_t *block_l;
    audio_block_t *block_r;

    block_l = receiveReadOnly(0);
    if (!block_l) return;
    block_r = receiveReadOnly(1);
    if (!block_r) { release(block_l); return; }

    buffer_l.insert_block(block_l->data);
    buffer_r.insert_block(block_r->data);

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
    audio_block_t *block_l = allocate();
    if (!block_l) return;
    audio_block_t *block_r = allocate();
    if (!block_r) { release(block_l); return; }

    if ( !buffer_l.is_empty() ) {
      buffer_l.pop_block(block_l->data);
      buffer_r.pop_block(block_r->data);
    }

    transmit(block_l, 0);
    transmit(block_r, 1);

    release(block_l);
    release(block_r);
  }
	
private:

	audio_block_t *inputQueueArray[2]; // MUST be 2 for stereo, 1 for mono

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
