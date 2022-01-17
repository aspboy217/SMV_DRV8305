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

void delay1();

class Controller {
public:
  Controller();
  void startup();
  void handleFault();
  void checkFaultPin();
  bool isActive();
  void standby();
  bool operate();
private:
  void checkWarningReg();
  void checkFaultReg();
  void clearFault();

  bool checkFault(); /* Check Fault bit only (0x1 D10) */
  void setMotor();
  bool active;
};

#endif