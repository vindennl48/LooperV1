/*******************************************************************************
* This provides a Ring Buffer or Circular Buffer with interrupt protection.     
* Mainly used for audio buffer transfers.                                       
*******************************************************************************/

#ifndef RING_H
#define RING_H

#define ROTATE(x,sz)    if(x+1==sz) {x = 0;} else {x++;}
#define ROTATE_IF(x,sz) (x+1==sz ? 0 : x+1)

template <typename T>
struct Ring {
  T *buff_t;
  uint16_t size, head, tail;

  Ring(uint16_t size) {
    this->size = size;
    buff_t = new T [size];
    head = tail = 0;
  }
  ~Ring() { for (int i=0; i<size; i++) delete &buff_t[i]; }

  void insert(T *item) { insert(*item); }
  void insert(T item) {
    __disable_irq();
    buff_t[head] = item;
    ROTATE(head, size);
    if ( head == tail ) { ROTATE(tail, size); }
    __enable_irq();
  }

  T pop() {
    __disable_irq();
    if ( head == tail ) { return buff_t[0]; }
    uint16_t result_tail = tail;
    ROTATE(tail, size);
    return buff_t[result_tail];
    __enable_irq();
  }

  bool is_empty() { return head == tail ? true : false; }
  bool is_full()  { return ROTATE_IF(head, size) == tail ? true : false; }
};

#endif
