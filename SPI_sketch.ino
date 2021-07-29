#include <SPI.h>

#define MAX_DATA 11

const int nSCS = 9; // enable SPI (active low)
const int SDI = 10; // SPI input
const int SDO = 11; // SPI output
const int SCLK = 12;// SPI clock

bool warning_Flags[MAX_DATA] = { false };
bool OV_Flags[MAX_DATA] = { false };
bool IC_Fault_Flags[MAX_DATA] = { false };
bool VGS_Flags[MAX_DATA] = { false };
bool H_Gate_Flags[MAX_DATA] = { false };
bool L_Gate_Flags[MAX_DATA] = { false };
bool Gate_Drive_Flags[MAX_DATA] = { false };
bool IC_Ops_Flags[MAX_DATA] = { false };
bool Shunt_Amp_Flags[MAX_DATA] = { false };
bool VR_Flags[MAX_DATA] = { false };
bool VDS_Flags[MAX_DATA] = { false };


void setup() {

}

void loop() {

}
