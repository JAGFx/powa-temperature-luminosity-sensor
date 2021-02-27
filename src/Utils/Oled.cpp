//
// Created by CLion.
// @author: SMITH Emmanuel <hey@emmanuel-smith.me>
// @site: https://emmanuel-smith.me
// Date: 14/02/2021
// Time: 11:31
//

#include "Oled.h"

// --

Oled::Oled() {
    _screen = new Adafruit_SSD1306( SSD1306_LCDWIDTH, SSD1306_LCDHEIGHT, &Wire );
}

// --

void Oled::begin() {
    _screen->begin( SSD1306_SWITCHCAPVCC, 0x3C ); // Address 0x3C for 128x32
    _screen->setTextSize( 1 );
    _screen->setTextColor( SSD1306_WHITE );
    _screen->clearDisplay();
    _screen->println( "Init..." );
}

void Oled::loop( uint16_t syncerCount, float lux, float temp ) {
    _screen->clearDisplay();
    _screen->setCursor( 0, 0 );
    _screen->setTextSize( 3 );
    
    if ( _side ) {
        _screen->print( lux, 0 );
        _screen->setTextSize( 2 );
        _screen->println( UNIT_LUX );
        
    } else {
        _screen->print( temp, 1 );
        _screen->setTextSize( 2 );
        _screen->println( UNIT_DEGREE );
    }
    
    _screen->setCursor( 0, 25 );
    _screen->setTextSize( 1 );
    _screen->print( "Sync:" );
    _screen->print( syncerCount );
    _screen->display();
    
    if ( _cycleCount++ >= CYCLE_MAX ) {
        _side       = !_side;
        _cycleCount = 0;
    }
}

/**
 * Method to whReset oled screen and display a new text
 *
 * @param str
 * @param clear
 */
void Oled::print( const char *str, bool clear ) {
    if ( clear ) {
        _screen->clearDisplay();
        _screen->setCursor( 0, 0 );
    }
    
    _screen->print( str );
    _screen->display();
}

/**
 * Method to whReset oled screen and display a new text
 *
 * @param str
 * @param clear
 */
void Oled::printLn( const char *str, bool clear ) {
    if ( clear ) {
        _screen->clearDisplay();
        _screen->setCursor( 0, 0 );
    }
    
    _screen->println( str );
    _screen->display();
}

/**
 * Method to whReset oled screen and display a new text
 *
 * @param x
 * @param clear
 */
void Oled::printLn( const Printable &x, bool clear ) {
    if ( clear ) {
        _screen->clearDisplay();
        _screen->setCursor( 0, 0 );
    }
    
    _screen->println( x );
    _screen->display();
}

/**
 * Method to whReset oled screen and display a new text
 *
 * @param b
 * @param clear
 */
void Oled::printLn( unsigned char b, bool clear ) {
    if ( clear ) {
        _screen->clearDisplay();
        _screen->setCursor( 0, 0 );
    }
    
    _screen->println( b );
    _screen->display();
}
