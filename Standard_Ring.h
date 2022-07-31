/*******************************************************************************
* This provides a Ring Buffer or Circular Buffer with interrupt protection.     
* Mainly used for audio buffer transfers.                                       
*******************************************************************************/

#ifdef INCLUDE_STANDARD_RING
#ifndef RING_H
#define RING_H

#define BLOCK_SIZE(n)   (AUDIO_BLOCK_SAMPLES*n)
#define ROTATE(x,sz)    if(x+1==sz) {x = 0;} else {x+=1;}
#define ROTATE_IF(x,sz) (x+1==sz ? 0 : x+1)

template <uint16_t S>
struct Ring {
  int16_t buff[S] = {0};
  /* use uint16_t for less than 512 blocks */
  uint16_t size      = S;
  uint16_t head      = 0;
  uint16_t tail      = 0;
  uint8_t  is_atomic = true;

  void insert(int16_t sample) {
    buff[head] = sample;
    ROTATE(head, size);
    if ( head == tail ) { ROTATE(tail, size); }
  }

  void insert_block(int16_t *block) {
    disable_irq();
    for (int i=0; i<AUDIO_BLOCK_SAMPLES; i++) {
      insert(block[i]);
    }
    enable_irq();
  }

  int16_t pop() {
    if ( head == tail ) { return buff[0]; } // CHECK EMPTY
    uint16_t result_tail = tail;
    if ( tail != head ) { ROTATE(tail, size); }
    return buff[result_tail];
  }

  void pop_block(int16_t *block) {
    disable_irq();
    for (int i=0; i<AUDIO_BLOCK_SAMPLES; i++) {
      block[i] = pop();
    }
    enable_irq();
  }

  bool is_empty() { return head == tail ? true : false; }
  bool is_full()  { return ROTATE_IF(head, size) == tail ? true : false; }
  uint16_t get_size() {
    if ( head >= tail ) return head - tail;
    return size - (tail - head);
  }

  void set_atomic(bool is_atomic) { this->is_atomic = (uint8_t)is_atomic; }

private:
  void disable_irq() { if ( is_atomic ) __disable_irq(); }
  void enable_irq()  { if ( is_atomic ) __enable_irq(); }
};

#ifdef STANDARD_RING_H_TEST

Ring<BLOCK_SIZE(12)> buff_test;

void setup() {
  Serial.begin(38400);
  SP("--> Start ALT TEST Standard_Ring.h");
}
void loop() {
}
#endif

#endif
#endif
