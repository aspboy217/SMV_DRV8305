#ifndef OLED_HPP
#define OLED_HPP

class Adafruit_SSD1306;

void OLEDInit(const Adafruit_SSD1306& display);

void OLED_startup(const Adafruit_SSD1306& display);

void OLED_display(const Adafruit_SSD1306& display, byte pwm, bool status, double current,
    uint16_t warn, uint16_t ovs, uint16_t ic, uint16_t vgs);

#endif