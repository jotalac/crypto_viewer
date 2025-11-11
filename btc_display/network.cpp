#include "network.h"
#include "utils.h"
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

CoinData fetch_coin_data() {
  CoinData data = {-1, 0.0};

  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi not connected");
    // setup_wifi()
    return data;
  }

  HTTPClient http;
  http.setTimeout(15000);
  http.begin("https://api.coingecko.com/api/v3/coins/markets?vs_currency=usd&ids=bitcoin");
  
  Serial.println("Sending request...");
  int httpCode = http.GET();
  Serial.printf("HTTP Code: %d\n", httpCode);

  if (httpCode == HTTP_CODE_OK) {
    String payload = http.getString();
    Serial.println("Got response, length: " + String(payload.length()));
    
    // Big doc - no filter, just brute force
    DynamicJsonDocument doc(4096);
    DeserializationError error = deserializeJson(doc, payload);
    
    if (!error) {
      Serial.println("JSON parsed successfully");
      
      // Try to extract - with lots of debug
      if (doc.is<JsonArray>() && doc.size() > 0) {
        JsonObject coin_json = doc[0];

        float priceFloat = coin_json["current_price"];
        data.price = (int)priceFloat;
        Serial.printf("Price found: %.2f -> %d\n", priceFloat, data.price);

        data.price_change_percentage = coin_json["price_change_percentage_24h"];
        Serial.printf("Change found: %.2f\n", data.price_change_percentage);

        data.symbol = std::string(coin_json["name"]);
        Serial.printf("Symbol found: %s\n", data.symbol);

        
        // if (coin_json.containsKey("price_change_percentage_24h")) {
          // data.price_change_percentage = coin_json["price_change_percentage_24h"];
          // Serial.printf("Change found: %.2f\n", data.price_change_percentage);
        // } else {
          // Serial.println("ERROR: price_change_percentage_24h not found!");
        // }
      } else {
        Serial.println("ERROR: Response is not an array or empty!");
      }
      
    } else {
      Serial.printf("JSON Error: %s\n", error.c_str());
    }
  } else {
    Serial.printf("HTTP Failed: %s\n", http.errorToString(httpCode).c_str());
  }

  http.end();
  Serial.printf("Returning: Price=%d, Change=%.2f\n", data.price, data.price_change_percentage);
  return data;
}