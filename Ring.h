#ifndef RING_H
#define RING_H

#define BUFF_SIZE 128   // 0 - 127
#define ROTATE(x) if(x+1==BUFF_SIZE) {x = 0;} else {x++;}

struct Ring {
  int buff[BUFF_SIZE] = {0};
  int  head           = 0;
  int  tail           = 0;

  void insert(int data) {
    buff[head] = data;
    ROTATE(head);

    if ( head == tail ) { ROTATE(tail); }
  }

  uint8_t is_empty() { return head == tail ? true : false; }

  int get() {
    if ( head == tail ) return 0; // CHECK IF EMPTY FIRST!!

    int result = buff[tail];
    ROTATE(tail);

    return result;
  }

  int size() {
    if ( head >= tail ) return head - tail;
    else return (head + (BUFF_SIZE - tail));
  }
};

//#undef BUFF_SIZE
//#undef ROTATE

#endif
