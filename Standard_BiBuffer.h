#ifdef INCLUDE_STANDARD_BI_BUFFER
#ifndef STANDARD_BI_BUFFER_H
#define STANDARD_BI_BUFFER_H

struct Block {
  volatile int16_t data[AUDIO_BLOCK_SAMPLES] = {0};
};

template <uint16_t NUM_BLOCKS>
struct BiBuffer {
  Block    data[2][NUM_BLOCKS];
  uint16_t head[2]       = {0};
  uint8_t  insert_buffer = 0;
  uint8_t  is_popping    = false;

  void insert(audio_block_t *block_t) { insert( (Block*)block_t->data ); }
  void insert(int16_t *block) { insert( (Block*)block ); }
  void insert(Block *block) {
    /* If current buffer is full */
    if ( head[insert_buffer] == NUM_BLOCKS ) {
      /* if second buffer is empty */
      if ( head[insert_buffer^1] == 0 ) {
        /* switch to filling second buffer */
        insert_buffer ^= 1;
      }
      else {
        /* wait for second buffer to empty */
        /* lost sample block */
        return;
      }
    }

    memcpy(
      &data[insert_buffer][head[insert_buffer]],
      block,
      sizeof(Block)
    );

    head[insert_buffer] += 1;
  }

  void pop(audio_block_t *block_t) { pop( (Block*)block_t->data ); }
  void pop(int16_t *block) { pop( (Block*)block ); }
  void pop(Block *block) {
    /* If buffer is empty, wait. */
    if ( head[insert_buffer^1] == 0 ) { return; }

    /* Lets grab the data from the buffer */
    memcpy(
      block,
      &data[insert_buffer^1][NUM_BLOCKS-head[insert_buffer^1]],
      sizeof(Block)
    );

    head[insert_buffer^1] -= 1;
  }

  bool is_empty() {
    if ( head[insert_buffer^1] == 0 ) { return true; }
    return false;
  }
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
