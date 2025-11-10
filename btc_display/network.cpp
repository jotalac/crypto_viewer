#include "network.h"
#include "utils.h"
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

BtcData fetch_btc_data() {
  BtcData data = {-1, 0.0};

  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi not connected");
    return data;
  }

  HTTPClient http;
  const char* api_url = "https://api.coingecko.com/api/v3/coins/markets?vs_currency=usd&ids=bitcoin&price_change_percentage=24h";
  
  http.begin(api_url);
  http.addHeader("User-Agent", "Mozilla/5.0 (ESP32; Arduino)");
  int httpCode = http.GET();

  if (httpCode > 0) {
    if (httpCode == HTTP_CODE_OK) {
      JsonDocument filter;
      filter[0]["current_price"] = true;
      filter[0]["price_change_percentage_24h_in_currency"] = true;

      JsonDocument doc;
      DeserializationError error = deserializeJson(doc, http.getStream(), DeserializationOption::Filter(filter));

      if (error) {
        Serial.print("deserializeJson() failed: ");
        Serial.println(error.c_str());
      } else {
        data.price = doc[0]["current_price"];
        data.price_change_percentage = doc[0]["price_change_percentage_24h_in_currency"];
      }
    } else {
      Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
    }
  } else {
    Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
  }

  http.end();
  return data;
}
