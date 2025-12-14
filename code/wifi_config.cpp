#include "config.h"
#include "utils.h"
#include "wifi_config.h"
#include "display.h"


ConfigParams load_config() {
    preferences.begin("crypto", true);
    ConfigParams config;
    
    config.coin1 = preferences.getString("coin1", "bitcoin");
    config.coin2 = preferences.getString("coin2", "");
    config.coin3 = preferences.getString("coin3", "");
    config.timezone = preferences.getString("timezone", "Europe/London");
    config.play_sounds = preferences.getString("play_sounds", "T");
    config.simple_layout = preferences.getString("simple_layout", "");
    config.display_graph = preferences.getString("display_graph", "T");
    preferences.end();
    return config;
}

void save_config(const ConfigParams& config) {
    preferences.begin("crypto", false);
    preferences.putString("coin1", config.coin1);
    preferences.putString("coin2", config.coin2);
    preferences.putString("coin3", config.coin3);
    preferences.putString("timezone", config.timezone);
    preferences.putString("play_sounds", config.play_sounds);
    preferences.putString("simple_layout", config.simple_layout);
    preferences.putString("display_graph", config.display_graph);
    preferences.end();
    
    Serial.println("Config saved:");
    Serial.println("  Coin 1: " + config.coin1);
    Serial.println("  Coin 2: " + config.coin2);
    Serial.println("  Coin 3: " + config.coin3);
    Serial.println("  Time zone: " + config.timezone);
    Serial.println("  Play sounds: " + config.play_sounds);
    Serial.println("  Simple display: " + config.simple_layout);
    Serial.println("  Display graph: " + config.display_graph);
}

void setup_wm_parameters(WiFiManager& wm, const ConfigParams& config, 
                        WiFiManagerParameter& coin1_param,
                        WiFiManagerParameter& coin2_param,
                        WiFiManagerParameter& coin3_param,
                        WiFiManagerParameter& timezone_param,
                        WiFiManagerParameter& sounds_param,
                        WiFiManagerParameter& simple_param,
                        WiFiManagerParameter& graph_param) 
                        
    {
    
    // Create checkbox HTML attributes
    const char* checkbox_sounds = config.play_sounds == "T" ? "type=\"checkbox\" checked" : "type=\"checkbox\"";
    const char* checkbox_simple = config.simple_layout == "T" ? "type=\"checkbox\" checked" : "type=\"checkbox\"";
    const char* checkbox_graph = config.display_graph == "T" ? "type=\"checkbox\" checked" : "type=\"checkbox\"";
    
    // Initialize parameters
    new (&coin1_param) WiFiManagerParameter("coin1", "Coin 1 (bitcoin, ethereum, doge, ...)", config.coin1.c_str(), 50);
    new (&coin2_param) WiFiManagerParameter("coin2", "Coin 2 (optional)", config.coin2.c_str(), 50);
    new (&coin3_param) WiFiManagerParameter("coin3", "Coin 3 (optional)", config.coin3.c_str(), 50);
    new (&timezone_param) WiFiManagerParameter("timezone", "Timezone: 'Continent/City' (eg. America/New_York, ...)", config.timezone.c_str(), 50);
    new (&sounds_param) WiFiManagerParameter("playSound", "Play sounds", "T", 2, checkbox_sounds, WFM_LABEL_AFTER);
    new (&simple_param) WiFiManagerParameter("simpleLayout", "Simple layout", "T", 2, checkbox_simple, WFM_LABEL_AFTER);
    new (&graph_param) WiFiManagerParameter("displayGraph", "Display graph", "T", 2, checkbox_graph, WFM_LABEL_AFTER);
    
    // Add parameters to WiFiManager
    wm.addParameter(&coin1_param);
    wm.addParameter(&coin2_param);
    wm.addParameter(&coin3_param);
    wm.addParameter(&timezone_param);
    wm.addParameter(&sounds_param);
    wm.addParameter(&simple_param);
    wm.addParameter(&graph_param);
}

