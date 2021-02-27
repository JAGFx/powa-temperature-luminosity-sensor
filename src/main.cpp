#include <Arduino.h>

#include <OneWire.h>
#include <DallasTemperature.h>
#include <Adafruit_TSL2561_U.h>
#include <ESP8266WiFi.h>

#include <Settings.h>
#include <Utils/Syncer.h>
#include <Utils/Oled.h>

// ----------------------
// ---- Modules

#define CYCLE_MAX 100
uint16_t cycleCount = CYCLE_MAX;

// -- OLED Scren
Oled oled;

// -- Luminosity sensor
Adafruit_TSL2561_Unified luxSensor  = Adafruit_TSL2561_Unified( TSL2561_ADDR_FLOAT, 12345 );
float                    currentLux = 0.0;

// -- Temperature sensor
#define TEMP_PIN D4
#define TEMPERATURE_PRECISION 10

OneWire           ow( TEMP_PIN );
DallasTemperature tempSensor( &ow );
DeviceAddress     owAddr            = { 0x28, 0xCF, 0xA5, 0x79, 0x97, 0x8, 0x3, 0xB9 };
float             currentTemp       = 0.0;

// -- Reset wh count
//#define RST_WH_BUTTON_PIN D6

// -- Led WH pulse
#define WH_PULSE_LED_PIN D7

// -- Wifi
#define WIFI_MAX_TRY 10

IPAddress ip( 192, 168, 1, 71 );
IPAddress gateway( 192, 168, 1, 1 );
IPAddress subnet( 255, 255, 255, 0 );
uint8_t   wifiCounterTry            = 0;

// -- Sync
#define SYNC_LED_PIN D8

Syncer syncer( SYNC_LED_PIN );

// ---- ./Modules
// ----------------------


// ----------------------
// ---- Common method

/**
 * Update the current luminosity form the sensor
 */
void getLuminosity() {
    // --- Lux
    digitalWrite( WH_PULSE_LED_PIN, HIGH );
    sensors_event_t event;
    luxSensor.getEvent( &event );
    currentLux = event.light;
    
    digitalWrite( WH_PULSE_LED_PIN, LOW );
}

/**
 * Update the current temperature form the sensor
 */
void getTemperature() {
    digitalWrite( WH_PULSE_LED_PIN, HIGH );
    
    tempSensor.requestTemperatures();
    currentTemp = tempSensor.getTempC( owAddr );
    
    digitalWrite( WH_PULSE_LED_PIN, LOW );
}

// ---- ./Common method
// ----------------------

// ----- Minimal functions

void setup() {
    Serial.begin( 115200 );
    delay( 1500 );
    
    // ---- LED Pulse
    pinMode( WH_PULSE_LED_PIN, OUTPUT );
    digitalWrite( WH_PULSE_LED_PIN, LOW );
    
    // ---- OLED Screen
    oled.begin();
    delay( 500 );
    
    // ---- Lux sensor
    oled.printLn( "Lux sensor init..." );
    luxSensor.begin();
    luxSensor.enableAutoRange( true );
    luxSensor.setIntegrationTime( TSL2561_INTEGRATIONTIME_402MS );
    delay( 500 );
    
    // ---- Temperature sensor
    oled.printLn( "Temp sensor init..." );
    tempSensor.begin();
    tempSensor.setResolution( owAddr, TEMPERATURE_PRECISION );
    delay( 500 );
    
    // ---- Wifi
    oled.printLn( "Wifi init..." );
    WiFi.config( ip, gateway, subnet );
    WiFi.begin( WIFI_SSID, WIFI_PSWD );
    
    while ( WiFi.status() != WL_CONNECTED ) {
        oled.print( "Wifi connecting #" );
        oled.printLn( ++wifiCounterTry, false );
        
        if ( wifiCounterTry > WIFI_MAX_TRY ) {
            wifiCounterTry = 0;
            WiFi.begin( WIFI_SSID, WIFI_PSWD );
            oled.printLn( "Wifi: Reset" );
        }
        
        delay( 500 );
    }
    
    oled.print( "Wifi: " );
    oled.printLn( WiFi.localIP(), false );
    delay( 1000 );
    
    // ---- Sync
    pinMode( SYNC_LED_PIN, OUTPUT );
    digitalWrite( SYNC_LED_PIN, LOW );
    
    syncer.connect();
}

void loop() {
    // ---- Wifi connexion
    if ( WiFi.status() != WL_CONNECTED )
        ESP.restart();
    
    if ( cycleCount++ >= CYCLE_MAX ) {
        cycleCount = 0;
        
        // ---- Lux senor
        getLuminosity();
        
        // ---- Temp sensor
        getTemperature();
    }
    
    // --- Sync
    syncer.sync();
    
    // ---- Display WH count
    oled.loop( syncer.getCycleCount(), currentLux, currentTemp );
    
    delay( 100 );
//    Serial.println( ESP.getFreeHeap() );
}
