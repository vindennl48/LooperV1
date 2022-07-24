#ifndef RING_H
#define RING_H

#define BUFF_SIZE 128   // 0 - 127
#define ROTATE(x) if(x+1==BUFF_SIZE) {x = 0;} else {x++;}

struct Ring {
  uint32_t buff[BUFF_SIZE] = {0};
  uint16_t head            = 0;
  uint16_t tail            = 0;

  void insert(uint32_t data) {
    buff[head] = data;
    ROTATE(head);

    if ( head == tail ) ROTATE(tail);
  }

  uint8_t is_empty() { return head == tail ? true : false; }

  uint32_t get() {
    if ( head == tail ) return 0; // CHECK IF EMPTY FIRST!!

    uint32_t result = buff[tail];
    ROTATE(tail);

    return result;
  }

  uint16_t size() {
    if ( head >= tail ) return head - tail;
    else return (head + (BUFF_SIZE - tail));
  }
};

//#undef BUFF_SIZE
//#undef ROTATE

#endif
