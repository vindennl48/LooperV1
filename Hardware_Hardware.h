/* I believe this library is fairly self-explanatory. Used to keep all of the
 * hardware peripherals in one location */

#ifndef HARDWARE_H
#define HARDWARE_H

#define SERIAL_USB  0
#define SERIAL_MIDI 1

using namespace BALibrary;

struct HW {
  static BAAudioControlWM8731 codecControl;

  static BTN btn_left;
  static BTN btn_right;

  static void setup() {
    /* ::Setup the dev-board:: */
    // Declare the version of the TGA Pro you are using.
    TGA_PRO_MKII_REV1();

    delay(5); // wait a few ms to make sure the TGA Pro is fully powered up
    /* Provide an arbitrarily large number of audio buffers (48 blocks) for
     * the effects (delays use a lot more than others) */
    AudioMemory(48);

    // If the codec was already powered up (due to reboot) power it down first
    codecControl.disable();
    delay(100);
    codecControl.enable();
    delay(100);

    pinMode(BA_EXPAND_LED1_PIN, OUTPUT); led1(OFF);
    pinMode(BA_EXPAND_LED2_PIN, OUTPUT); led2(OFF);


    /* ::Setup hardware:: */
    pinMode(PinLEDBoard, OUTPUT); board_led(OFF);
    btn_left.setup(BA_EXPAND_SW1_PIN, false);
    btn_right.setup(BA_EXPAND_SW2_PIN, false);
  }

  static void setup_serial(uint8_t type) {
    if      ( type == SERIAL_USB  ) Serial.begin(38400);
    else if ( type == SERIAL_MIDI ) Serial.begin(31250);
  }

  static void loop() {
    btn_left.loop();
    btn_right.loop();
  }

  static void board_led(uint8_t state) {
    if ( state == TOGGLE ) digitalWrite(PinLEDBoard, !digitalRead(PinLEDBoard));
    else digitalWrite(PinLEDBoard, state);
  }

  static void led1(uint8_t state) {
    if ( state == TOGGLE ) digitalWrite(BA_EXPAND_LED1_PIN, !digitalRead(BA_EXPAND_LED1_PIN));
    else digitalWrite(BA_EXPAND_LED1_PIN, state);
  }

  static void led2(uint8_t state) {
    if ( state == TOGGLE ) digitalWrite(BA_EXPAND_LED2_PIN, !digitalRead(BA_EXPAND_LED2_PIN));
    else digitalWrite(BA_EXPAND_LED2_PIN, state);
  }
};

#endif
