#include "display.h"
#include "utils.h"
#include "network.h"
#include "config.h"

unsigned long last_fetch_time = 0;
const unsigned long FETCH_INTERVAL = 1000 * 60 * 10;  // 10 minutes

void setup() {
  Serial.begin(9600);
  delay(500);

  // button for config mode
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  //turn on external antenna
  pinMode(3, OUTPUT);    // RF switch power on
  digitalWrite(3, LOW);
  pinMode(14, OUTPUT);   // select external antenna
  digitalWrite(14, HIGH);

  tft.init();
  tft.setRotation(1);
  tft.setSwapBytes(true);
  
  //setup wifi
  display_message("Connecting...");

  if (!setup_wifi_manager()) {
    display_message("Wifi connection failed");
    delay(2000);
    ESP.restart();
  }

  // render_price(0, 0.0f, "FETCHING DATA...", "*24h");
  // render_price(1010875, 21.2f, "Bitcoin", "*24h");
  
  // setup_wifi(WIFI_SSID, WIFI_PASSWORD);
  last_fetch_time = 0;
}

void loop() {  
  if (check_config_button()) {
    Serial.println("Config mode entered");

    display_wifi_setup_message("--Configue--");

    if (start_config_portal_on_demand()) {
      display_message("Config saved\nRestarting...");
      delay(2000);
      ESP.restart(); //restart to apply changes
    } else {
      display_message("Config canceled");
      last_fetch_time = 0; //fetch imidietly
    }
  }

  //check if we should fetch new data
  unsigned long current_time = millis();
  if (current_time - last_fetch_time >= FETCH_INTERVAL || last_fetch_time == 0) {
    Serial.println("Fetching new price...");

    //check if wifi is connected
    if (!check_wifi_connection()) {
      display_message("Wifi reconnect failed\nRestarting...");
      delay(2000);
      ESP.restart();
    };

    CoinData fetched_data = fetch_coin_data();
    float price = fetched_data.price;
    float price_change = fetched_data.price_change_percentage;
    std::string coin_symbol = fetched_data.symbol;
    
    if (price != -1) {
      render_price(price, price_change, String(coin_symbol.c_str()), "*24h");
      last_fetch_time = current_time; 
    } else {
      display_message("Error fetching price");
      // Retry in 30 seconds instead of 10 minutes on error
      last_fetch_time = current_time - FETCH_INTERVAL + 30000;
    }
  }

  delay(100);
}