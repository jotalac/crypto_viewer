#include <string>

struct ConfigParams {
    String coin1;
    String coin2;
    String coin3;
    String play_sounds;
    String simple_layout;
    String display_graph;
};

ConfigParams load_config();

void save_config(const ConfigParams& config);

void setup_wm_parameters(WiFiManager& wm, const ConfigParams& config, 
                        WiFiManagerParameter& coin1_param,
                        WiFiManagerParameter& coin2_param,
                        WiFiManagerParameter& coin3_param,
                        WiFiManagerParameter& sounds_param,
                        WiFiManagerParameter& simple_param,
                        WiFiManagerParameter& graph_param);

ConfigParams get_wm_values(WiFiManagerParameter& coin1_param,
                           WiFiManagerParameter& coin2_param,
                           WiFiManagerParameter& coin3_param,
                           WiFiManagerParameter& sounds_param,
                           WiFiManagerParameter& simple_param,
                           WiFiManagerParameter& graph_param);

                           
bool setup_wifi_manager();
bool start_config_portal_on_demand();

bool check_wifi_connection();
bool wifi_connected();


