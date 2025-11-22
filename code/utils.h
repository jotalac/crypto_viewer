#include <string>
#include <Arduino.h>

//colors
#define DARK_YELLOW 0x8c61
#define DARK_GREEN 0x4de2
#define DARK_PURPLE 0x80b3
#define DARK_BLUE 0x11b3
#define LIGHT_GRAY 0xce79

std::string format_price(float number);
std::string format_price_change(float price_change, int precision);

void turn_on_external_antenna();

//wifi
void setup_wifi(const char* ssid, const char* password);
bool setup_wifi_manager();
bool check_wifi_connection();
bool wifi_connected();
bool start_config_portal_on_demand();

// bool check_config_button(); 
// bool check_coin_change_button();

String get_coin_name();
String get_coin_name(int index);
void set_coin_names(String coin1, String coin2, String coin3);
int get_current_coin_index();
void set_current_coin_index(int index);

bool go_to_next_coin();
int get_coin_count(); //how many coins are configured
