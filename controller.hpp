#ifndef SPI_COMM_H
#define SPI_COMM_H
#include <SPI.h>
#include "GLOBAL.hpp"

/* SPI SETTING CONSTS */
#define SPISPD            SPI_CLOCK_DIV4  // Set SPI Clock Speed
#define BITORDER          MSBFIRST        // Set which bit to transfer first
#define SPIMODE           SPI_MODE1       // Set SPI Mode
#define MICRODELAY        20              // Set delay amount to receive data from Slave (microsec)
#define TRANSFERDEALAY    1000            // Set delay amount to transfer data (millisec)- > 500ns

/* SPI ADDR */
// STATUS REGS (R ONLY)
#define WARN       0x1 // Warning
#define FLT_OC     0x2 // Overcurrent faults
#define FLT_IC     0x3 // IC Faults
#define FLT_VGS    0x4 // Gate Driver VGS Faults
// CONTROL REGS (R & W)
#define CNTL_HS    0x5 // HS Gate Drv Control
#define CNTL_LS    0x6 // LS Gate Drv Control
#define CNTL_DRV   0x7 // Gate Drv Control - modes
#define CNTL_IC    0x9 // IC Operation - Fault & Watchdog control
#define CNTL_SHAMP 0xA // Shunt Amp Control
#define CNTL_VREG  0xB // Voltage Regulator Control
#define CNTL_VDS   0xC // VDS Sense Control

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

/* REGISTER CHECKING FUNCTIONS */
/* Check Fault bit only (0x1 D10) */
bool checkFault();

/* Check Register 0x1 & return true if there is fault */
void checkWarningReg();

/* Check Register 0x2, 0x3, 0x4 */
void checkFaultReg();

#endif