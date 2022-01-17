#include "controller.hpp"
#include "global.hpp"

void delay1() {
  long tmp;
  for(int i = 0; i < 10; i++){
    tmp = 0xffffffff;
      while(tmp > 0) {tmp--;}
  }
}

/********************************************************
 *                    SPI FUNCTIONS                     *
 ********************************************************/
void SPI_init() {
  SPI.begin();
  
  digitalWrite(nSCS, HIGH);
  SPI.setDataMode(SPIMODE);
  SPI.setBitOrder(BITORDER);
  SPI.setClockDivider(SPISPD);
}

uint16_t readReg(uint8_t addr){
  uint16_t value = (0x1 << 15) | (addr << 11);
  
  digitalWrite(nSCS, LOW);
  value = SPI.transfer16(value);
  delayMicroseconds(MICRODELAY);
  digitalWrite(nSCS, HIGH); // disable Slave Select

  return value;
}

uint16_t writeReg(uint8_t addr, uint16_t data){
  uint16_t value = (addr << 11) | data;

  digitalWrite(nSCS, LOW);
  value = SPI.transfer16(value);
  delayMicroseconds(MICRODELAY);
  digitalWrite(nSCS, HIGH); // disable Slave Select

  return value;
}

/********************************************************
 *                 CONTROLLER FUNCTIONS                 *
 ********************************************************/
Controller::Controller() {
  active = false;
}

bool Controller::isActive() {
  return active;
}

// OPERATION RELATED ************************************
void Controller::startup() {
  digitalWrite(STARTUP, HIGH);

  handleFault();

  while(true) {
    if(digitalRead(nFAULT) == HIGH)
      break;
  };

  setMotor();
  standby();
}

void Controller::standby() {
  if(active)
    active = false;

  digitalWrite(STARTUP, LOW);
  digitalWrite(EN_GATE, LOW);
  digitalWrite(OPERATE, LOW);
  digitalWrite(STANDBY, HIGH);
}

bool Controller::operate() {
  if(checkFault())
    return false;

  if(!active)
    active = true;

  digitalWrite(STANDBY, LOW);
  digitalWrite(OPERATE, HIGH);
  digitalWrite(EN_GATE, HIGH);
  return true;
}

// FAULT RELATED ************************************
void Controller::checkFaultPin(){
  if(digitalRead(nFAULT) == LOW) {
    if(checkFault())
      checkFaultReg();
    else
      checkWarningReg();
  }
}

void Controller::handleFault(){
  while(true){
    clearFault();
    if(!checkFault())
      break;
  }
}

/* return true if there is fault 
 * return false if there's no fault */
bool Controller::checkFault(){
  uint16_t status = readReg(WARN);
  if(status & 0x400){
    standby();
    return true;
  }
  return false;
}

// OTHERS ******************************************
/* Setting driving modes & alerts */
void Controller::setMotor(){
  uint16_t data = 0;

  /* Set modes to 0x5 - HS Gate Drive Control */
  data = CNTL_HS_DATA;
  while(true){
    if((writeReg(CNTL_HS, data) & 0x3ff) == data)
      break;
  }

  /* Set modes to 0x6 - LS Gate Drive Control */
  data = CNTL_LS_DATA;
  while(true){
    if((writeReg(CNTL_LS, data) & 0x3ff) == data)
      break;
  }

  /* Set modes to 0x7 - Gate Drive Control */
  data = CNTL_DRV_DATA; 
  while(true){
    if((writeReg(CNTL_DRV, data) & 0x3ff) == data)
      break;
  }

  /* Set modes to 0x9 - IC Operation */
  data = CNTL_IC_DATA;
  while(true){
    if(writeReg(CNTL_IC, data) == data)
      break;
  }

  /* Set modes to 0xA - Shunt Amplifier */
  data = CNTL_SHAMP_DATA;
  while(true){
    if(writeReg(CNTL_SHAMP, data) == data)
      break;
  }

  /* Set modes to 0xB - Voltage Regulator */
  data = CNTL_VREG_DATA;
  while(true){
    if((writeReg(CNTL_VREG, data) & 0x31f) == data)
      break;
  }

  /* Set modes to 0xC - VDS Sense Control */
  data = CNTL_VDS_DATA;
  while(true) {
    if((writeReg(CNTL_VDS, data) & 0xff) == data)
      break;
  }
}

void Controller::checkWarningReg(){
  warning = readReg(WARN);
}

void Controller::checkFaultReg(){
  ovs_flt = readReg(FLT_OC);
  ic_flt = readReg(FLT_IC);
  vgs_flt = readReg(FLT_VGS);
}

void Controller::clearFault(){
  digitalWrite(EN_GATE, HIGH);
  delay1();
  digitalWrite(EN_GATE, LOW);

  warning = 0; // 0x1
  ovs_flt = 0; // 0x2
  ic_flt  = 0; // 0x3
  vgs_flt = 0; // 0x4
}