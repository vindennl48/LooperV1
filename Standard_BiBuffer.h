#ifdef INCLUDE_STANDARD_BI_BUFFER
#ifndef STANDARD_BI_BUFFER_H
#define STANDARD_BI_BUFFER_H

#define BUFFER_SAMPLES (AUDIO_BLOCK_SAMPLES*4)

template <uint16_t NUM_SAMPLES>
struct BiBuffer {
  int16_t  data[2][NUM_SAMPLES];
  uint16_t head[2] = {0};
  uint8_t  ib      = 0;

  void insert(audio_block_t *block_t) { insert( (int16_t*)block_t->data ); }
  void insert(int16_t *block) {
    /* If current buffer is full */
    if ( head[ib] == NUM_SAMPLES ) {
      /* if second buffer is empty */
      if ( head[ib^1] == 0 ) {
        /* switch to filling second buffer */
        ib ^= 1;
      }
      else {
        /* wait for second buffer to empty */
        /* lost sample block */
        return;
      }
    }

    memcpy(
      &data[ib][head[ib]],
      block,
      sizeof(int16_t)*AUDIO_BLOCK_SAMPLES
    );

    head[ib] += AUDIO_BLOCK_SAMPLES;
  }

  void pop(audio_block_t *block_t) { pop( (int16_t*)block_t->data ); }
  void pop(int16_t *block) {
    /* If buffer is empty, wait. */
    if ( head[ib^1] == 0 ) { return; }

    /* Lets grab the data from the buffer */
    memcpy(
      block,
      &data[ib^1][NUM_SAMPLES-head[ib^1]],
      sizeof(int16_t)*AUDIO_BLOCK_SAMPLES
    );

    head[ib^1] -= AUDIO_BLOCK_SAMPLES;
  }

  bool is_empty() {
    if ( head[ib^1] == 0 ) { return true; }
    return false;
  }

  uint16_t max_samples() { return NUM_SAMPLES; }
  uint16_t max_bytes()   { return NUM_SAMPLES*sizeof(int16_t); }

  /* CHECK IF EMPTY FIRST! use only for sd card or big memory dumps */
  int16_t* get_buffer() { return data[ib^1]; }
  void clear_buffer() { head[ib^1] = 0; }
};

#ifdef STANDARD_BI_BUFFER_H_TEST
void setup() {
  SP("--> Start ALT TEST Standard_BiBuffer.h");
}
void loop() {
}
#endif

#endif
#endif
