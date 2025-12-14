#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <WiFiManager.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <Preferences.h>
#include <TFT_eSPI.h>
#include <ezTime.h>
#include <vector>
#include <string>
#include <cstdio>

// extern variables
extern Preferences preferences; 
extern TFT_eSPI tft;
extern Timezone myTZ;

// button
#define PORTAL_TIMEOUT 120 // 2 minutes wait in wifi config mode
#define CONFIG_PRESS_TIME 3000
#define BUTTON_SHORT_PRESS_TIME 50

#define BUTTON_COIN_PIN 1
#define BUTTON_CONFIG_PIN 2

//buzzer
#define BUZZER_PIN 0

//coin cycling
#define MAX_COINS 3

#endif