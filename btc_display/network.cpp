#include "network.h"
#include "utils.h"
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

// BtcData fetch_btc_data() {
//   BtcData data = {-1, 0.0};

//   if (WiFi.status() != WL_CONNECTED) {
//     Serial.println("WiFi not connected");
//     return data;
//   }

//   HTTPClient http;
//   const char* api_url = "https://api.coingecko.com/api/v3/coins/markets?vs_currency=usd&ids=bitcoin&price_change_percentage=24h";
//   // const char* api_url = "https://api.coingecko.com/api/v3/simple/price?ids=bitcoin&vs_currencies=usd";

//   http.begin(api_url);
//   http.addHeader("User-Agent", "Mozilla/5.0 (ESP32; Arduino)");
//   int httpCode = http.GET();

//   if (httpCode > 0) {
//     if (httpCode == HTTP_CODE_OK) {
//       JsonDocument filter;
//       filter[0]["current_price"] = true;
//       filter[0]["price_change_percentage_24h_in_currency"] = true;

//       JsonDocument doc;
//       DeserializationError error = deserializeJson(doc, http.getStream(), DeserializationOption::Filter(filter));

//       if (error) {
//         Serial.print("deserializeJson() failed: ");
//         Serial.println(error.c_str());
//       } else {
//         data.price = doc[0]["current_price"];
//         data.price_change_percentage = doc[0]["price_change_percentage_24h_in_currency"];
//       }
//     } else {
//       Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
//     }
//   } else {
//     Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
//   }

//   http.end();
//   return data;
// }

// BtcData fetch_btc_data() {
//   BtcData data = {-1, 0.0};

//   if (WiFi.status() != WL_CONNECTED) {
//     Serial.println("WiFi not connected");
//     return data;
//   }

//   HTTPClient http;
//   http.setTimeout(15000);
//   http.begin("https://api.coingecko.com/api/v3/coins/markets?vs_currency=usd&ids=bitcoin&price_change_percentage=24h");
  
//   Serial.println("Sending request...");
//   int httpCode = http.GET();
//   Serial.printf("HTTP Code: %d\n", httpCode);

//   if (httpCode == HTTP_CODE_OK) {
//     // Get raw response to debug
//     String payload = http.getString();
//     Serial.println("=== RAW API RESPONSE ===");
//     Serial.println(payload);
//     Serial.println("=== END RESPONSE ===");
    
//     // Parse WITHOUT filter first to see everything
//     StaticJsonDocument<2048> doc;
//     DeserializationError error = deserializeJson(doc, payload);
    
//     Serial.printf("Deserialization: %s\n", error.c_str());
    
//     if (!error) {
//       Serial.println("=== PARSED JSON ===");
//       serializeJsonPretty(doc, Serial);
//       Serial.println("\n=== END PARSED ===");
      
//       // Extract data
//       data.price = doc[0]["current_price"] | -1;
//       data.price_change_percentage = doc[0]["price_change_percentage_24h"] | 0.0f;
      
//       Serial.printf("Extracted: Price=%d, Change=%.2f%%\n", data.price, data.price_change_percentage);
//     } else {
//       Serial.printf("JSON Error: %s\n", error.c_str());
//     }
//   } else {
//     Serial.printf("HTTP Failed: %s\n", http.errorToString(httpCode).c_str());
//   }

//   http.end();
//   return data;
// }

BtcData fetch_btc_data() {
  BtcData data = {-1, 0.0};

  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi not connected");
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
        JsonObject bitcoin = doc[0];
        
        if (bitcoin.containsKey("current_price")) {
          float priceFloat = bitcoin["current_price"];
          data.price = (int)priceFloat;
          Serial.printf("Price found: %.2f -> %d\n", priceFloat, data.price);
        } else {
          Serial.println("ERROR: current_price not found!");
        }
        
        if (bitcoin.containsKey("price_change_percentage_24h")) {
          data.price_change_percentage = bitcoin["price_change_percentage_24h"];
          Serial.printf("Change found: %.2f\n", data.price_change_percentage);
        } else {
          Serial.println("ERROR: price_change_percentage_24h not found!");
        }
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