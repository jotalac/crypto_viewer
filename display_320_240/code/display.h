#include <TFT_eSPI.h>
#include <string>

extern TFT_eSPI tft;

void draw_gradient(uint16_t color);

void print_background();

void draw_curreny_title(String currency_title);

void draw_price(float price);

void draw_price_change(float price_change, String time_frame);

void render_price(const float price, const float price_change, const String& currency_title, const String time_frame);

void display_message(String message);

void display_wifi_setup_message(String message);