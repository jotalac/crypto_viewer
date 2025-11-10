#include <string>

//colors
#define DARK_YELLOW 0x8c61
#define DARK_GREEN 0x4de2

std::string format_price(int number);

std::string format_price_change(float price_change, int precision);

void setup_wifi(const char* ssid, const char* password);