#include "global.hpp"
#include "controller.hpp"
#include "OLED.hpp"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

/* Variables */
byte pwm;
bool status;
double current;
/* Objects */
Controller controller;
Adafruit_SSD1306 display = Adafruit_SSD1306(128, 64, &Wire, -1);

/* Function Declarations */
void pinInit();
void pwmFrequencyAdjuster();
byte writeSpeed();
double checkCurrent();

void setup() {
  pinInit();
  OLEDInit(display);

  OLED_startup(display);
  controller.startup();
}

void loop() {
  /* output pwm % to drv & return pwm value */
  pwm = writeSpeed();

  /* read current sense amp */
  current = checkCurrent();

  /* check for warning or faults & sets global vars */
  controller.checkFaultPin();

  status = controller.isActive();
  /* if motor is in standby mode */
  if(!status) {
    /* if switch is pressed -> operation */
    if(analogRead(SW) < 300){
      if(!controller.operate())
        controller.handleFault();
      while(analogRead(SW) < 300) {} // to give 10ms time to charge gate pumps
    }
    else /* if not pressed -> keep refresh faults */
      controller.handleFault();
  }
  /* if motor is in operation mode */
  else{
    /* if switch is pressed & pwm is low -> standby */
    if(analogRead(SW) < 300 && pwm < 10)
      controller.standby();
    while(analogRead(SW) < 300) {} // to prevent changing back to operation mode
  }
  
  /* OLED update */
  OLED_display(display, pwm, status, current, warning, ovs_flt, ic_flt, vgs_flt);
}

/********************************************************
 *                 FUNCTIONS DEFINITIONS                *
 ********************************************************/
void pinInit(){
  pinMode(THR_IN , INPUT);
  pinMode(SO1    , INPUT);
  pinMode(SO2    , INPUT);
  pinMode(SO3    , INPUT);
  pinMode(SW     , INPUT); // Switch
  pinMode(CURR_A , INPUT);

  pinMode(OPERATE, OUTPUT); // blue LED
  pinMode(STANDBY, OUTPUT); // green LED
  pinMode(STARTUP, OUTPUT); // red LED
  pinMode(WAKE   , OUTPUT);
  pinMode(PWRGD  , INPUT);
  pinMode(THR_OUT, OUTPUT);
  pinMode(nFAULT , INPUT);
  pinMode(EN_GATE, OUTPUT);

  SPI_init();
  pwmFrequencyAdjuster();

  pwm = 0;
  status = false;
  current = 0;
}

void pwmFrequencyAdjuster() {
  // The value of each register determines the frequency for both its associated pins
  // TCCR0B : Pins 5,6
  // TCCR1B : Pins 9,10
  // TCCR2B : Pins 3,11
  // If TCCRnB = xxxxx001, then frequency is 64    kHz for n=0, and 32    kHz for n=1,2
  // If TCCRnB = xxxxx010, then frequency is 8     kHz for n=0, and 4     kHz for n=1,2
  // If TCCRnB = xxxxx011, then frequency is 1     kHz for n=0, and 500   Hz  for n=1,2
  // If TCCRnB = xxxxx100, then frequency is 250   Hz  for n=0, and 125   Hz  for n=1,2
  // If TCCRnB = xxxxx101, then frequency is 61.25 Hz  for n=0, and 31.25 Hz  for n=1,2
  int prescalerVal = 7;  // Binary representation 0b00000111
  int prescalerVal2 = 1; // Binary representation 0b00000001
  TCCR0B &= ~prescalerVal; // Bitwise-and with 0b11111000, essentially discarding the 3 LSB
  TCCR1B &= ~prescalerVal;
  TCCR2B &= ~prescalerVal;
  TCCR0B |= prescalerVal2; // Bitwise-or with desired pwm frequency to set 3 LSB, in this case 001 for fastest frequency
  TCCR1B |= prescalerVal2;
  TCCR2B |= prescalerVal2;
  // More information at http://usethearduino.blogspot.com/2008/11/changing-pwm-frequency-on-arduino.html
}

byte writeSpeed() {
  byte output = analogRead(THR_IN)/4;
  if(output >= MIN_THR)
    analogWrite(THR_OUT, output);
  return (byte)((double)output / 255. * 100.);
}

double checkCurrent() {
  double current = (double)analogRead(CURR_A)/51.15;
  if(current >= CURR_A_SHUTDOWN){
    controller.standby();
  }
  return current;
}
