#include "display.h"
#include "utils.h"
#include "network.h"

// --- Wi-Fi Credentials ---
const char* ssid = "Honor 20";
const char* password = "nevimasi123";

void setup() {
  Serial.begin(9600);

  tft.init();
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);
  
  create_sprite();
  render_price(0, 0.0f, "BTC/USD", "*24h");

  setup_wifi(ssid, password);
}

void loop() {
  Serial.println("Fetching new price...");
  
  BtcData fetched_data = fetch_btc_data();
  int price = fetched_data.price;
  float price_change = fetched_data.price_change_percentage;

  create_sprite();
  
  if (price != -1) {
    render_price(price, price_change, "BTC/USD", "*24h");
  } else {
    spr.drawString("Error fetching price", 20, 100);
    spr.pushSprite(0, 0);
  }

  delay(300000); 
}