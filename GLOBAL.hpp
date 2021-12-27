#ifndef GLOBAL_H
#define GLOBAL_H

/* PIN DEFINITIONS */
#define nSCS    10 
#define nFAULT  7
#define EN_GATE 8
#define THR_IN  A0
#define THR_OUT 3

bool active;
// true  - operating state
// false - standby, sleep state

/* REGISTER DATA */
#define CNTL_HS_DATA    0x344 // default
#define CNTL_LS_DATA    0x344 // default
#define CNTL_DRV_DATA   0x316 // diode freewheeling with 1PWM
#define CNTL_IC_DATA    0x420 // default
#define CNTL_SHAMP_DATA 0x000 // default
#define CNTL_VREG_DATA  0x10A // default
#define CNTL_VDS_DATA   0x0C8 // default

#endif