ConfigParams get_wm_values(WiFiManagerParameter& coin1_param,
                           WiFiManagerParameter& coin2_param,
                           WiFiManagerParameter& coin3_param,
                           WiFiManagerParameter& timezone_param,
                           WiFiManagerParameter& sounds_param,
                           WiFiManagerParameter& simple_param,
                           WiFiManagerParameter& graph_param) {
    ConfigParams config;
    config.coin1 = String(coin1_param.getValue());
    config.coin2 = String(coin2_param.getValue());
    config.coin3 = String(coin3_param.getValue());
    config.timezone = String(timezone_param.getValue());
    config.play_sounds = String(sounds_param.getValue());
    config.simple_layout = String(simple_param.getValue());
    config.display_graph = String(graph_param.getValue());
    return config;
}

bool setup_wifi_manager() {
    WiFiManager wm;
    // wm.resetSettings(); //reset saved settings  testing

    wm.setConfigPortalTimeout(PORTAL_TIMEOUT);
    wm.setClass("invert");
    wm.setAPCallback([](WiFiManager *myWiFiManager) {
        Serial.println("Entered config mode");
        display_wifi_setup_message("Wifi setup");
    });

    bool params_saved = false;
    wm.setSaveParamsCallback([&params_saved]() {
        Serial.println("SaveParams callback triggered!");
        params_saved = true;
    });

    // Load current config
    ConfigParams config = load_config();
    
    // Setup parameters
    WiFiManagerParameter coin1_param, coin2_param, coin3_param;
    WiFiManagerParameter timezone_param, sounds_param, simple_param, graph_param;
    
    setup_wm_parameters(wm, config, coin1_param, coin2_param, coin3_param, timezone_param,
                       sounds_param, simple_param, graph_param);

    Serial.println("Starting WifiManager...");
    WiFi.mode(WIFI_STA);

    bool success = wm.autoConnect("crypto_display", "crypto123");

    if (!success) {
        Serial.println("Failed to connect to wifi from wifimanager");
        return false;
    }

    Serial.println("Wifi connected");

    // Save new config if params were changed
    if (params_saved) {
        ConfigParams new_config = get_wm_values(coin1_param, coin2_param, coin3_param, timezone_param,
                                                sounds_param, simple_param, graph_param);
        save_config(new_config);
    } else {
        Serial.println("Skipped preference save. Current config unchanged.");
    }

    return true;
}

bool start_config_portal_on_demand() {
    Serial.println("Starting configuration portal on demand...");
    
    WiFiManager wm;
    wm.setConfigPortalTimeout(PORTAL_TIMEOUT);
    wm.setClass("invert");
    wm.setBreakAfterConfig(true);

    bool config_saved = false;
    wm.setSaveConfigCallback([&config_saved]() {
        Serial.println("Save callback triggered - config saved!");
        config_saved = true;
    });

    // Load current config
    ConfigParams config = load_config();
    
    // Setup parameters
    WiFiManagerParameter coin1_param, coin2_param, coin3_param;
    WiFiManagerParameter timezone_param, sounds_param, simple_param, graph_param;
    
    setup_wm_parameters(wm, config, coin1_param, coin2_param, coin3_param, timezone_param,
                       sounds_param, simple_param, graph_param);

    // Start portal
    bool success = wm.startConfigPortal("crypto_display", "crypto123");
    
    // Save config
    ConfigParams new_config = get_wm_values(coin1_param, coin2_param, coin3_param, timezone_param,
                                           sounds_param, simple_param, graph_param);
    
    if (!new_config.coin1.isEmpty()) {
        save_config(new_config);
    }
    
    return success || config_saved;
}

bool check_wifi_connection() {
    if (wifi_connected()) {
        return true;
    }

    display_message("Reconnecting wifi...");
    WiFi.reconnect();

    int attempts = 0;
    while (WiFi.status() != WL_CONNECTED && attempts < 50) {
        delay(1000);
        Serial.print(".");
        attempts++;
    }
    
    Serial.println();
    
    if (WiFi.status() == WL_CONNECTED) {
        Serial.println("WiFi reconnected successfully!");
        Serial.print("IP address: ");
        Serial.println(WiFi.localIP());
        return true;
    }
    
    Serial.println("WiFi reconnection failed");
    return false;
}

bool wifi_connected() {
    return WiFi.status() == WL_CONNECTED;
}