#include "network.h"
#include "utils.h"
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "config.h"

CoinData fetch_coin_data() {
  CoinData data = {-1, 0.0};

  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi not connected");
    // setup_wifi()
    return data;
  }

  String coin_name = get_coin_name();
  Serial.print("Fetching data for: ");
  Serial.println(coin_name);

  HTTPClient http;
  http.setTimeout(15000);
  http.begin("https://api.coingecko.com/api/v3/coins/markets?vs_currency=usd&ids=" + coin_name);
  
  Serial.println("Sending request to: ");
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

        data.price = coin_json["current_price"];

        data.price_change_percentage = coin_json["price_change_percentage_24h"];
        Serial.printf("Change found: %.2f\n", data.price_change_percentage);

        data.ath_price = coin_json["ath"];
        Serial.println(data.ath_price);

        data.ath_percentage = coin_json["ath_change_percentage"];
        Serial.println(data.ath_percentage);

        data.symbol = std::string(coin_json["name"]);
        Serial.printf("Symbol found: %s\n", data.symbol);

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

GraphData fetch_graph_data() {
  GraphData graph_data = {99999999.0, 0, 0, 0, 0};

  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Wifi not connected for graph data fetch");
    return graph_data;
  }

  String coin_name = get_coin_name();
  coin_name.toLowerCase();
  Serial.println("Fetching graph data: ...");

  HTTPClient http;
  
  http.begin("https://api.coingecko.com/api/v3/coins/" + coin_name + "/market_chart?vs_currency=usd&days=1");
  // http.setUserAgent("Mozilla/5.0 (ESP32)"); 
  
  int httpCode = http.GET();
  Serial.printf("HTTP Code: %d\n", httpCode);

  if (httpCode == HTTP_CODE_OK) {
    // 1. Get the Raw String (Robust method)
    String payload = http.getString();
    Serial.print("Payload size: ");
    Serial.println(payload.length()); // Debug: Should be around 15000-20000

    // 2. Filter the data (Only keep 'prices')
    StaticJsonDocument<200> filter;
    filter["prices"] = true;

    // 3. Parse
    // Increased size to 20000 to be safe
    DynamicJsonDocument doc(24000); 
    DeserializationError error = deserializeJson(doc, payload, DeserializationOption::Filter(filter));

    if (!error) {
      JsonArray prices = doc["prices"];
      int arraySize = prices.size();
      Serial.print("Total points found: ");
      Serial.println(arraySize); // Debug: Should be around 288


      // 4. Loop and Save
      for (int i = 0; i < arraySize; i++) {
        
        // Logic: Save every 10th point OR the very last point
        if ((i % DOWNSAMPLE_GRAPH == 0 && graph_data.data_count < GRAPH_POINTS) || (i == arraySize - 1)) {
          
          // If we are forcing the last point, overwrite the last slot if full
          int saveIndex = graph_data.data_count;
          if (i == arraySize - 1 && graph_data.data_count >= GRAPH_POINTS) {
              saveIndex = GRAPH_POINTS - 1; 
          }

          float val = prices[i][1];
          graph_data.price_history[saveIndex] = val;
          
          // Update Min/Max
          if (val < graph_data.min_price) graph_data.min_price = val;
          if (val > graph_data.max_price) graph_data.max_price = val;
          
          // Increment count only if we didn't overwrite
          if (saveIndex == graph_data.data_count) graph_data.data_count++;
          
          // Set current price to the absolute last one processed
          // currentPrice = val; 
        }
      }
      
      Serial.print("Points saved to RAM: ");
      Serial.println(graph_data.data_count);

    } else {
      Serial.print("JSON Error: "); 
      Serial.println(error.c_str());
    }
  } else {
    Serial.print("HTTP Failed: "); 
    Serial.println(httpCode);
  }
  http.end();
  graph_data.last_fetch = millis();
  return graph_data;
}
