#ifndef GLOBAL_HPP
#define GLOBAL_HPP

#define MIN_THR   0

/* PIN DEFINITIONS */
#define  THR_IN   A0
#define  SO1      A1
#define  SO2      A2
#define  SO3      A3
#define  SW       A6 // Switch
#define  CURR_A   A7

#define OPERATE   2 // blue LED
#define THR_OUT   3
#define STANDBY   4 // green LED
#define STARTUP   5 // red LED
#define WAKE      6
#define nFAULT    7
#define EN_GATE   8
#define PWRGD     9
#define nSCS      10

/* CURRENT SENSE VALUES */
#define CURR_A_WARN 15
#define CURR_A_SHUTDOWN 18

/* REGISTER DATA */
uint16_t warning = 0; // 0x1
uint16_t ovs_flt = 0; // 0x2
uint16_t ic_flt  = 0; // 0x3
uint16_t vgs_flt = 0; // 0x4

/* SPI ADDR */
// STATUS REGS (R ONLY)
#define WARN           0x1 // Warning
#define FLT_OC         0x2 // Overcurrent faults
#define FLT_IC         0x3 // IC Faults
#define FLT_VGS        0x4 // Gate Driver VGS Faults
// CONTROL REGS (R & W)
#define CNTL_HS        0x5 // HS Gate Drv Control
#define CNTL_LS        0x6 // LS Gate Drv Control
#define CNTL_DRV       0x7 // Gate Drv Control - modes
#define CNTL_IC        0x9 // IC Operation - Fault & Watchdog control
#define CNTL_SHAMP     0xA // Shunt Amp Control
#define CNTL_VREG      0xB // Voltage Regulator Control
#define CNTL_VDS       0xC // VDS Sense Control

/* REGISTER DATA */
// 0x5 HS Gate Control 
#define TDRIVEN        0x3 // 220ns
#define IDRIVEN_HS     0x4 // 1.25A
#define IDRIVEP_HS     0x4 // 1.00A
// 0x6 LS Gate Control
#define TDRIVEP        0x3 // 220ns
#define IDRIVEN_LS     0x4 // 1.25A
#define IDRIVEP_LS     0x4 // 1.00A
// 0x7 Gate Drive Control
#define COMM_OPTION    0x1 // control: active freewheeling
#define PWM_MODE       0x2 // PWM mode: 1-PWM
#define DEAD_TIME      0x0 // Dead time: 35ns
#define TBLANK         0x3 // VDS sense blanking: 7us
#define TVDS           0x2 // VDS sense deglitch: 3.5us
// 0x9 IC Operation
#define FLIP_OTSD      0x1 // Enable
#define DIS_PVDD_UVLO2 0x0 // Disable
#define DIS_GDRV_FAULT 0x0 // Enable
#define EN_SNS_CLAMP   0x0 // Disable
#define WD_DLY         0x1 // 20ms
#define DIS_SNS_OCP    0x0 // Enable
#define SET_VCPH_UV    0x0 // 4.9V
#define WD_EN          0x8
#define SLEEP_BIT      0x4
#define CLR_FLTS       0x2
// 0xA Shunt Amplifier Control
#define DC_CAL_CH3     0x1 // Calib Operation
#define DC_CAL_CH2     0x1 // Calib Operation
#define DC_CAL_CH1     0x1 // Calib Operation
#define CS_BLANK       0x0 // 0ns
#define GAIN_CS3       0x2 // 40V/V
#define GAIN_CS2       0x2 // 40V/V
#define GAIN_CS1       0x2 // 40V/V
// 0xB Voltage Regulator Control
#define VREF_SCALE     0x1 // k = 2
#define SLEEP_DLY      0x1 // 10us
#define DIS_VREG_PWRGD 0x0 // enable
#define VREG_UV_LEVEL  0x2 // VREG * 0.7
// 0xC VDS Sense Control
#define VDS_LEVEL      0x11// 0.454V
#define VDS_MODE       0x1 // Report only

/* REGISTER DATA */
const uint16_t CNTL_HS_DATA    = (TDRIVEN<<8)|(IDRIVEN_HS<<4)|IDRIVEP_HS;
const uint16_t CNTL_LS_DATA    = (TDRIVEP<<8)|(IDRIVEN_LS<<4)|IDRIVEP_LS;
const uint16_t CNTL_DRV_DATA   = (COMM_OPTION<<9)|(PWM_MODE<<7)|(DEAD_TIME<<4)|(TBLANK<<2)|TVDS;
const uint16_t CNTL_IC_DATA    = (FLIP_OTSD<<10)|(DIS_PVDD_UVLO2<<9)|(DIS_GDRV_FAULT<<8)|(EN_SNS_CLAMP<<7)|(WD_DLY<<5)|(DIS_SNS_OCP<<4)|SET_VCPH_UV;
const uint16_t CNTL_SHAMP_DATA = (DC_CAL_CH3<<10)|(DC_CAL_CH2<<9)|(DC_CAL_CH1<<8)|(CS_BLANK<<6)|(GAIN_CS3<<4)|(GAIN_CS2<<2)|GAIN_CS1;
const uint16_t CNTL_VREG_DATA  = (VREF_SCALE<<8)|(SLEEP_DLY<<3)|(DIS_VREG_PWRGD<<2)|VREG_UV_LEVEL;
const uint16_t CNTL_VDS_DATA   = (VDS_LEVEL<<3)|VDS_MODE;

#endif
