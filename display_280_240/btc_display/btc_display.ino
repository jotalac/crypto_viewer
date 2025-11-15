#include "display.h"
#include "utils.h"
#include "network.h"
#include "config.h"

unsigned long last_fetch_time = 0;
const unsigned long FETCH_INTERVAL = 1000 * 60 * 10;  // 10 minutes

void setup() {
  Serial.begin(9600);
  delay(500);

  pinMode(BUTTON_PIN, INPUT_PULLUP);

  tft.init();
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);
  
  create_sprite();

  //setup wifi
  display_wifi_setup_message();

  if (!setup_wifi_manager()) {
    display_error_screen("Wifi connection failed");
    delay(2000);
    ESP.restart();
  }

  render_price(0, 0.0f, "FETCHING DATA...", "*24h");

  // setup_wifi(WIFI_SSID, WIFI_PASSWORD);
  last_fetch_time = 0;
}

void loop() {
  if (check_config_button()) {
    Serial.println("Config mode entered");

    display_wifi_setup_message();

    if (start_config_portal_on_demand()) {
      ESP.restart(); //restart to apply changes
    } else {
      display_error_screen("Config canceled");
      last_fetch_time = 0; //fetch imidietly
    }
  }

  //check if we should fetch new data
  unsigned long current_time = millis();
  if (current_time - last_fetch_time >= FETCH_INTERVAL || last_fetch_time == 0) {
    Serial.println("Fetching new price...");

    CoinData fetched_data = fetch_coin_data();
    float price = fetched_data.price;
    float price_change = fetched_data.price_change_percentage;
    std::string coin_symbol = fetched_data.symbol;
    
    if (price != -1) {
      render_price(price, price_change, String(coin_symbol.c_str()), "*24h");
      last_fetch_time = current_time; 
    } else {
      display_error_screen("Error fetching price");
      // Retry in 30 seconds instead of 10 minutes on error
      last_fetch_time = current_time - FETCH_INTERVAL + 30000;
    }
  }

  delay(100);
}