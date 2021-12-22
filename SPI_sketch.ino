#include "controller.hpp"

uint16_t data;
uint16_t value;
uint16_t set = 0x620; // for Gate driver control w addr = 0x7 316 for driver setting
//set = 0x620;// to disable PVDD_UVLO2 
uint8_t addr = 0x3;

/* addr
 * 0x1: watchdog
 * 0x2: OV/VDS Fault
 * 0x3: IC Fault
 * 0x4: VGS Fault 
 * 0x5: HS Gate Drive Control
 * 0x6: LS Gate Drive Contorl 
 * 0x7: Gate Drive Control 
 * 0x9: IC Operation
 * 0xA: Shunt Amplifier Control 
 * 0xB: Voltage Regulator Control 
 * 0xC: VDS Sense Control */

const int nFAULT = 7;
const int EN_GATE = 8;
const int throttle_in = A0;
const int throttle_out = 3;

int input;
int output;

void SPI_test();
void startup();
void operate();
void pwmFrequencyAdjuster();

void setup() {
  Serial.begin(9600);

  SPI_init();

  pinMode(nFAULT, INPUT);
  pinMode(EN_GATE, OUTPUT);
  pinMode(throttle_in, INPUT);
  pinMode(throttle_out, OUTPUT);

  input = 0;
  output = 0;
  data = 0;
  value = 0;

  pwmFrequencyAdjuster();

  Serial.println("start");

  data = (0x1 << 15) | (addr << 11); // only to read
  //data = (addr << 11) | set; // to write
  
  startup();

  delay(1000);
  operate();
  delay(1000);
}

void loop() {
  SPI_test();
  
  Serial.println(analogRead(throttle_in));

  
  input = analogRead(throttle_in);
  output = input / 4;
  analogWrite(throttle_out, output);
  Serial.print("Speed: ");
  output = (int)((double)output / 255. * 100.);
  Serial.println(output);

  if(digitalRead(nFAULT) == LOW)
    Serial.println("Fault!");

  Serial.println();
  Serial.println();
}


void SPI_test(){
  Serial.print("data sent: ");
  Serial.println(data,BIN);

  value = 0;
  digitalWrite(nSCS, LOW);
  value = SPI.transfer16(data);
  delayMicroseconds(MICRODELAY);
  digitalWrite(nSCS, HIGH); // disable Slave Select
  Serial.print("data received: ");
  Serial.println(value,BIN);

  delay (3000); 
}

void startup() {
  delay(300);
  while(true) {
    if(digitalRead(nFAULT) == HIGH)
      break;
  }
  Serial.println("Standby Mode");
  delay(1000);
}

void operate() {
  digitalWrite(EN_GATE, HIGH);
  Serial.print("Operation Mode");
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
