#include <string>

//colors
#define DARK_YELLOW 0x8c61
#define DARK_GREEN 0x4de2

std::string format_price(float number);
std::string format_price_change(float price_change, int precision);

//wifi
void setup_wifi(const char* ssid, const char* password);
bool setup_wifi_manager();

bool start_config_portal_on_demand();
bool check_config_button(); 