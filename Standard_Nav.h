#ifndef NAV_H
#define NAV_H

struct Nav {
  uint8_t init  = false;
  uint8_t event = 0;

  uint8_t not_init() {
    uint8_t result = init;
    init           = true;
    return !result; 
  }

  void reset() { init = false; }

  void jump_to(uint8_t event) {
    this->event = event; 
    reset();
  }

  uint8_t e() { return event; }
};

#endif
