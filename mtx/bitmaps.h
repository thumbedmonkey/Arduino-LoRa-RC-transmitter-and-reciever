#ifndef BITMAPS_H_
#define BITMAPS_H_

#include <avr/pgmspace.h>

static const unsigned char PROGMEM point[] = {
  // 'point, 6x7px
  0x20, 0x30, 0xf8, 0xfc, 0xf8, 0x30, 0x20
};

static const unsigned char pow_icon[] PROGMEM = {
  // 'power_icon, 4x5px
  0x20, 0x40, 0xf0, 0x20, 0x40
};

static const unsigned char rf_icon[] PROGMEM = {
  // 'rf_icon, 7x7px
  0x38, 0x44, 0x82, 0x38, 0x44, 0x10, 0x38
};

static const unsigned char cut_icon[] PROGMEM = {
  // 'cut_icon', 13x6px
  0xff, 0xf8, 0xca, 0x88, 0xba, 0xd8, 0xba, 0xd8, 0xc8, 0xd8, 0xff, 0xf8
};

static const unsigned char dualrate_icon[] PROGMEM = {
  // 'dualrate_icon', 13x6px
  0xff, 0xf8, 0x9e, 0x98, 0xad, 0xa8, 0xad, 0x98, 0x9b, 0xa8, 0xff, 0xf8
};

static const unsigned char warningTriangle[] PROGMEM = {
  // 'warning', 16x15px
  0x01, 0x80, 0x02, 0x40, 0x02, 0x40, 0x04, 0x20, 0x05, 0xa0, 0x09, 0x90, 0x09, 0x90, 0x11, 0x88,
  0x11, 0x88, 0x21, 0x84, 0x20, 0x04, 0x41, 0x82, 0x41, 0x82, 0x80, 0x01, 0xff, 0xff
};

static const unsigned char checkbox_checked[] PROGMEM = {
  // 'checkbox_checked, 7x7px
  0xfa, 0x86, 0x8c, 0xda, 0xf2, 0xa2, 0xfe
};

static const unsigned char checkbox_unchecked[] PROGMEM = {
  // 'checkbox_unchecked, 7x7px
  0xfe, 0x82, 0x82, 0x82, 0x82, 0x82, 0xfe
};

static const unsigned char lock_icon [] PROGMEM = {
  // 'lock_icon, 6x7px
  0x30, 0x48, 0x48, 0xfc, 0xfc, 0xfc, 0xfc
};

static const unsigned char mute_icon [] PROGMEM = {
  // 'mute_icon, 7x7px
  0x20, 0x60, 0xea, 0xe4, 0xea, 0x60, 0x20
};

static const unsigned char menu_icon [] PROGMEM = {
// 'menu_icon', 8x7px
	0x00, 0x5e, 0x00, 0x5e, 0x00, 0x5e, 0x00
};
static const unsigned char menu_icon_focused [] PROGMEM = {
// 'menu_icon_focused', 8x7px
	0xff, 0xa1, 0xff, 0xa1, 0xff, 0xa1, 0xff
};

#endif
