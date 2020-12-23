#ifndef BITMAPS_H_
#define BITMAPS_H_

#include <avr/pgmspace.h>

static const unsigned char PROGMEM point[] = {
  // 6x7px
  0x20, 0x30, 0xf8, 0xfc, 0xf8, 0x30, 0x20
};

static const unsigned char pow_icon[] PROGMEM = {
  // 4x5px
  0x20, 0x40, 0xf0, 0x20, 0x40
};

static const unsigned char rf_icon[] PROGMEM = {
  // 7x7px
  0x44, 0x82, 0xba, 0x92, 0x54, 0x10, 0x10
};

static const unsigned char cut_icon[] PROGMEM = {
  // 13x6px
  0xff, 0xf8, 0xca, 0x88, 0xba, 0xd8, 0xba, 0xd8, 0xc8, 0xd8, 0xff, 0xf8
};

static const unsigned char dualrate_icon[] PROGMEM = {
  // 13x6px
  0xff, 0xf8, 0x9e, 0x98, 0xad, 0xa8, 0xad, 0x98, 0x9b, 0xa8, 0xff, 0xf8
};

static const unsigned char checkbox_checked[] PROGMEM = {
  // 7x7px
  0xfa, 0x86, 0x8c, 0xda, 0xf2, 0xa2, 0xfe
};

static const unsigned char checkbox_unchecked[] PROGMEM = {
  // 7x7px
  0xfe, 0x82, 0x82, 0x82, 0x82, 0x82, 0xfe
};

static const unsigned char lock_icon [] PROGMEM = {
  // 6x7px
  0x30, 0x48, 0x48, 0xfc, 0xfc, 0xfc, 0xfc
};

static const unsigned char mute_icon [] PROGMEM = {
  // 7x7px
  0x20, 0x60, 0xea, 0xe4, 0xea, 0x60, 0x20
};

static const unsigned char menu_icon [] PROGMEM = {
  // 8x7px
	0x00, 0x5e, 0x00, 0x5e, 0x00, 0x5e, 0x00
};

static const unsigned char menu_icon_focused [] PROGMEM = {
  // 8x7px
	0xff, 0xa1, 0xff, 0xa1, 0xff, 0xa1, 0xff
};

static const unsigned char chA_icon0 [] PROGMEM = {
  // 9x9px
	0xff, 0x80, 0x88, 0x80, 0x94, 0x80, 0xa2, 0x80, 0xa2, 0x80, 0xbe, 0x80, 0xa2, 0x80, 0xa2, 0x80, 
	0xff, 0x80
};

static const unsigned char chA_icon1 [] PROGMEM = {
	// 9x9px
	0xff, 0x80, 0xf7, 0x80, 0xeb, 0x80, 0xdd, 0x80, 0xdd, 0x80, 0xc1, 0x80, 0xdd, 0x80, 0xdd, 0x80, 
	0xff, 0x80
};

static const unsigned char chB_icon0 [] PROGMEM = {
  // 9x9px
	0xff, 0x80, 0xbc, 0x80, 0x92, 0x80, 0x92, 0x80, 0x9c, 0x80, 0x92, 0x80, 0x92, 0x80, 0xbc, 0x80, 
	0xff, 0x80
};

static const unsigned char chB_icon1 [] PROGMEM = {
	// 9x9px
	0xff, 0x80, 0xc3, 0x80, 0xed, 0x80, 0xed, 0x80, 0xe3, 0x80, 0xed, 0x80, 0xed, 0x80, 0xc3, 0x80, 
	0xff, 0x80
};

static const unsigned char switchDown_icon [] PROGMEM = {
  // 5x7px
	0x20, 0x20, 0x20, 0x20, 0x88, 0x70, 0x20
};
static const unsigned char switchMid_icon [] PROGMEM = {
  // 5x7px
	0x00, 0x00, 0x00, 0x70, 0x00, 0x00, 0x00
};

static const unsigned char switchUp_icon [] PROGMEM = {
  // 5x7px
	0x20, 0x70, 0x88, 0x20, 0x20, 0x20, 0x20
};

static const unsigned char asterisk_small [] PROGMEM = {
	//3x3px
	0xa0, 0x40, 0xa0
};

#endif
