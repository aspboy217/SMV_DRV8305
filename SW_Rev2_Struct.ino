#include "global.hpp"
#include "controller.hpp"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

void pinInit();
void pwmFrequencyAdjuster();
void displayInit();
void displayOLED(int top, bool status);

int thr_in = 0;
int thr_out = 0;
int output;
int pwm;
bool status;
Controller controller;
Adafruit_SSD1306 display = Adafruit_SSD1306(128, 64, &Wire, -1);

void setup() {
  Serial.begin(115200);
  Serial.println("start");

  pinInit();
  displayInit();

  controller.startup();

  output = 0;
  pwm = 0;
  status = false;
}

void loop() {
  output = analogRead(THR_IN) / 4;
  analogWrite(THR_OUT, output);
  pwm = (int)((double)output / 255. * 100.);
  
  controller.checkFaultPin();

  status = controller.isActive();
  displayOLED(pwm, status);
  if(!status) {
    if(analogRead(SW) < 300){ // need to change this pin to interrupt
      if(!controller.operate())
        controller.handleFault();
    }
    else
      controller.handleFault();
  }
  Serial.println();
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

void displayInit() {
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setRotation(2);
}

void displayOLED(int top, bool status) {
  display.clearDisplay();
  display.setCursor(0, 0);
  display.print("Duty %: ");
  display.setCursor(40, 0);
  display.print(top);

  display.setCursor(0, 15);
  display.print("Status: ");
  display.setCursor(40, 15);
  if(status)
    display.print("operation");
  else
    display.print("standby");
  
  display.display();
}