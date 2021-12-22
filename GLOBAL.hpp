#ifndef GLOBAL_H
#define GLOBAL_H

// clock pin: D13
// PIN DEFS
#define nSCS                10 // enable SPI (active low) nSCS


// SPI Setting
#define SPISPD            SPI_CLOCK_DIV4  // Set SPI Clock Speed
#define BITORDER          MSBFIRST        // Set which bit to transfer first
#define SPIMODE           SPI_MODE1       // Set SPI Mode
#define MICRODELAY        20              // Set delay amount to receive data from Slave (microsec)
#define TRANSFERDEALAY    1000            // Set delay amount to transfer data (millisec)- > 500ns

// Constants
#define READBIT           1
#define WRITEBIT          0
#define BIT0              1
#define BIT1              1 << 1
#define BIT2              1 << 2
#define BIT3              1 << 3
#define BIT4              1 << 4
#define BIT5              1 << 5
#define BIT6              1 << 6
#define BIT7              1 << 7
#define BIT8              1 << 8
#define BIT9              1 << 9
#define BIT10             1 << 10

// REG ID
enum ID{
  ID_WARN,
  ID_OV_VDS_F,
  ID_IC_F,
  ID_VGS_F,
  ID_HG_CTRL,
  ID_LG_CTRL,
  ID_G_CTRL,
  ID_IC_OP,
  ID_SH_AMP_CTRL,
  ID_VR_CTRL,
  ID_VDS_CTRL
};

// REG ADDRESS
#define ADDR_WARN         0x1
#define ADDR_OV_VDS_F     0x2
#define ADDR_IC_F         0x3
#define ADDR_VGS_F        0x4
#define ADDR_HG_CTRL      0x5
#define ADDR_LG_CTRL      0x6
#define ADDR_G_CTRL       0x7
#define ADDR_IC_OP        0x9
#define ADDR_SH_AMP_CTRL  0xA
#define ADDR_VR_CTRL      0xB
#define ADDR_VDS_CTRL     0xC

#endif
