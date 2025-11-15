#include "utils.h"
#include <WiFi.h>
#include <cstdio>
#include <WiFiManager.h>
#include "config.h"

std::string format_price(float number) {
    if (number == 0) {
        return "0";
    }

    // Use snprintf for float formatting with thousands separator
    char buffer[32];
    
    // Format based on number size
    if (number >= 1000) {
        // Large numbers: format with commas
        int whole = (int)number;
        std::string result = "";
        int count = 0;
        
        while (whole > 0) {
            if (count > 0 && count % 3 == 0) {
                result += ' ';
            }
            result += (char)((whole % 10) + '0');
            whole /= 10;
            count++;
        }
        std::reverse(result.begin(), result.end());
        
        // Add decimal part -- not adding decimal part for big numbers
        // int decimals = (int)((number - (int)number) * 100);
        // if (decimals > 0) {
        //     snprintf(buffer, sizeof(buffer), ".%02d", decimals);
        //     result += buffer;
        // }
        return result;
        
    } else if (number >= 1) {
        // Numbers 1-999: show 2 decimals
        snprintf(buffer, sizeof(buffer), "%.2f", number);
        return std::string(buffer);
        
    } else if (number >= 0.01) {
        // Numbers 0.01-0.99: show 4 decimals
        snprintf(buffer, sizeof(buffer), "%.4f", number);
        return std::string(buffer);
        
    } else {
        // Very small numbers: show 8 decimals
        snprintf(buffer, sizeof(buffer), "%.8f", number);
        return std::string(buffer);
    }
}


std::string format_price_change(float value, int precision) {
    char buffer[16]; 

    char format_string[8]; 
    snprintf(format_string, sizeof(format_string), "%%.%df", precision);
    
    int length = snprintf(
        buffer, 
        sizeof(buffer), 
        format_string, 
        value
    );

    // Error check (optional, but good practice)
    if (length < 0 || length >= sizeof(buffer)) {
        return "ERR"; 
    }

    // Convert the C-style array to a C++ string
    return std::string(buffer);
}


void setup_wifi(const char* ssid, const char* password) {
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

bool setup_wifi_manager() {
    WiFiManager wm;

    // wm.resetSettings(); //reset saved settings for testing

    wm.setConfigPortalTimeout(PORTAL_TIMEOUT);
    wm.setClass("invert"); //dark mode

    //custom params for coin selection
    WiFiManagerParameter custom_coin("coin", "Coin name (bitcoin, ethereum, doge, ...)", "bitcoin", 50);
    wm.addParameter(&custom_coin);

    Serial.println("Starting WifiManager...");

    WiFi.mode(WIFI_STA);

    //try to connect
    bool success = wm.autoConnect("crypto_display", "crypto123");

    //if connection fails
    if (!success) {
        Serial.println("Failed to connect to wifi from wifimanager");
        return false;
    }

    COIN_NAME = String(custom_coin.getValue());

    Serial.println("Wifi connected");
    return true;
    
}

//check if button is hold for 3 seconds
bool check_config_button() {
    static unsigned long button_press_start = 0;
    static bool button_was_pressed = false;
    
    bool button_pressed = (digitalRead(BUTTON_PIN) == LOW);
    
    if (button_pressed && !button_was_pressed) {
        // Button just pressed
        button_press_start = millis();
        button_was_pressed = true;
    } 
    else if (!button_pressed && button_was_pressed) {
        // Button released
        button_was_pressed = false;
        button_press_start = 0;
    }
    else if (button_pressed && button_was_pressed) {
        // Button is being held
        unsigned long hold_time = millis() - button_press_start;
        if (hold_time >= CONFIG_PRESS_TIME) {
            button_was_pressed = false;
            button_press_start = 0;
            return true;  // Button held long enough
        }
    }
    
    return false;
}

// Start configuration portal on demand
bool start_config_portal_on_demand() {
    Serial.println("Starting configuration portal on demand...");
    
    WiFiManager wm;
    
    wm.setConfigPortalTimeout(PORTAL_TIMEOUT);
    wm.setClass("invert");
    
    // Custom parameter for coin selection with current value
    WiFiManagerParameter custom_coin("coin", "Coin name (bitcoin, ethereum, doge, ...)", COIN_NAME.c_str(), 50);
    wm.addParameter(&custom_coin);
    
    // Start portal blocking mode
    bool success = wm.startConfigPortal("crypto_display", "crypto123");
    
    if (success) {
        COIN_NAME = String(custom_coin.getValue());
        Serial.println("Configuration updated!");
        Serial.print("New coin: ");
        Serial.println(COIN_NAME);
        return true;
    }
    
    Serial.println("Configuration portal timed out or was cancelled");
    return false;
}