#include "utils.h"
#include <WiFi.h>


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
    std::array<char, 32> buffer; 
    
    auto [ptr, ec] = std::to_chars(
        buffer.data(),
        buffer.data() + buffer.size(),
        value,
        std::chars_format::fixed,
        precision // This is the '2' for 2 decimal places
    );

    if (ec != std::errc()) {
        return "ERROR"; // Handle error appropriately
    }

    return std::string(buffer.data(), ptr);
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