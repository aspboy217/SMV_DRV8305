#ifndef GLOBAL_HPP
#define GLOBAL_HPP

/* PIN DEFINITIONS */
#define nSCS    10 
#define nFAULT  7
#define EN_GATE 8
#define THR_IN  A0
#define THR_OUT 3

bool active;
// true  - operating state
// false - standby, sleep state

/* SPI ADDR */
// STATUS REGS (R ONLY)
#define WARN            0x1 // Warning
#define FLT_OC          0x2 // Overcurrent faults
#define FLT_IC          0x3 // IC Faults
#define FLT_VGS         0x4 // Gate Driver VGS Faults
// CONTROL REGS (R & W)
#define CNTL_HS         0x5 // HS Gate Drv Control
#define CNTL_LS         0x6 // LS Gate Drv Control
#define CNTL_DRV        0x7 // Gate Drv Control - modes
#define CNTL_IC         0x9 // IC Operation - Fault & Watchdog control
#define CNTL_SHAMP      0xA // Shunt Amp Control
#define CNTL_VREG       0xB // Voltage Regulator Control
#define CNTL_VDS        0xC // VDS Sense Control

/* REGISTER DATA */
// 0x5 HS Gate Control 
#define TDRIVEN         0X3 // 1780ns
#define IDRIVEN_HS      0X4 // 60mA
#define IDRIVEP_HS      0x4 // 50mA
// 0x6 LS Gate Control
#define TDRIVEP         0x3 // 1780ns
#define IDRIVEN_LS      0X4 // 60mA
#define IDRIVEP_LS      0X4 // 50mA
// 0x7 Gate Drive Control
#define COMM_OPTION     0x1 // control: active freewheeling
#define PWM_MODE        0x2 // PWM mode: 1-PWM
#define DEAD_TIME       0x1 // Dead time: 52ns
#define TBLANK          0x1 // VDS sense blanking: 1.75us
#define TVDS            0x2 // VDS sense deglitch: 3.5us
// 0x9 IC Operation
#define FLIP_OTSD       0x1 // Enable
#define DIS_PVDD_UVLO2  0x0 // Enable
#define DIS_GDRV_FAULT  0x0 // Enable
#define EN_SNS_CLAMP    0x0 // Disable
#define WD_DLY          0x1 // 20ms
#define DIS_SNS_OCP     0x0 // Disable
#define WD_EN           0x0 // Disable
#define SET_VCPH_UV     0x0 // 4.9V
#define SLEEP_BIT       (0x1<<2)
#define CLR_FLTS        (0x1<<1)
// 0xA Shunt Amplifier Control
#define DC_CAL_CH3      0x0 // Normal Operation
#define DC_CAL_CH2      0x0 // Normal Operation
#define DC_CAL_CH1      0x0 // Normal Operation
#define CS_BLANK        0x0 // 0ns
#define GAIN_CS3        0x0 // 10V/V
#define GAIN_CS2        0x0 // 10V/V
#define GAIN_CS1        0x0 // 10V/V
// 0xB Voltage Regulator Control
#define VREF_SCALE      0x1 // k = 2
#define SLEEP_DLY       0x1 // 10us
#define DIS_VREG_PWRGD  0x0 // enable
#define VREG_UV_LEVEL   0x2 // VREG * 0.7
// 0xC VDS Sense Control
#define VDS_LEVEL       0x19 // 1.175V
#define VDS_MODE        0x0 // Latched shutdown when over-current detected

/* REGISTER DATA */
#define CNTL_HS_DATA    ((TDRIVEN<<8)|(IDRIVEN_HS<<4)|IDRIVEP_HS)
#define CNTL_LS_DATA    ((TDRIVEP<<8)|(IDRIVEN_LS<<4)|IDRIVEP_LS)
#define CNTL_DRV_DATA   ((COMM_OPTION<<9)|(PWM_MODE<<7)|(DEAD_TIME<<4)|(TBLANK<<2)|TVDS)
#define CNTL_IC_DATA    ((FLIP_OTSD<<10)|(DIS_PVDD_UVLO2<<9)|(DIS_GDRV_FAULT<<8)|(EN_SNS_CLAMP<<7)|(WD_DLY<<5)|(DIS_SNS_OCP<<4)|(WD_EN<<3)|SET_VCPH_UV)
#define CNTL_SHAMP_DATA ((DC_CAL_CH3<<10)|(DC_CAL_CH2<<9)|(DC_CAL_CH1<<8)|(CS_BLANK<<6)|(GAIN_CS3<<4)|(GAIN_CS2<<2)|GAIN_CS1)
#define CNTL_VREG_DATA  ((VREF_SCALE<<8)|(SLEEP_DLY<<3)|(DIS_VREG_PWRGD<<2)|VREG_UV_LEVEL)
#define CNTL_VDS_DATA   ((VDS_LEVEL<<3)|VDS_MODE)

#endif