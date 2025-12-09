#include <string>
#include <Arduino.h>

//colors
#define DARK_YELLOW 0x8c61
#define DARK_GREEN 0x4de2
#define DARK_PURPLE 0x80b3
#define DARK_BLUE 0x11b3
#define LIGHT_GRAY 0xce79

#define GRAPH_FILL_RED 0x5061
#define GRAPH_LINE_RED 0xc1c3

#define GRAPH_FILL_GREEN 0x00c0
#define GRAPH_LINE_GREEN 0x0d23


std::string format_price(float number);
std::string format_price_change(float price_change, int precision);

void turn_on_external_antenna();


String get_coin_name();
String get_coin_name(int index);
void set_coin_names(String coin1, String coin2, String coin3);
int get_current_coin_index();
void set_current_coin_index(int index);

void set_play_sounds(String val);
bool should_play_sounds();

bool is_simple_layout();
bool should_display_graph();

bool go_to_next_coin();
int get_coin_count(); //how many coins are configured
