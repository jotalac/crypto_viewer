#include "utils.h"
#include <WiFi.h>
#include <cstdio>
#include <WiFiManager.h>
#include <Preferences.h>
#include "config.h"
#include "display.h"

Preferences preferences;

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

void turn_on_external_antenna() {
    pinMode(3, OUTPUT);    // RF switch power on
    digitalWrite(3, LOW);
    pinMode(14, OUTPUT);   // select external antenna
    digitalWrite(14, HIGH);
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

    //callback when entering web portal config mode
    wm.setAPCallback([](WiFiManager *myWiFiManager) {
        Serial.println("Entered config mode");
        display_wifi_setup_message("Wifi setup");
    });

    bool params_saved = false;
    wm.setSaveParamsCallback([&params_saved]() {
        Serial.println("SaveParams callback triggered!");
        params_saved = true;
    });


    // Load saved coin name from preferences
    preferences.begin("crypto", true);  // false = read/write
    String saved_coin1 = preferences.getString("coin1", "bitcoin");  // default = "bitcoin"
    String saved_coin2 = preferences.getString("coin2", "");
    String saved_coin3 = preferences.getString("coin3", "");
    String saved_play_sounds = preferences.getString("play_sounds", "T");
    preferences.end();

    const char* checkbox_html = saved_play_sounds == "T" ? "type=\"checkbox\" checked" : "type=\"checkbox\"";

    //custom params for coin selection
    WiFiManagerParameter custom_coin1("coin1", "Coin 1 (bitcoin, ethereum, doge, ...)", saved_coin1.c_str(), 50);
    WiFiManagerParameter custom_coin2("coin2", "Coin 2 (bitcoin, ethereum, doge, ...)", saved_coin2.c_str(), 50);
    WiFiManagerParameter custom_coin3("coin3", "Coin 3 (bitcoin, ethereum, doge, ...)", saved_coin3.c_str(), 50);
    WiFiManagerParameter play_sounds("playSound", "Play sounds", "T", 2, checkbox_html, WFM_LABEL_AFTER);
    wm.addParameter(&custom_coin1);
    wm.addParameter(&custom_coin2);
    wm.addParameter(&custom_coin3);
    wm.addParameter(&play_sounds);

    Serial.println("Starting WifiManager...");
    WiFi.mode(WIFI_STA);

    //try to connect
    bool success = wm.autoConnect("crypto_display", "crypto123");

    //if connection fails
    if (!success) {
        Serial.println("Failed to connect to wifi from wifimanager");
        return false;
    }

    Serial.println("Wifi connected");

    //set new coin only if it was edited
    // otherwise it will be always bitcoin (the default paremeter)
    if (params_saved) { 
        String new_coin1 = String(custom_coin1.getValue());
        String new_coin2 = String(custom_coin2.getValue());
        String new_coin3 = String(custom_coin3.getValue());
        String new_play_sounds = String(play_sounds.getValue());

        set_coin_names(new_coin1, new_coin2, new_coin3);
        set_play_sounds(new_play_sounds);

        // if (new_coin.isEmpty()) {Serial.println("Coin name empty"); return true;} //check if the coin is not empty
        Serial.println("Saved coins from portal:");
        Serial.println("  Coin 1: " + new_coin1);
        Serial.println("  Coin 2: " + new_coin2);
        Serial.println("  Coin 3: " + new_coin3);
        Serial.println(new_play_sounds);
    } else {
        Serial.print("Skipped preference save. Current coins: ");
        Serial.println(saved_coin1 + ", " + saved_coin2 + ", " + saved_coin3);
    }

    return true;
    
}


bool check_wifi_connection() {
    if (wifi_connected()) {
        return true;
    }

    display_message("Reconnecting wifi...");
    WiFi.reconnect();

    int attempts = 0;
    while (WiFi.status() != WL_CONNECTED && attempts < 50) {
        delay(1000);
        Serial.print(".");
        attempts++;
    }
    
    Serial.println();
    
    if (WiFi.status() == WL_CONNECTED) {
        Serial.println("WiFi reconnected successfully!");
        Serial.print("IP address: ");
        Serial.println(WiFi.localIP());
        return true;
    }
    
    Serial.println("WiFi reconnection failed");
    return false;
}

bool wifi_connected() {
    return WiFi.status() == WL_CONNECTED;
}

