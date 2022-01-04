#ifndef GLOBAL_HPP
#define GLOBAL_HPP

/* PIN DEFINITIONS */
const int  THR_IN  = A0;
const int  SO1     = A1;
const int  SO2     = A2;
const int  SO3     = A3;
const int  SDA     = A4;
const int  SCL     = A5;
const int  SW      = A6; // Switch
const int  CURR_A  = A7;

const byte OPERATE = 2; // blue LED
const byte THR_OUT = 3;
const byte STANDBY = 4; // green LED
const byte STARTUP = 5; // red LED
const byte WAKE    = 6;
const byte nFAULT  = 7;
const byte EN_GATE = 8;
const byte PWRGD   = 9;
const byte nSCS    = 10;

/* SPI ADDR */
// STATUS REGS (R ONLY)
const byte WARN           = 0x1; // Warning
const byte FLT_OC         = 0x2; // Overcurrent faults
const byte FLT_IC         = 0x3; // IC Faults
const byte FLT_VGS        = 0x4; // Gate Driver VGS Faults
// CONTROL REGS (R & W)
const byte CNTL_HS        = 0x5; // HS Gate Drv Control
const byte CNTL_LS        = 0x6; // LS Gate Drv Control
const byte CNTL_DRV       = 0x7; // Gate Drv Control - modes
const byte CNTL_IC        = 0x9; // IC Operation - Fault & Watchdog control
const byte CNTL_SHAMP     = 0xA; // Shunt Amp Control
const byte CNTL_VREG      = 0xB; // Voltage Regulator Control
const byte CNTL_VDS       = 0xC; // VDS Sense Control

/* REGISTER DATA */
// 0x5 HS Gate Control 
const byte TDRIVEN        = 0x0; // 220ns
const byte IDRIVEN_HS     = 0xB; // 1.25A
const byte IDRIVEP_HS     = 0xB; // 1.00A
// 0x6 LS Gate Control
const byte TDRIVEP        = 0x0; // 220ns
const byte IDRIVEN_LS     = 0xB; // 1.25A
const byte IDRIVEP_LS     = 0xB; // 1.00A
// 0x7 Gate Drive Control
const byte COMM_OPTION    = 0x1; // control: active freewheeling
const byte PWM_MODE       = 0x2; // PWM mode: 1-PWM
const byte DEAD_TIME      = 0x0; // Dead time: 35ns
const byte TBLANK         = 0x3; // VDS sense blanking: 7us
const byte TVDS           = 0x2; // VDS sense deglitch: 3.5us
// 0x9 IC Operation
const byte FLIP_OTSD      = 0x1; // Enable
const byte DIS_PVDD_UVLO2 = 0x1; // Disable
const byte DIS_GDRV_FAULT = 0x0; // Enable
const byte EN_SNS_CLAMP   = 0x0; // Disable
const byte WD_DLY         = 0x1; // 20ms
const byte DIS_SNS_OCP    = 0x0; // Enable
const byte SET_VCPH_UV    = 0x0; // 4.9V
const byte WD_EN          = 0x8;
const byte SLEEP_BIT      = 0x4;
const byte CLR_FLTS       = 0x2;
// 0xA Shunt Amplifier Control
const byte DC_CAL_CH3     = 0x1; // Calib Operation
const byte DC_CAL_CH2     = 0x1; // Calib Operation
const byte DC_CAL_CH1     = 0x1; // Calib Operation
const byte CS_BLANK       = 0x0; // 0ns
const byte GAIN_CS3       = 0x2; // 40V/V
const byte GAIN_CS2       = 0x2; // 40V/V
const byte GAIN_CS1       = 0x2; // 40V/V
// 0xB Voltage Regulator Control
const byte VREF_SCALE     = 0x1; // k = 2
const byte SLEEP_DLY      = 0x1; // 10us
const byte DIS_VREG_PWRGD = 0x0; // enable
const byte VREG_UV_LEVEL  = 0x2; // VREG * 0.7
// 0xC VDS Sense Control
const byte VDS_LEVEL      = 0x11;// 0.454V
const byte VDS_MODE       = 0x1; // Report only

/* REGISTER DATA */
const uint16_t CNTL_HS_DATA    = (TDRIVEN<<8)|(IDRIVEN_HS<<4)|IDRIVEP_HS;
const uint16_t CNTL_LS_DATA    = (TDRIVEP<<8)|(IDRIVEN_LS<<4)|IDRIVEP_LS;
const uint16_t CNTL_DRV_DATA   = (COMM_OPTION<<9)|(PWM_MODE<<7)|(DEAD_TIME<<4)|(TBLANK<<2)|TVDS;
const uint16_t CNTL_IC_DATA    = (FLIP_OTSD<<10)|(DIS_PVDD_UVLO2<<9)|(DIS_GDRV_FAULT<<8)|(EN_SNS_CLAMP<<7)|(WD_DLY<<5)|(DIS_SNS_OCP<<4)|SET_VCPH_UV;
const uint16_t CNTL_SHAMP_DATA = (DC_CAL_CH3<<10)|(DC_CAL_CH2<<9)|(DC_CAL_CH1<<8)|(CS_BLANK<<6)|(GAIN_CS3<<4)|(GAIN_CS2<<2)|GAIN_CS1;
const uint16_t CNTL_VREG_DATA  = (VREF_SCALE<<8)|(SLEEP_DLY<<3)|(DIS_VREG_PWRGD<<2)|VREG_UV_LEVEL;
const uint16_t CNTL_VDS_DATA   = (VDS_LEVEL<<3)|VDS_MODE;

#endif
