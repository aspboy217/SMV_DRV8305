#ifndef CONTROLLER_HPP
#define CONTROLLER_HPP
#include <SPI.h>

/* SPI SETTING CONSTS */
#define SPISPD            SPI_CLOCK_DIV4  // Set SPI Clock Speed
#define BITORDER          MSBFIRST        // Set which bit to transfer first
#define SPIMODE           SPI_MODE1       // Set SPI Mode
#define MICRODELAY        20              // Set delay amount to receive data from Slave (microsec)
#define TRANSFERDEALAY    1000            // Set delay amount to transfer data (millisec)- > 500ns

/* SPI COMM FUNCTIONS */
void SPI_init();
uint16_t readReg(uint8_t addr);
uint16_t writeReg(uint8_t addr, uint16_t data);

/* CONTROLLER FUNCTIONS */
void startup();
void setMotor();
void checkAll();
bool operate();
void handleFault();
bool checkFault(); /* Check Fault bit only (0x1 D10) */
// void sleep();

class WarningReg {
public:
  WarningReg();
  void checkWarningReg();
  void printWarningReg();
private:
  void resetReg();
  bool FAULT, TEMP_FLAG4, TEMP_FLAG3, TEMP_FLAG2, TEMP_FLAG1,
      PVDD_UVFL, PVDD_OVFL, VDS_STATUS, VCHP_UVFL, OTW;
};

class FaultReg {
public:
  FaultReg();
  void checkFaultReg();
  void printFaultReg();
  void clearFault();
private:
  void resetReg();
  bool VDS_HA, VDS_LA, VDS_HB, VDS_LB, VDS_HC, VDS_LC, SNS_A_OCP, SNS_B_OCP, SNS_C_OCP;
  bool PVDD_UVLO2, WD_FAULT, OTSD, VREG_UV, AVDD_UVLO, VCP_LSD_UVLO2, VCHP_UVLO2, VCHP_OVLO, VCHP_OVLO_ABS;
  bool VGS_HA, VGS_LA, VGS_HB, VGS_LB, VGS_HC, VGS_LC;
};

#endif