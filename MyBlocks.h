#ifndef MY_BLOCKS_H
#define MY_BLOCKS_H

#include <Arduino.h>
#include "AudioStream.h"
#include "Buffers.h"

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

    Buffers::input_l.insert(block_l);
    Buffers::input_r.insert(block_r);

    release(block_l);
    release(block_r);

    return;
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

    if ( !Buffers::input_l.is_empty() ) {
      block_l = Buffers::input_l.pop();
      block_r = Buffers::input_r.pop();

      transmit(block_l, 0);
      transmit(block_r, 1);

      release(block_l);
      release(block_r);
    }

  }
	
private:

	audio_block_t *inputQueueArray[2]; // MUST be 2 for stereo, 1 for mono

};

#endif
