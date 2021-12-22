#ifndef SPI_COMM_H
#define SPI_COMM_H

#include <SPI.h>
#include "GLOBAL.h"

/*  DRIVER REQUIREMENTS (p 13, 34)
    - default mode              : 1 (SPOL = 0, SPHA = 1 - trailing edge)
    - Minimum SPI clock period  : 100ns ~
    - CLK low and high time     : 40ns ~
    - SDO output data delay time: ~ 20ns
    - word size                 : 16 bits = 2 bytes
    - SDO (MISO): MSB..5 bits don't care + 11 bits data..LSB
     _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
    |_ _ _ _ _ _ _ _ _ _ _ _ _ _ DATA_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _|
    |B15|B14|B13|B12|B11|B10|B9 |B8 |B7 |B6 |B5 |B4 |B3 |B2 |B1 |B0 |
    | X | X | X | X | X |D10|D9 |D8 |D7 |D6 |D5 |D4 |D3 |D2 |D1 |D0 |
    |_ _|_ _|_ _|_ _|_ _|_ _|_ _|_ _|_ _|_ _|_ _|_ _|_ _|_ _|_ _|_ _|

    - SDI (MOSI): MSB..5 bits commands + 11 bits data..LSB
     _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
    |R/W|_ _ ADDRESS _ _|_ _ _ _ _ _ _ _ DATA_ _ _ _ _ _ _ _ _ _ _ _|
    |B15|B14|B13|B12|B11|B10|B9 |B8 |B7 |B6 |B5 |B4 |B3 |B2 |B1 |B0 |
    |W0 |A3 |A2 |A1 |A0 |D10|D9 |D8 |D7 |D6 |D5 |D4 |D3 |D2 |D1 |D0 |
    |_ _|_ _|_ _|_ _|_ _|_ _|_ _|_ _|_ _|_ _|_ _|_ _|_ _|_ _|_ _|_ _|
    W0 = 0 : write the data into registers
        return data: data in the register BEFORE being written
    W0 = 1 : read the data from registers
        return data: data in the register
*/

void SPI_init();

void writeData(uint16_t val);

uint16_t readAndWriteData(uint16_t val);

/*
  INPUT: id, data (11 bit of data), RWbit (1 to read, 0 to write)
  OUTPUT: true if communication is successful, else, false
  DESC: Communicate with Slave (unsigned int: 16 bit)
*/
// communicate with slave. if id can write, data is used for 10 bit data
// return false if communication is bad
bool SPI_comm(ID id, uint16_t data, bool RWbit);
#endif
