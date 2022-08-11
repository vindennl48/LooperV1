#ifdef INCLUDE_STANDARD_BIBUFFER
#ifndef STANDARD_BIBUFFER_H
#define STANDARD_BIBUFFER_H

#define BUFFER_SAMPLES (AUDIO_BLOCK_SAMPLES*4)

template <uint16_t NUM_BUF_SAMPLES>
struct BiBuffer {
  int16_t  data[2][NUM_BUF_SAMPLES];
  uint16_t head[2] = {0};
  uint8_t  ib      = 0; // sets which buffer is active 0:first, 1:sescond

  void reset() {
    for(int i=0; i<2; i++) head[i] = 0;
    ib = 0;
  }

  /* fill the rest of buffer with 0 */
  void flush() {
    for(int i=head[ib]; i<NUM_BUF_SAMPLES; i++) { data[ib][i] = 0; }
    head[ib] = NUM_BUF_SAMPLES;
    flip();
  }

  /* send in one sample at a time */
  void insert(int16_t *sample) { insert(sample, 1); }
  /* send in audio_block_t object directly */
  void insert(audio_block_t *block_t) {
    insert( (int16_t*)block_t->data, AUDIO_BLOCK_SAMPLES ); 
  }
  /* main function */
  void insert(int16_t *block, uint16_t num_samples) {
    /* If current buffer is full */
    if ( head[ib] == NUM_BUF_SAMPLES ) {
      if (!flip()) {
        SP("### insert: LOST SAMPLE");
        return;
      }
    }

    if ( num_samples > (NUM_BUF_SAMPLES-head[ib]) ) {
      /* fill up what we can of current buffer */
      memcpy(
        &data[ib][head[ib]],
        block,
        sizeof(int16_t)*(NUM_BUF_SAMPLES-head[ib])
      );
      head[ib] += (NUM_BUF_SAMPLES-head[ib]);

      if ( head[ib^1] == 0 ) {
        /* switch to filling second buffer if it's empty */
        ib ^= 1;

        /* put the rest of the block in the next buffer */
        memcpy(
          &data[ib][head[ib]],
          block,
          sizeof(int16_t)*(num_samples-(NUM_BUF_SAMPLES-head[ib]))
        );
        head[ib] += (num_samples-(NUM_BUF_SAMPLES-head[ib]));

      }
      return;
    }

    memcpy(
      &data[ib][head[ib]],
      block,
      sizeof(int16_t)*num_samples
    );

    head[ib] += num_samples;
  }

  /* grab one sample at a time */
  void pop(int16_t *sample) { pop(sample, 1); }
  /* pop direct into audio_block_t */
  void pop(audio_block_t *block_t) {
    pop( (int16_t*)block_t->data, AUDIO_BLOCK_SAMPLES ); 
  }
  /* main function */
  void pop(int16_t *block, uint16_t num_samples) {
    /* If buffer is empty, wait. */
    if ( head[ib^1] == 0 ) {
      SP("pop: DROPPED SAMPLE!");
      return; 
    }

    /* if we ask for more ssamples than we have */
    if ( num_samples > head[ib^1] ) {
      /* only grab what is left in the buffer */
      memcpy(
        block,
        &data[ib^1][NUM_BUF_SAMPLES-head[ib^1]],
        sizeof(int16_t)*head[ib^1]
      );

      /* set the rest to 0 */
      for (int i=head[ib^1]; i<num_samples; i++) { block[i] = 0; }

      /* clean out the buffer */
      head[ib^1] = 0;

      return;
    }

    /* Lets grab the data from the buffer */
    memcpy(
      block,
      &data[ib^1][NUM_BUF_SAMPLES-head[ib^1]],
      sizeof(int16_t)*num_samples
    );

    head[ib^1] -= num_samples;
  }

  bool insert_is_empty() {
    if ( head[ib] == 0 ) { return true; }
    flip();
    return false;
  }

  bool insert_is_full() {
    if ( head[ib] == NUM_BUF_SAMPLES ) { return true; }
    return false;
  }

  bool pop_is_empty() {
    if ( head[ib^1] == 0 ) { return true; }
    return false;
  }

  bool pop_is_full() {
    if ( head[ib^1] == NUM_BUF_SAMPLES ) { return true; }
    return false;
  }

  uint16_t max_samples() { return NUM_BUF_SAMPLES; }
  uint16_t max_bytes()   { return NUM_BUF_SAMPLES*sizeof(int16_t); }

  /* CHECK IF EMPTY FIRST! use only for sd card or big memory dumps */
  int16_t* get_insert_buffer() { return data[ib]; }
  void set_insert_full() {
    head[ib] = NUM_BUF_SAMPLES; 
    flip();
  }
  int16_t* get_pop_buffer() { return data[ib^1]; }
  void clear_pop_buffer() { head[ib^1] = 0; }

private:
  bool flip() {
    /* if second buffer is empty */
    if ( head[ib^1] == 0 ) {
      /* switch to filling second buffer */
      ib ^= 1;
    }
    else {
      /* wait for second buffer to empty */
      /* lost sample block */
//      SP("--> flip: DROPPED SAMPLE!");
//      SP("    flip: head pos: ", head[ib^1]);
      return false;
    }
    return true;
  }
};

#ifdef STANDARD_BIBUFFER_H_TEST
void setup() {
  SP("--> Start ALT TEST Standard_BiBuffer.h");
}
void loop() {
}
#endif

#endif
#endif
