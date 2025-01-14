#include <WiFi.h>
#include "ThingSpeak.h"
#include <Wire.h>
#include "DHT.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <PZEM004Tv30.h>

// OLED display dimensions and I2C address
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define SSD1306_I2C_ADDRESS 0x3c

#define DHTPIN 26    
#define DHTTYPE DHT11 
#define BUTTON_PIN_RESET 33
#define BUTTON_PIN_SWITCH 32

const int relay1Pin = 19; // Pin for Relay 1
const int relay2Pin = 18; // Pin for Relay 2

// WiFi credentials
const char* WIFI_NAME = "LAP123";  
const char* WIFI_PASSWORD = "nikhil90409";         

// ThingSpeak configuration
const int myChannelId = 2704346;         
const char* myWriteApiKey = "EQ1R2ZC2JNRFY9CA"; 
const int myChannelId1 = 2741278;         
const char* myWriteApiKey1 = "AK3VR6HR85R0PWQD"; 
const char* server = "api.thingspeak.com";

// ThingSpeak field numbers
int FIELD_TEMP = 2;
int FIELD_HUMIDITY = 1;
int FIELD_ERROR_CODE = 7;
int FIELD_CURRENT = 6;
int FIELD_POWER = 4;
int FIELD_VOLTAGE = 5;
//int FIELD_FREQUENCY = 9;
int FIELD_ENERGY = 3;
int FIELD_SWITCH = 8;

// Threshold values
const float MAX_VOLTAGE = 260.0f;
const float MIN_VOLTAGE = 180.0f;
const float MAX_CURRENT = 5.0f;

float voltage, current, power, energy_kWh, frequency;
int errorCode = 0;
int lastErrorCode = 0;
int switchValue = 3;
unsigned long lastUpdateMillis = 0;
const unsigned long updateInterval = 30000;  // Update every 30 seconds
unsigned long lastSwitchInterval = 0;
const unsigned long switchInterval = 20000;
unsigned long lastUpdateOled = 0;
const unsigned long updateOledInterval = 1500; 

float tempVoltage ,tempCurrent , tempPower , tempEnergy_kWh , tempFrequency;

String statusText;