#include "display.h"
#include "utils.h"
#include "network.h"

// --- Wi-Fi Credentials ---
// const char* ssid = "Honor 20";
const char* ssid = "Trisolaris";
const char* password = "abraKA27+dabRA-";


void setup() {
  Serial.begin(9600);

  tft.init();
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);
  
  create_sprite();
  render_price(0, 0.0f, "FETCHING DATA...", "*24h");

  setup_wifi(ssid, password);
}

void loop() {
  Serial.println("Fetching new price...");
  
  CoinData fetched_data = fetch_coin_data();
  int price = fetched_data.price;
  float price_change = fetched_data.price_change_percentage;
  std::string coin_symbol = fetched_data.symbol;
  
  if (price != -1) {
    render_price(price, price_change, String(coin_symbol.c_str()), "*24h");
    delay(1000 * 60 * 2); 
  } else {
    display_error_screen("Error fetching price");
    delay(1000 * 30);
  }

}