// Start configuration portal on demand
bool start_config_portal_on_demand() {
    Serial.println("Starting configuration portal on demand...");
    
    WiFiManager wm;
    
    wm.setConfigPortalTimeout(PORTAL_TIMEOUT);
    wm.setClass("invert");
    wm.setBreakAfterConfig(true);

    // Load current coin name from preferences
    preferences.begin("crypto", true);
    String current_coin1 = preferences.getString("coin1", "bitcoin");
    String current_coin2 = preferences.getString("coin2", "");
    String current_coin3 = preferences.getString("coin3", "");
    String saved_play_sounds = preferences.getString("play_sounds", "T");
    preferences.end();

    const char* checkbox_html = saved_play_sounds == "T" ? "type=\"checkbox\" checked" : "type=\"checkbox\"";
    
    WiFiManagerParameter custom_coin1("coin1", "Coin 1 (bitcoin, ethereum, doge, ...)", current_coin1.c_str(), 50);
    WiFiManagerParameter custom_coin2("coin2", "Coin 2 (optional)", current_coin2.c_str(), 50);
    WiFiManagerParameter custom_coin3("coin3", "Coin 3 (optional)", current_coin3.c_str(), 50);
    WiFiManagerParameter play_sounds("playSound", "Play sounds", "T", 2, checkbox_html, WFM_LABEL_AFTER);

    
    wm.addParameter(&custom_coin1);
    wm.addParameter(&custom_coin2);
    wm.addParameter(&custom_coin3);
    wm.addParameter(&play_sounds);

    // Track if config was saved
    bool config_saved = false;
    // Capture by reference - this is the key!
    wm.setSaveConfigCallback([&config_saved]() {
        Serial.println("Save callback triggered - config saved!");
        config_saved = true;
    });
    
    // Start portal blocking mode
    bool success = wm.startConfigPortal("crypto_display", "crypto123");
    
    String new_coin1 = String(custom_coin1.getValue());
    String new_coin2 = String(custom_coin2.getValue());
    String new_coin3 = String(custom_coin3.getValue());
    String new_play_sounds = String(play_sounds.getValue());
    
    if (!new_coin1.isEmpty()) {
        set_coin_names(new_coin1, new_coin2, new_coin3);
        set_play_sounds(new_play_sounds);
        Serial.println("Coins saved:");
        Serial.println("  " + new_coin1);
        Serial.println("  " + new_coin2);
        Serial.println("  " + new_coin3);
        Serial.println(String(play_sounds.getValue()));
    }
    
    if (success || config_saved) {
        return true;
    } else {
        return false;
    }
}

String get_coin_name() {
    int index = get_current_coin_index();
    return get_coin_name(index);
}

String get_coin_name(int index) {
    preferences.begin("crypto", true);
    String key = "coin" + String(index + 1);
    String coin = preferences.getString(key.c_str(), index == 0 ? "bitcoin" : "");
    preferences.end();
    return coin;
}

void set_coin_names(String coin1, String coin2, String coin3) {
    preferences.begin("crypto", false);
    preferences.putString("coin1", coin1);
    preferences.putString("coin2", coin2);
    preferences.putString("coin3", coin3);
    preferences.end();
}

int get_current_coin_index() {
    preferences.begin("crypto", false);
    int index = preferences.getInt("current_index", 0);
    preferences.end();
    return index;
}

void set_current_coin_index(int index) {
    preferences.begin("crypto", false);
    preferences.putInt("current_index", index);
    preferences.end();
}

void set_play_sounds(String val) {
    preferences.begin("crypto", false);
    preferences.putString("play_sounds", val);
    preferences.end();
}

String get_play_sounds() {
    preferences.begin("crypto", true);
    String val = preferences.getString("play_sounds", "T");
    preferences.end();

    return val;
}

// Cycle to next coin
bool go_to_next_coin() {
    int total = get_coin_count();
    if (total <= 1) return false;  // Nothing to cycle
    
    int current = get_current_coin_index();
    int next = (current + 1) % total;
    
    set_current_coin_index(next);
    
    Serial.print("Cycled from coin ");
    Serial.print(current);
    Serial.print(" to coin ");
    Serial.println(next);

    return true;
}

// Get total number of configured coins
int get_coin_count() {
    preferences.begin("crypto", true);
    String coin1 = preferences.getString("coin1", "bitcoin");
    String coin2 = preferences.getString("coin2", "");
    String coin3 = preferences.getString("coin3", "");
    preferences.end();
    
    int count = 0;
    if (!coin1.isEmpty()) count++;
    if (!coin2.isEmpty()) count++;
    if (!coin3.isEmpty()) count++;
    
    return count;
}