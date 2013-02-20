//-------------------------------------------------------------------
#ifndef __fancypants_main_H__
#define __fancypants_main_H__
//-------------------------------------------------------------------
 
#include <arduino.h>
#include <SPI.h>
 
//-------------------------------------------------------------------
 
//-------------------------------------------------------------------
 
// Put yout declarations here
 
//-------------------------------------------------------------------
 
//===================================================================
// -> DO NOT WRITE ANYTHING BETWEEN HERE...
// 		This section is reserved for automated code generation
// 		This process tries to detect all user-created
// 		functions in main_sketch.cpp, and inject their
// 		declarations into this file.
// 		If you do not want to use this automated process,
//		simply delete the lines below, with "&MM_DECLA" text
//===================================================================
//---- DO NOT DELETE THIS LINE -- @MM_DECLA_BEG@---------------------
void fill(uint32_t c, uint8_t first, uint8_t last);
void set_rb_pixel(uint16_t pixel, uint16_t pos, uint16_t cycle_step, uint8_t length);
void set_rb_cycle(uint16_t cycle_step, uint8_t first, uint8_t last);
void alternating_rainbows();
void rainbow_v_wipe();
void rainbow_h_wipe();
void bloom(boolean dir);
void rainbow_sash();
void rainbow_rain(bool diag);
void animStep();
void loop();
void setup();
//---- DO NOT DELETE THIS LINE -- @MM_DECLA_END@---------------------
// -> ...AND HERE. This space is reserved for automated code generation!
//===================================================================
 
 
//-------------------------------------------------------------------
#endif
//-------------------------------------------------------------------
 
