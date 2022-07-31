/*******************************************************************************
* This provides a Ring Buffer or Circular Buffer with interrupt protection.     
* Mainly used for audio buffer transfers.                                       
*******************************************************************************/

#ifdef INCLUDE_STANDARD_RING
#ifndef RING_H
#define RING_H

#define BUFF_SIZE       (AUDIO_BLOCK_SAMPLES*4)
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

// struct Ring {
//   audio_block_t* buff[BUFF_SIZE];
//   uint16_t       size      = BUFF_SIZE;
//   uint16_t       head      = 0;
//   uint16_t       tail      = 0;
//   uint8_t        is_atomic = true;
// 
//   void insert(audio_block_t *item) {
//     disable_irq();
//     buff[head] = item;
//     ROTATE(head, size);
//     if ( head == tail ) { ROTATE(tail, size); }
//     enable_irq();
//   }
// 
//   audio_block_t* pop() {
//     disable_irq();
//     if ( head == tail ) { return buff[0]; }
//     uint16_t result_tail = tail;
//     ROTATE(tail, size);
//     return buff[result_tail];
//     enable_irq();
//   }
// 
//   bool is_empty() {
//     disable_irq();
//     uint8_t result = head == tail ? true : false;
//     enable_irq();
//     return result;
//   }
//   bool is_full() {
//     disable_irq();
//     uint8_t result = ROTATE_IF(head, size) == tail ? true : false;
//     enable_irq();
//     return result;
//   }
// 
//   uint16_t get_size() {
//     uint8_t result = 0;
//     disable_irq();
//     if ( head >= tail )  {
//       result = head - tail;
//     } else {
//       result = size - (tail - head);
//     }
//     enable_irq();
//     return result;
//   }
// 
//   // true false
//   void atomic(uint8_t is_atomic) { this->is_atomic = is_atomic; }
// 
// private:
//   void disable_irq() { if ( is_atomic ) __disable_irq(); }
//   void enable_irq()  { if ( is_atomic ) __enable_irq(); }
// };

#ifdef STANDARD_RING_H_TEST

Ring<BUFF_SIZE> buff_test;

void setup() {
  Serial.begin(38400);
  SP("--> Start ALT TEST Standard_Ring.h");
}
void loop() {
}
#endif

#endif
#endif
