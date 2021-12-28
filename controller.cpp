#include "controller.hpp"
#include "global.hpp"

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

/********************************************************
 *                 WarningReg METHODS                   *
 ********************************************************/
WarningReg::WarningReg(){
  resetReg();
}

void WarningReg::checkWarningReg(){
  resetReg();
  uint16_t status = readReg(WARN);
  if(status != 0){
    if(status & 0x400)
      FAULT = true;
    if(status & 0x100)
      TEMP_FLAG4 = true;
    if(status & 0x080)
      PVDD_UVFL = true;
    if(status & 0x040)
      PVDD_OVFL = true;
    if(status & 0x020)
      VDS_STATUS = true;
    if(status & 0x010)
      VCHP_UVFL = true;
    if(status & 0x008)
      TEMP_FLAG1 = true;
    if(status & 0x004)
      TEMP_FLAG2 = true;
    if(status & 0x002)
      TEMP_FLAG3 = true;
    if(status & 0x001)
      OTW = true;
  }
}

void WarningReg::printWarningReg(){
  Serial.println("Printing Warning Registers");
  if(FAULT)
    Serial.println("D10: Fault Indication");
  if(TEMP_FLAG4)
    Serial.println("D8: Temp > 175C");
  if(PVDD_UVFL)
    Serial.println("D7: PVDD undervoltage warning");
  if(PVDD_OVFL)
    Serial.println("D6: PVDD overvoltage warning");
  if(VDS_STATUS)
    Serial.println("D5: VDS overcurrent warning");
  if(VCHP_UVFL)
    Serial.println("D4: Charge pump undervoltage warning");
  if(TEMP_FLAG1)
    Serial.println("D3: Temp > 105C");
  if(TEMP_FLAG2)
    Serial.println("D2: Temp > 125C");
  if(TEMP_FLAG3)
    Serial.println("D1: Temp > 135C");
  if(OTW)
    Serial.println("D0: Overtemperature warning");
  Serial.println("Done");
}

void WarningReg::resetReg(){
  FAULT = false; PVDD_UVFL = false; PVDD_OVFL = false;
  TEMP_FLAG4 = false; TEMP_FLAG3 = false; TEMP_FLAG2 = false; TEMP_FLAG1 = false; 
  VDS_STATUS = false; VCHP_UVFL = false; OTW = false;
}

/********************************************************
 *                   FaultReg METHODS                   *
 ********************************************************/
FaultReg::FaultReg(){
  resetReg();
}

void FaultReg::checkFaultReg(){
  resetReg();

  uint16_t status = 0;
  status = readReg(FLT_OC);
  if(status != 0){
    if(status & 0x001)
      SNS_A_OCP = true;
    if(status & 0x002)
      SNS_B_OCP = true;
    if(status & 0x004)
      SNS_C_OCP = true;
    if(status & 0x400)
      VDS_HA = true;
    if(status & 0x200)
      VDS_LA = true;
    if(status & 0x100)
      VDS_HB = true;
    if(status & 0x080)
      VDS_LB = true;
    if(status & 0x040)
      VDS_HC = true;
    if(status & 0x020)
      VDS_LC = true;
  }

  status = readReg(FLT_IC);
  if(status != 0){
    if(status & 0x400)
      PVDD_UVLO2 = true;
    if(status & 0x200)
      WD_FAULT = true;
    if(status & 0x100)
      OTSD = true;
    if(status & 0x040)
      VREG_UV = true;
    if(status & 0x020)
      AVDD_UVLO = true;
    if(status & 0x010)
      VCP_LSD_UVLO2 = true;
    if(status & 0x004)
      VCHP_UVLO2 = true;
    if(status & 0x002)
      VCHP_OVLO = true;
    if(status & 0x001)
      VCHP_OVLO_ABS = true;
  }
    
  status = readReg(FLT_VGS);
  if(status != 0){
    if(status & 0x400)
      VGS_HA = true;
    if(status & 0x200)
      VGS_LA = true;
    if(status & 0x100)
      VGS_HB = true;
    if(status & 0x080)
      VGS_LB = true;
    if(status & 0x040)
      VGS_HC = true;
    if(status & 0x020)
      VGS_LC = true;
  }
}

void FaultReg::printFaultReg(){
  Serial.println("Printing Fault Registers");

  Serial.println("Checking 0x2 - Overcurrent Faults");
  if(SNS_A_OCP)
    Serial.println("D0: Sense A overcurrent fault");
  if(SNS_B_OCP)
    Serial.println("D1: Sense B overcurrent fault");
  if(SNS_C_OCP)
    Serial.println("D2: Sense C overcurrent fault");
  if(VDS_HA)
    Serial.println("D10: VDS overcurrent fault for HS A");
  if(VDS_LA)
    Serial.println("D9: VDS overcurrent fault for LS A");
  if(VDS_HB)
    Serial.println("D8: VDS overcurrent fault for HS B");
  if(VDS_LB)
    Serial.println("D7: VDS overcurrent fault for LS B");
  if(VDS_HC)
    Serial.println("D6: VDS overcurrent fault for HS C");
  if(VDS_LC)
    Serial.println("D5: VDS overcurrent fault for LS C");
  Serial.println("Done 0x2");

  Serial.println("Checking 0x3 - IC Faults");
  if(PVDD_UVLO2)
    Serial.println("D10: PVDD undervoltage 2 fault");
  if(WD_FAULT)
    Serial.println("D9: Watchdog fault");
  if(OTSD)
    Serial.println("D8: Overtemperature fault");
  if(VREG_UV)
    Serial.println("D6: VREG undervoltage fault");
  if(AVDD_UVLO)
    Serial.println("D5: AVDD undervoltage fault");
  if(VCP_LSD_UVLO2)
    Serial.println("D4: LS Gate supply fault");
  if(VCHP_UVLO2)
    Serial.println("D2: HS charge pump undervoltage 2 fault");
  if(VCHP_OVLO)
    Serial.println("D1: HS charge pump overvoltage fault");
  if(VCHP_OVLO_ABS)
    Serial.println("D0: HS charge pump overvoltage ABS fault");
  Serial.println("Done 0x3");
    
  Serial.println("Checking 0x4 - Gate VGS Faults");
  if(VGS_HA)
    Serial.println("D10: VGS gate drive fault HS A");
  if(VGS_LA)
    Serial.println("D9: VGS gate drive fault LS A");
  if(VGS_HB)
    Serial.println("D8: VGS gate drive fault HS B");
  if(VGS_LB)
    Serial.println("D7: VGS gate drive fault LS B");
  if(VGS_HC)
    Serial.println("D6: VGS gate drive fault HS C");
  if(VGS_LC)
    Serial.println("D5: VGS gate drive fault LS C");
  Serial.println("Done 0x3");
}

void FaultReg::clearFault(){
  uint16_t data = CNTL_IC_DATA | CLR_FLTS;
  while(true){
    if(writeReg(CNTL_IC, data) == data)
      break;
  }
}

void FaultReg::resetReg(){
  VDS_HA = false; VDS_LA= false; VDS_HB= false; VDS_LB = false; VDS_HC = false; VDS_LC = false;
  SNS_A_OCP = false; SNS_B_OCP = false; SNS_C_OCP = false;
  PVDD_UVLO2 = false; WD_FAULT = false; OTSD = false; VREG_UV = false; AVDD_UVLO = false;
  VCP_LSD_UVLO2 = false; VCHP_UVLO2 = false; VCHP_OVLO = false; VCHP_OVLO_ABS = false;
  VGS_HA = false; VGS_LA = false; VGS_HB = false; VGS_LB = false; VGS_HC = false; VGS_LC = false;
}