#include "controller.hpp"

void SPI_init() {
  SPI.begin();
  // set SCK, MOSI, SS to output mode
  // set SCK, MOSI into LOW
  // set SS into HIGH
  // turn SPI on
  
  digitalWrite(nSCS, HIGH); // to make sure
  SPI.setDataMode(SPIMODE);
  // can choose Mode: 0 ~ 3
  SPI.setBitOrder(BITORDER);
  // either MSBFIRST or LSBFIRST
  SPI.setClockDivider(SPISPD); // to slow down Master sampling speed
  // can choose divider: power of 2 up to 128 (2^7)
  // 2: fastest, 128: slowest
}

void writeData(uint16_t val) {
  digitalWrite(nSCS, LOW); // enable Slave Select
  SPI.transfer16(val);
  digitalWrite(nSCS, HIGH); // disable Slave Select
}

uint16_t readAndWriteData(uint16_t val) {
  uint16_t data = 0;
  digitalWrite(nSCS, LOW); // enable Slave Select
  data = SPI.transfer16(val);
  delayMicroseconds(MICRODELAY);
  digitalWrite(nSCS, HIGH); // disable Slave Select
  return data;
}

bool SPI_comm(ID id, uint16_t data, bool RWbit) {
  data = (data & (~(0x1f << 11))) | (RWbit << 15); // format data to transfer

  // this switch forms address of register into data
  switch (id) {
    // read only
    case ID_WARN:
      if (RWbit == WRITEBIT)
        return false;
      data = data | (ADDR_WARN << 11);
      break;
    case ID_OV_VDS_F:
      if (RWbit == WRITEBIT)
        return false;
      data = data | (ID_OV_VDS_F << 11);
      break;
    case ID_IC_F:
      if (RWbit == WRITEBIT)
        return false;
      data = data | (ADDR_IC_F << 11);
      break;
    case ID_VGS_F:
      if (RWbit == WRITEBIT)
        return false;
      data = data | (ADDR_VGS_F << 11);
      break;
    // read or write
    case ID_HG_CTRL:
      data = data | (ADDR_HG_CTRL << 11);
      break;
    case ID_LG_CTRL:
      data = data | (ADDR_LG_CTRL << 11);
      break;
    case ID_G_CTRL:
      data = data | (ADDR_G_CTRL << 11);
      break;
    case ID_IC_OP:
      data = data | (ADDR_IC_OP << 11);
      break;
    case ID_SH_AMP_CTRL:
      data = data | (ADDR_SH_AMP_CTRL << 11);
      break;
    case ID_VR_CTRL:
      data = data | (ADDR_VR_CTRL << 11);
      break;
    case ID_VDS_CTRL:
      data = data | (ADDR_VDS_CTRL << 11);
      break;
    default:
      return false;
  }
  // now data has 16 bit to trasfer to Slave

  uint16_t data_read = 0;
  switch (id) {
    // read only
    case ID_WARN:
    case ID_OV_VDS_F:
    case ID_IC_F:
    case ID_VGS_F:
      data_read = readAndWriteData(data);
      break;
    // read or write
    case ID_HG_CTRL:
    case ID_LG_CTRL:
    case ID_G_CTRL:
    case ID_IC_OP:
    case ID_SH_AMP_CTRL:
    case ID_VR_CTRL:
    case ID_VDS_CTRL:
      if (RWbit == READBIT)
        data_read = readAndWriteData(data);
      else {
        writeData(data);
        return true;
      }
      break;
    default:
      return false;
  }
  // data_read has 16 bit from Slave

  // add function to check if the communication is good
  // add function to analyze received data
  return true;
}