#include "display.h"
#include "fonts/mono_small.h"
#include "fonts/mono_bold_50.h"
#include "fonts/mono_medium.h"
#include "background.h"
#include "utils.h"

TFT_eSPI tft = TFT_eSPI();
TFT_eSprite spr = TFT_eSprite(&tft);

void create_sprite() {
  void* ptr = spr.createSprite(tft.width(), tft.height(), 1);  // Change 16 to 1
  if (ptr == nullptr) {
    Serial.println("!!! FATAL: Sprite creation failed. Not enough memory?");
    while(1); // Halt execution
  }
  spr.fillSprite(TFT_BLACK);
  spr.setSwapBytes(true);
}

void draw_gradient() {
  spr.createSprite(tft.width(), tft.height(), 16);
  spr.fillSprite(TFT_BLACK);
  spr.fillRectVGradient(0, 0, tft.width(), tft.height(), TFT_BLACK, DARK_YELLOW);
}

void print_background() {
  spr.pushImage(0, 0, 280, 240, bg_img);
}

void draw_curreny_title(String currency_title) {
  spr.setTextColor(TFT_WHITE, TFT_TRANSPARENT);
  
  spr.loadFont(mono_small);
  spr.drawString(currency_title, 20, 20);
  spr.unloadFont();
}

void draw_price(int price) {
  std::string tempPriceString;  
  if (price == 0) {
    tempPriceString = "$ ...";
  } else {
    tempPriceString = "$ " + format_price(price);
  }
  String priceDisplay(tempPriceString.c_str());
  
  spr.setTextColor(TFT_WHITE, TFT_TRANSPARENT);
  spr.loadFont(mono_bold_50);
  spr.drawString(priceDisplay, 20, 60);
  spr.unloadFont();
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

  spr.loadFont(mono_medium);
  spr.setTextDatum(MC_DATUM);

  int padding = 10;
  int textWidth = spr.textWidth(display_string);
  int textHeight = spr.fontHeight();

  int x = spr.width() / 2;
  int y = 160;

  spr.fillRoundRect(x - textWidth / 2 - padding, y - textHeight / 2 - padding, textWidth + padding * 2, textHeight + padding * 2, 10, bg_color);
  spr.setTextColor(TFT_WHITE, TFT_TRANSPARENT);
  spr.drawString(display_string, x, y - 5);

  spr.unloadFont();


  //print the change time frame

  spr.loadFont(mono_small);
  spr.setTextColor(TFT_WHITE, TFT_TRANSPARENT);
  spr.drawString(time_frame, x, y + 40);
  spr.unloadFont();

  spr.setTextDatum(TL_DATUM); // Reset to default
}


void render_price(const int price, const float price_change, const String& currency_title, const String time_frame) {
  tft.fillScreen(TFT_BLACK);
  
  print_background();
  draw_curreny_title(currency_title);
  draw_price(price);
  draw_price_change(price_change, time_frame);

  spr.pushSprite(0 ,0);
}
