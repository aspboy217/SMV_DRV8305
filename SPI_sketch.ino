#include <SPI.h>

#define MAX_DATA 11

// PIN DEFS
const int SS   = 9; // enable SPI (active low) nSCS
const int MOSI = 10; // SPI input SDI
const int MISO = 11; // SPI output SDO

// REG ADDRESS
const int WARNINGS          = 0x1;
const int OV_VDS_FAULTS     = 0x2;
const int IC_FAULTS         = 0x3;
const int VGS_FAULTS        = 0x4;
const int H_GATE_CONTROL    = 0x5;
const int L_GATE_CONTROL    = 0x6;
const int GATE_CONTROL      = 0x7;
const int IC_OPERATION      = 0x9;
const int SHUNT_AMP_CONTROL = 0xA;
const int VR_CONTROL        = 0xB;
const int VDC_CONTROL       = 0xC;

// FLAG ARRAYS
bool warning_Flags[MAX_DATA]    = { false };
bool OV_Flags[MAX_DATA]         = { false };
bool IC_Fault_Flags[MAX_DATA]   = { false };
bool VGS_Flags[MAX_DATA]        = { false };
bool H_Gate_Flags[MAX_DATA]     = { false };
bool L_Gate_Flags[MAX_DATA]     = { false };
bool Gate_Drive_Flags[MAX_DATA] = { false };
bool IC_Ops_Flags[MAX_DATA]     = { false };
bool Shunt_Amp_Flags[MAX_DATA]  = { false };
bool VR_Flags[MAX_DATA]         = { false };
bool VDS_Flags[MAX_DATA]        = { false };


void setup() {
  Serial.begin();
  SPI.begin();
  
  pinMode(MOSI, OUTPUT);
  pinMode(SS, OUTPUT);
  pinMode(MISO, INPUT);



}

void loop() {

}
