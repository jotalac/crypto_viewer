#include "display.h"
#include "fonts/mono_small.h"
#include "fonts/mono_bold_50.h"
#include "fonts/mono_bold_60.h"
#include "fonts/mono_medium.h"
#include "background.h"
#include "utils.h"

TFT_eSPI tft = TFT_eSPI();

void draw_gradient(uint16_t color) {
  tft.fillRectVGradient(0, 0, tft.width(), tft.height(), TFT_BLACK, color);
}

void print_background() {
  tft.pushImage(0, 0, tft.width(), tft.height(), bg_img);
}

void draw_curreny_title(String currency_title) {
  tft.setTextColor(TFT_WHITE, TFT_TRANSPARENT);
  
  tft.loadFont(mono_small);
  tft.drawString(currency_title , 20, 20);
  tft.unloadFont();
}

void draw_price(float price) {
  std::string tempPriceString;  
  if (price == 0) {
    tempPriceString = "$ ...";
  } else {
    tempPriceString = "$ " + format_price(price);
  }
  String priceDisplay(tempPriceString.c_str());


  //the the right font size for the test length
  int maxWidth = tft.width() - 30;
  tft.loadFont(mono_bold_60);
  if (tft.textWidth(priceDisplay) > maxWidth) {
    tft.unloadFont();
    tft.loadFont(mono_bold_50);
    
    if (tft.textWidth(priceDisplay) > maxWidth) {
      tft.unloadFont();
      tft.loadFont(mono_medium);
    }
  }
  

  tft.setTextDatum(MC_DATUM);
  tft.setTextColor(TFT_WHITE, TFT_TRANSPARENT);
  tft.drawString(priceDisplay, tft.width() / 2, 80);
  tft.unloadFont();

  tft.setTextDatum(TL_DATUM);
}



void draw_price_change(float price_change, String time_frame) {
  uint16_t bg_color;
  String display_string;

  //set the dispaly string
  if (price_change == 0.0f) {
    display_string = "...";
  } else {
    std::string temp = format_price_change(price_change, 2) + "%"; 
    display_string = String(temp.c_str());
  }

  //set the backgournd color
  if (price_change < 0) {
    bg_color = TFT_RED;
  } else {
    display_string = "+ " + display_string;
    bg_color = DARK_GREEN;
  }

  tft.loadFont(mono_medium);
  tft.setTextDatum(MC_DATUM);

  int padding = 10;
  int textWidth = tft.textWidth(display_string);
  int textHeight = tft.fontHeight();

  int x = tft.width() / 2;
  int y = 160;

  tft.fillRoundRect(x - textWidth / 2 - padding, y - textHeight / 2 - padding, textWidth + padding * 2, textHeight + padding * 2, 10, bg_color);
  tft.setTextColor(TFT_WHITE, TFT_TRANSPARENT);
  tft.drawString(display_string, x, y - 5);

  tft.unloadFont();

  //print the change time frame

  tft.loadFont(mono_small);
  tft.setTextColor(TFT_WHITE, TFT_TRANSPARENT);
  tft.drawString(time_frame, x, y + 40);
  tft.unloadFont();

  tft.setTextDatum(TL_DATUM); // Reset to default
}


void render_price(const float price, const float price_change, const String& currency_title, const String time_frame) {
  
  // print_background(); // prints image
  uint16_t color = price_change >= 0 ? DARK_YELLOW : TFT_RED;
  draw_gradient(color);

  draw_curreny_title(currency_title);
  draw_price(price);
  draw_price_change(price_change, time_frame);
}


void display_message(String message) {
  // tft.fillRect(0, 0, tft.width(), tft.height(), TFT_BLACK);
  draw_gradient(TFT_RED);

  tft.setTextDatum(MC_DATUM);
  tft.loadFont(mono_small);
  tft.drawString(message, tft.width()/2, 100);
  tft.setTextDatum(TL_DATUM);

  tft.unloadFont();
}

void display_wifi_setup_message(String message) {
  tft.fillRect(0, 0, tft.width(), tft.height(), TFT_BLACK);

  
  tft.setTextColor(TFT_SKYBLUE, TFT_TRANSPARENT);
  
  tft.loadFont(mono_medium);
  tft.drawString(message, 20, 20);
  tft.unloadFont();
  
  tft.setTextColor(TFT_WHITE, TFT_TRANSPARENT);
  tft.loadFont(mono_small);

  tft.drawString("Connect to:", 20, 70);

  tft.drawString("SSID:", 30, 110);
  tft.drawString("crypto_display", 40, 130);
  
  tft.drawString("Password:", 30, 160);
  tft.drawString("crypto123", 40, 180);
  
  tft.unloadFont();
}