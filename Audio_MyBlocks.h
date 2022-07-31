#ifdef INCLUDE_AUDIO_MYBLOCKS
#ifndef MY_BLOCKS_H
#define MY_BLOCKS_H

class InputBlock : public AudioStream
{
public:
	InputBlock(void): AudioStream(2, inputQueueArray) {}

	virtual void update(void) {
    audio_block_t *block[2];
    //Buffers::BlockStereo block;
    for (int i=0; i<2; i++) {
      block[i] = receiveReadOnly(i);
      if (!block[i]) return;
      Buffers::output[i].insert(block[i]);  // only as a test, remove
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
      block[i] = Buffers::output[i].pop();
      if (!block[i]) return;
      transmit(block[i], i);
      release(block[i]);
    }
  }
	
private:

	audio_block_t *inputQueueArray[2]; // MUST be 2 for stereo, 1 for mono

};

#endif
#endif
