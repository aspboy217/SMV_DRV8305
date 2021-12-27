#include "controller.hpp"

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
 *                 CONTROL FUNCTIONS                    *
 ********************************************************/
void startup() {
  Serial.println("startup");
  delay(300);
  while(true) {
    if(digitalRead(nFAULT) == HIGH)
      break;
  }
  Serial.println("Standby Mode");
  active = false;
}

/* Setting driving modes & alerts */
void setMotor(){
  Serial.println("setMotor");
  delay(300);
  // manually write modes
  // read all registers and store them into the status struct
  uint16_t data = 0;

  /* Set modes to 0x5 - HS Gate Drive Control */
  data = CNTL_HS_DATA;
  while(true){
    if(writeReg(CNTL_HS, data) == data)
      break;
  }

  /* Set modes to 0x6 - LS Gate Drive Control */
  data = CNTL_LS_DATA;
  while(true){
    if(writeReg(CNTL_LS, data) == data)
      break;
  }

  /* Set modes to 0x7 - Gate Drive Control */
  data = CNTL_DRV_DATA; 
  while(true){
    if(writeReg(CNTL_DRV, data) == data)
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
    if(writeReg(CNTL_VREG, data) == data)
      break;
  }

  /* Set modes to 0xC - VDS Sense Control */
  data = CNTL_VDS_DATA;
  while(true){
    if(writeReg(CNTL_VDS, data) == data)
      break;
  }
}

/* Check All registers and store the status into struct */
void checkAll(){
  Serial.println("checkAll");
}

bool operate() {
  Serial.println("operate");
  if(checkFault())
    return false;

  if(!active)
    active = true;
  digitalWrite(EN_GATE, HIGH);
  Serial.print("Operation Mode");
  return true;
}

void handleFault(){
  Serial.println("handleFault");
  if(!checkFault()){
    if(operate()) // if fault is cleared
      return;
  }

  Serial.println("Get into while loop until fault resolved");  
  while(true){
    if(!checkFault())
      break;
  }
  if(operate())
    return;
  // handle cases
  // call operate if everything is cleared
}

/********************************************************
 *              REG CHECKIGN FUNCTIONS                  *
 ********************************************************/
/* return true if there is fault 
 * return false if there's no fault */
bool checkFault(){
  Serial.println("checkFault");
  uint16_t status = readReg(WARN);
  if(status & 0x400){
    Serial.println("checkFault - Fault Flag!");
    active = false;
    return true;
  }
  return false;
}

void checkWarningReg(){
  Serial.println("checkWarningReg");
  Serial.println("Checking 0x1");
  uint16_t status = readReg(WARN);

  if(status != 0){
    /* Temperature Monitor */
    if(status & 0x008) 
      Serial.println("D3: Temp > 105C");
    if(status & 0x004)
      Serial.println("D2: Temp > 125C");
    if(status & 0x002)
      Serial.println("D1: Temp > 135C");
    if(status & 0x100)
      Serial.println("D8: Temp > 175C");
    if(status & 0x001)
      Serial.println("D0: Overtemperature warning");
    
    /* Voltage Monitor */
    if(status & 0x080)
      Serial.println("D7: PVDD undervoltage warning");
    if(status & 0x040)
      Serial.println("D6: PVDD overvoltage warning");
    if(status & 0x020)
      Serial.println("D5: VDS overcurrent warning");
    if(status & 0x010)
      Serial.println("D4: Charge pump undervoltage warning");

    /* Check for faults */
    if(status & 0x400)
      Serial.println("D10: Fault Indication");
  }
  Serial.println("Done 0x1");
}

/* Check Fault addresses */
void checkFaultReg(){
  Serial.println("checkFaultReg");
  uint16_t status = 0;

  /* Check 0x2 - OV/VDS Faults */
  Serial.println("Checking 0x2 - Overcurrent Faults");
  status = readReg(FLT_OC);
  if(status != 0){
    if(status & 0x001)
      Serial.println("D0: Sense A overcurrent fault");
    if(status & 0x002)
      Serial.println("D1: Sense B overcurrent fault");
    if(status & 0x004)
      Serial.println("D2: Sense C overcurrent fault");
    if(status & 0x400)
      Serial.println("D10: VDS overcurrent fault for HS A");
    if(status & 0x200)
      Serial.println("D9: VDS overcurrent fault for LS A");
    if(status & 0x100)
      Serial.println("D8: VDS overcurrent fault for HS B");
    if(status & 0x080)
      Serial.println("D7: VDS overcurrent fault for LS B");
    if(status & 0x040)
      Serial.println("D6: VDS overcurrent fault for HS C");
    if(status & 0x020)
      Serial.println("D5: VDS overcurrent fault for LS C");
  }
  Serial.println("Done 0x2");

  /* Check 0x3 - IC Faults */
  Serial.println("Checking 0x3 - IC Faults");
  status = readReg(FLT_IC);
  if(status != 0){
    if(status & 0x400)
      Serial.println("D10: PVDD undervoltage 2 fault");
    if(status & 0x200)
      Serial.println("D9: Watchdog fault");
    if(status & 0x100)
      Serial.println("D8: Overtemperature fault");
    if(status & 0x040)
      Serial.println("D6: VREG undervoltage fault");
    if(status & 0x020)
      Serial.println("D5: AVDD undervoltage fault");
    if(status & 0x010)
      Serial.println("D4: LS Gate supply fault");
    if(status & 0x004)
      Serial.println("D2: HS charge pump undervoltage 2 fault");
    if(status & 0x002)
      Serial.println("D1: HS charge pump overvoltage fault");
    if(status & 0x001)
      Serial.println("D0: HS charge pump overvoltage ABS fault");
  }
  Serial.println("Done 0x3");
    
  /* Check 0x4 - Gate VGS Faults*/
  Serial.println("Checking 0x4 - Gate VGS Faults");
  status = readReg(FLT_VGS);
  if(status != 0){
    if(status & 0x400)
      Serial.println("D10: VGS gate drive fault HS A");
    if(status & 0x200)
      Serial.println("D9: VGS gate drive fault LS A");
    if(status & 0x100)
      Serial.println("D8: VGS gate drive fault HS B");
    if(status & 0x080)
      Serial.println("D7: VGS gate drive fault LS B");
    if(status & 0x040)
      Serial.println("D6: VGS gate drive fault HS C");
    if(status & 0x020)
      Serial.println("D5: VGS gate drive fault LS C");
  }
  Serial.println("Done 0x3");
}