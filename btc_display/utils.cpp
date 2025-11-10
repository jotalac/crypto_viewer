#include "utils.h"
#include <WiFi.h>
#include <cstdio>


std::string format_price(int number) {
    if (number == 0) {
        return "0";
    }

    std::string result = "";
    int count = 0;

    while (number > 0) {
        if (count > 0 && count % 3 == 0) {
            result += ' ';
        }
        
        result += (char)((number % 10) + '0');
        
        number /= 10;
        count++;
    }

    std::reverse(result.begin(), result.end());

    return result;
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