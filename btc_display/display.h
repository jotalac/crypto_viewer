#include <TFT_eSPI.h>
#include <string>

extern TFT_eSPI tft;
extern TFT_eSprite spr;

void create_sprite();

void draw_gradient();

void print_background();

void draw_curreny_title(String currency_title);

void draw_price(int price);

void draw_price_change(String price_change, String time_frame);

void render_price(const int price, const float price_change, const String& currency_title, const String time_frame);