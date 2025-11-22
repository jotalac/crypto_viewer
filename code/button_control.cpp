#include "button_control.h"
#include "config.h"

//check if button is hold for 3 seconds
bool check_config_button() {
    static unsigned long button_press_start = 0;
    static bool button_was_pressed = false;
    
    bool button_pressed = (digitalRead(BUTTON_CONFIG_PIN) == LOW);
    
    if (button_pressed && !button_was_pressed) {
        // Button just pressed
        button_press_start = millis();
        button_was_pressed = true;
    } 
    else if (!button_pressed && button_was_pressed) {
        // Button released
        button_was_pressed = false;
        button_press_start = 0;
    }
    else if (button_pressed && button_was_pressed) {
        // Button is being held
        unsigned long hold_time = millis() - button_press_start;
        if (hold_time >= CONFIG_PRESS_TIME) {
            button_was_pressed = false;
            button_press_start = 0;
            return true;  // Button held long enough
        }
    }
    
    return false;
}

bool check_reset_button() {
    static unsigned long button_press_start = 0;
    static bool button_was_pressed = false;
    
    bool button_pressed = (digitalRead(BUTTON_CONFIG_PIN) == LOW);
    
    if (button_pressed && !button_was_pressed) {
        button_press_start = millis();
        button_was_pressed = true;
    } 
    else if (!button_pressed && button_was_pressed) {
        // Button released - check duration
        unsigned long hold_time = millis() - button_press_start;
        button_was_pressed = false;
        button_press_start = 0;
        
        // Short press (less than config time)
        if (hold_time >= BUTTON_SHORT_PRESS_TIME && hold_time < CONFIG_PRESS_TIME) {
            return true;
        }
    }
    
    return false;
}



bool check_coin_change_button() {
    static unsigned long button_press_start = 0;
    static bool button_was_pressed = false;
    
    bool button_pressed = (digitalRead(BUTTON_COIN_PIN) == LOW);
    
    if (button_pressed && !button_was_pressed) {
        button_press_start = millis();
        button_was_pressed = true;
    } 
    else if (!button_pressed && button_was_pressed) {
        // Button released - check duration
        unsigned long hold_time = millis() - button_press_start;
        button_was_pressed = false;
        button_press_start = 0;
        
        // Short press (less than config time)
        if (hold_time >= BUTTON_SHORT_PRESS_TIME && hold_time < CONFIG_PRESS_TIME) {
            return true;
        }
    }
    
    return false;
}

bool check_coin_fetch_button() {
    static unsigned long button_press_start = 0;
    static bool button_was_pressed = false;
    
    bool button_pressed = (digitalRead(BUTTON_COIN_PIN) == LOW);
    
    if (button_pressed && !button_was_pressed) {
        // Button just pressed
        button_press_start = millis();
        button_was_pressed = true;
    } 
    else if (!button_pressed && button_was_pressed) {
        // Button released
        button_was_pressed = false;
        button_press_start = 0;
    }
    else if (button_pressed && button_was_pressed) {
        // Button is being held
        unsigned long hold_time = millis() - button_press_start;
        if (hold_time >= CONFIG_PRESS_TIME) {
            button_was_pressed = false;
            button_press_start = 0;
            return true;  // Button held long enough
        }
    }
    
    return false;
}