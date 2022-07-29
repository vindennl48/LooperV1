#ifndef MY_FIRST_AUDIO_BLOCK_H
#define MY_FIRST_AUDIO_BLOCK_H

#include <Arduino.h>
#include "AudioStream.h"
#include "Hardware.h"
#include "Storage.h"

ExFile file;
int16_t buff[128] = {0};

/*
 *  Need a dual buffer system for the input.  One buffer gets filled by the
 *  input.  Once the buffer fills up completely, the input switches to the
 *  second buffer.  Then the SD card gets written to by the first buffer.  Once
 *  the second buffer is full, the input switches back to the first buffer.
 *  Then the SD card gets written to by the second buffer.  This keeps
 *  alternating.
 * */



class MyFirstAudioBlock : public AudioStream
{
public:
	MyFirstAudioBlock(void): AudioStream(1, inputQueueArray) {}

	virtual void update(void) {
    audio_block_t *block; block = receiveReadOnly();
    if (!block) return;  // safety check

    if ( digitalRead(BA_EXPAND_LED2_PIN) ) {
      if ( !Storage::is_recording() ) Storage::record();
    } else {
      if ( Storage::is_recording() ) Storage::stop();
    }

//    if ( digitalRead(BA_EXPAND_LED2_PIN) ) {
//      file.open("test2.txt", O_RDWR | O_CREAT); file.seek(EOF);
//      file.write(block->data, (AUDIO_BLOCK_SAMPLES*2));
//      file.close();
//    }

    transmit(block); // to send audio to output

    release(block); return;
  }
	
private:

	audio_block_t *inputQueueArray[1];

};

#endif
