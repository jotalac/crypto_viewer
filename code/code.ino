#include "display.h"
#include "utils.h"
#include "network.h"
#include "config.h"
#include "buzzer.h"
#include "button_control.h"

unsigned long last_fetch_time = 0;
const unsigned long FETCH_INTERVAL = 1000 * 60 * 10;  // 10 minutes
const unsigned long FETCH_GRAPH_INTERVAL = 1000 * 60 * 60;  // 1 hour
CoinData all_coins_data[3] = {{}, {}, {}};
GraphData all_graph_data[3] = {{}, {}, {}};


void setup() {
  Serial.begin(9600);

  // setup buttons and anthena  pins
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

  // last_fetch_time = 0;
}

void loop() {  
  //check all button presses
  check_btn_1_press();
  check_btn1_hold();

  check_btn2_press();
  check_btn2_hold();

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

    int current_coin_index = get_current_coin_index();

    //fetch coin data
    CoinData fetched_data = fetch_coin_data();

    //check if we need to fetch new grapth (every 1 hour)
    int graph_last_fetch_time = all_graph_data[current_coin_index].last_fetch; 
    GraphData graph_data = {0, 0, 0, 0, 0};
    if (current_time - graph_last_fetch_time >= FETCH_GRAPH_INTERVAL || graph_last_fetch_time == 0) {
      graph_data = fetch_graph_data();
    }

    if (fetched_data.price != -1) {
      //display new data
      render_price(fetched_data, graph_data, "*24h");
      last_fetch_time = current_time;
      //update the saved vlaues
      all_coins_data[current_coin_index] = fetched_data;
      all_graph_data[current_coin_index] = graph_data;
      //play sound if the current price is ath
      if (fetched_data.ath_percentage >= 0) play_ath_sound();
    } else {
      display_message("Fetch error for: " + get_coin_name());
      // Retry in 30 seconds instead of 10 minutes on error
      last_fetch_time = current_time - FETCH_INTERVAL + 30000;
    }

  }

  delay(100);
}

void check_btn_1_press() {
  if (check_coin_change_button()) {    
      //try to go to next coin if only one coin is setup dont do anything
      if (go_to_next_coin()) {
        Serial.println("Cycling to next coin");  
        // Show indicator
        int current_index = get_current_coin_index();
        String coin_name = get_coin_name();
        display_message("Switching to: " + coin_name);
        delay(500);
        
        // Force immediate fetch
        if (all_coins_data[current_index].symbol.empty()) {
          last_fetch_time = 0;
        } else {
          render_price(all_coins_data[current_index], all_graph_data[current_index], "*24h");
        }
      }
  }
}

void check_btn1_hold() {
  if (check_coin_fetch_button()) {    
    Serial.println("Fetching new price");
    display_message("Fetching new price");    
    // Force immediate fetch
    last_fetch_time = 0;
  }
}

void check_btn2_press() {
  if (check_reset_button()) {
    Serial.println("Reseting button pressed");

    display_message("Reseting device");
    delay(1000);
    ESP.restart();
  }
}

void check_btn2_hold() {
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
}