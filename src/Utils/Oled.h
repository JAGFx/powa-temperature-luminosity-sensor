//
// Created by CLion.
// @author: SMITH Emmanuel <hey@emmanuel-smith.me>
// @site: https://emmanuel-smith.me
// Date: 14/02/2021
// Time: 11:31
//

#ifndef CONSUMPTIONSNIFFER_OLED_H
#define CONSUMPTIONSNIFFER_OLED_H

#include <Arduino.h>
//#include <Wire.h>
//#include <Adafruit_GFX.h>
#include <Adafruit_I2CDevice.h>
#include <Adafruit_SSD1306.h>

class Oled {
private:
    Adafruit_SSD1306 *_screen;
    boolean          _side       = true;
    uint16_t         _cycleCount = CYCLE_MAX;

public:
    static constexpr const char *UNIT_LUX    = " lux";
    static constexpr const char *UNIT_DEGREE = " °C";
    static const uint16_t       CYCLE_MAX    = 20;
    
    // --
    
    explicit Oled();
    
    // --
    void begin();
    
    void loop( uint16_t syncerCount, float lux, float temp );
    
    void print( const char *str, bool clear = true );
    
    void printLn( const char *str, bool clear = true );
    
    void printLn( const Printable &x, bool clear = true );
    
    void printLn( unsigned char b, bool clear = true );
};


#endif //CONSUMPTIONSNIFFER_OLED_H
