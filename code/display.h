#include "config.h"
#include <string>
#include "network.h"

void draw_gradient(uint16_t color);

void draw_curreny_title(String currency_title);

void draw_price(float price, bool centered);

void draw_price_change(float price_change, String time_frame);

void draw_ath_data(float ath_price, float ath_change);

void render_screen(const CoinData &fetchedData, const GraphData &graph_data, const String time_frame);

void display_message(String message);

void display_wifi_setup_message(String message);

void draw_graph(GraphData graph_data, bool is_growing);