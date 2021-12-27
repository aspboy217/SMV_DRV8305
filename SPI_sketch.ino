#include "controller.hpp"

int thr_in = 0;
int thr_out = 0;

void pinInit();
void pwmFrequencyAdjuster();

void setup() {
  Serial.begin(9600);
  Serial.println("start");

  init();

  // wait for DRV to be in standby mode
  startup();

  // set driving mode & alerts
  setMotor();

  // check all registers and store the results into struct
  // checkAll();
  
  // get into operation mode
  if(!operate())
    handleFault();
}

void loop() {
  /* throttle read & write */
  input = analogRead(THR_IN);
  // direct mapping
  output = input / 4;
  analogWrite(THR_OUT, output);
  Serial.print("Speed: ");
  Serial.println((int)((double)output / 255. * 100.));

  if(digitalRead(nFAULT) == LOW) {
    Serial.println("nFault pin LOW!");
    if(checkFault())
      checkFaultReg();
    else
      checkWarningReg();
  }

  if(!active)
    handleFault();

  Serial.println();
  Serial.println();
}

/********************************************************
 *                 FUNCTIONS DEFINITIONS                *
 ********************************************************/
void pinInit(){
  pinMode(nFAULT, INPUT);
  pinMode(EN_GATE, OUTPUT);
  pinMode(THR_IN, INPUT);
  pinMode(THR_OUT, OUTPUT);

  SPI_init();
  pwmFrequencyAdjuster();
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
