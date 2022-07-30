/*******************************************************************************
* This provides a Ring Buffer or Circular Buffer with interrupt protection.     
* Mainly used for audio buffer transfers.                                       
*******************************************************************************/

#ifdef INCLUDE_STANDARD_RING
#ifndef RING_H
#define RING_H

#define ROTATE(x,sz)    if(x+1==sz) {x = 0;} else {x+=1;}
#define ROTATE_IF(x,sz) (x+1==sz ? 0 : x+1)

template <typename T>
struct Ring {
  T        *buff_t;
  uint16_t size, head, tail;
  uint8_t  is_atomic;

  Ring(uint16_t size) {
    this->size = size;
    buff_t     = new T [size];
    is_atomic  = true;
    head = tail = 0;
  }
  ~Ring() { for (int i=0; i<size; i++) delete &buff_t[i]; }

  //void insert(T *item) { insert(*item); }
  void insert(T item) {
    disable_irq();
    buff_t[head] = item;
    ROTATE(head, size);
    if ( head == tail ) { ROTATE(tail, size); }
    enable_irq();
  }

  T pop() {
    disable_irq();
    if ( head == tail ) { return buff_t[0]; }
    uint16_t result_tail = tail;
    ROTATE(tail, size);
    return buff_t[result_tail];
    enable_irq();
  }

  bool is_empty() {
    disable_irq();
    uint8_t result = head == tail ? true : false;
    enable_irq();
    return result;
  }
  bool is_full() {
    disable_irq();
    uint8_t result = ROTATE_IF(head, size) == tail ? true : false;
    enable_irq();
    return result;
  }

  uint16_t get_size() {
    uint8_t result = 0;
    disable_irq();
    if ( head >= tail )  {
      result = head - tail;
    } else {
      result = size - (tail - head);
    }
    enable_irq();
    return result;
  }

  // true false
  void atomic(uint8_t is_atomic) { this->is_atomic = is_atomic; }

private:
  void disable_irq() { if ( is_atomic ) __disable_irq() }
  void enable_irq()  { if ( is_atomic ) __enable_irq() }
};

#endif
#endif
