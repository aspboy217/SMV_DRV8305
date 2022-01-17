#include "OLED.hpp"

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

void displayInit(const Adafruit_SSD1306& display) {
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setRotation(2);
}

void OLED_startup(const Adafruit_SSD1306& display) {
  display.clearDisplay();
  display.setCursor(0,0);
  display.print("Starting up...");

  display.setCursor(13, 50);
  display.print("SMV-EV Powertrain");
  
  display.display();
}

void OLED_display(const Adafruit_SSD1306& display, byte pwm, bool status, double current,
    uint16_t warn, uint16_t ovs, uint16_t ic, uint16_t vgs) {
  display.clearDisplay();

  // line 1
  display.setCursor(0, 0);
  display.print("Duty %:");
  display.setCursor(50, 0);
  display.print(pwm);

  // line 2
  display.setCursor(0, 10);
  display.print("Status:");
  display.setCursor(50, 10);
  if(status)
    display.print("Operation");
  else
    display.print("Standby");
  
  // line 3
  display.setCursor(0, 20);
  display.print("Current:     A");
  display.setCursor(50, 20);
  display.print(current);
  display.setCursor(90, 20);
  if(current >= CURR_A_SHUTDOWN)
    display.print("> 18 A");
  else if(current >= CURR_A_WARN)
    display.print("> 15 A");

  // line 4
  display.setCursor(0, 30);
  display.print("Warn: ");
  display.setCursor(42, 30);
  display.print(warn);

  // line 5
  display.setCursor(0, 40);
  display.print("Fault: 2-     3-");
  display.setCursor(55, 40);
  display.print(ovs);
  display.setCursor(97, 40);
  display.print(ic);
  
  // line 6
  display.setCursor(42, 50);
  display.print("4-");
  display.setCursor(55, 50);
  display.print(vgs);

  display.display();
}