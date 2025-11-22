#include "display.h"
#include "utils.h"
#include "network.h"
#include "config.h"
#include "buzzer.h"
#include "button_control.h"

unsigned long last_fetch_time = 0;
const unsigned long FETCH_INTERVAL = 1000 * 60 * 10;  // 10 minutes

void setup() {
  Serial.begin(9600);
  // button for config mode
  pinMode(BUTTON_COIN_PIN, INPUT_PULLUP);
  pinMode(BUTTON_CONFIG_PIN, INPUT_PULLUP);
  pinMode(BUZZER_PIN, OUTPUT);

  turn_on_external_antenna();

  tft.init();
  tft.setRotation(1);
  tft.setSwapBytes(true);
  
  //setup wifi
  display_message("Connecting...");
  //play boot sound
  play_startup_sound();


  if (!setup_wifi_manager()) {
    display_message("Wifi connection failed");
    delay(2000);
    ESP.restart();
  }

  // render_price(0, 0.0f, "FETCHING DATA...", "*24h");
  // CoinData cd = CoinData{132}
  // render_price(1010875, 21.2f, "Bitcoin", "*24h");
  
  // setup_wifi(WIFI_SSID, WIFI_PASSWORD);
  last_fetch_time = 0;
}

void loop() {  
  // === check for coin button === 
  // Check for short press (cycle coins)
  if (check_coin_change_button()) {    
    //try to go to next coin if only one coin is setup dont do anything
    if (go_to_next_coin()) {
      Serial.println("Cycling to next coin");  
      // Show indicator
      int current_index = get_current_coin_index();
      String coin_name = get_coin_name();
      display_message("Switching to: " + coin_name);
      delay(1000);
      
      // Force immediate fetch
      last_fetch_time = 0;
    }
  }

  if (check_coin_fetch_button()) {    
    Serial.println("Fetching new price");
    display_message("Fetching new price");    
    // Force immediate fetch
    last_fetch_time = 0;
  }

  // === check for config button ===
  if (check_config_button()) {
    Serial.println("Config mode entered");
    play_config_sound();

    display_wifi_setup_message("Configue");

    if (start_config_portal_on_demand()) {
      display_message("Config saved, restarting...");
      delay(2000);
      ESP.restart(); //restart to apply changes
    } else {
      display_message("Config canceled");
      last_fetch_time = 0; //fetch imidietly
    }
  }

  if (check_reset_button()) {
    Serial.println("Reseting button pressed");

    display_message("Reseting device");
    delay(1000);
    ESP.restart();
  }


  //check if we should fetch new data
  unsigned long current_time = millis();
  if (current_time - last_fetch_time >= FETCH_INTERVAL || last_fetch_time == 0) {
    Serial.println("Fetching new price...");

    //check if wifi is connected
    if (!check_wifi_connection()) {
      display_message("Wifi reconnect failed, Restarting...");
      delay(2000);
      ESP.restart();
    };

    CoinData fetched_data = fetch_coin_data();
    float price = fetched_data.price;
    
    if (price != -1) {
      render_price(fetched_data, "*24h");
      last_fetch_time = current_time; 
    } else {
      display_message("Error fetching price for: " + get_coin_name());
      // Retry in 30 seconds instead of 10 minutes on error
      last_fetch_time = current_time - FETCH_INTERVAL + 30000;
    }
  }

  delay(100);
}