#include "config.h"
#include "utils.h"
#include "display.h"

Preferences preferences;
Timezone myTZ;

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

void setup_time() {
    Serial.println("Waiting for time sync...");
    waitForSync(30); // freezes here until time is received
    Serial.println("Time synced!");

    Serial.println("Timezone is:" + get_timezone());

    if(!myTZ.setLocation(get_timezone())) {
        // set_timezone("Invalid timezone");
        Serial.println("Failed to set timezone.");
    };
}

String get_timezone() {
    preferences.begin("crypto", true);
    String tz = preferences.getString("timezone", "Europe/London");
    preferences.end();
    return tz;
}


String get_coin_name() {
    int index = get_current_screen_index();
    return get_coin_name(index);
}

void set_timezone(String val) {
    preferences.begin("crypto", false);
    preferences.putString("timezone", val);
    preferences.end();
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

int get_current_screen_index() {
    preferences.begin("crypto", false);
    int index = preferences.getInt("current_index", 0);
    preferences.end();
    return index;
}

void set_current_screen_index(int index) {
    preferences.begin("crypto", false);
    preferences.putInt("current_index", index);
    preferences.end();
}

void set_play_sounds(String val) {
    preferences.begin("crypto", false);
    preferences.putString("play_sounds", val);
    preferences.end();
}

bool should_play_sounds() {
    preferences.begin("crypto", true);
    String val = preferences.getString("play_sounds", "T");
    preferences.end();

    return val == "T";
}

bool is_simple_layout() {
    preferences.begin("crypto", true);
    String val = preferences.getString("simple_layout", "");
    preferences.end();

    return val == "T";
}

bool should_display_graph() {
    preferences.begin("crypto", true);
    String val = preferences.getString("display_graph", "T");
    preferences.end();

    return val == "T";
}

// Cycle to next coin
bool go_to_next_screen() {
    int total = get_screen_count();
    if (total <= 1) return false;  // Nothing to cycle
    
    int current = get_current_screen_index();
    int next = (current + 1) % total;
    
    set_current_screen_index(next);
    
    Serial.print("Cycled from coin ");
    Serial.print(current);
    Serial.print(" to coin ");
    Serial.println(next);

    return true;
}

// Get total number of configured coins
int get_screen_count() {
    preferences.begin("crypto", true);
    String coin1 = preferences.getString("coin1", "bitcoin");
    String coin2 = preferences.getString("coin2", "");
    String coin3 = preferences.getString("coin3", "");
    preferences.end();
    
    int count = 1; // for the time screen
    if (!coin1.isEmpty()) count++;
    if (!coin2.isEmpty()) count++;
    if (!coin3.isEmpty()) count++;
    
    return count;
}
