#include "buzzer.h"
#include "config.h"

void play_startup_sound() {  
  for (int i = 0; i < 5; i++) {
    tone(BUZZER_PIN, 150, 30); // Low
    delay(30);
    tone(BUZZER_PIN, 250, 30); // Mid-Low
    delay(30);
    tone(BUZZER_PIN, 180, 30); // Low-ish
    delay(30);
    
    noTone(BUZZER_PIN);
    delay(10);
  }

  for (int f = 600; f > 100; f -= 40) {
    tone(BUZZER_PIN, f);
    delay(15); // Very fast slide
  }

  tone(BUZZER_PIN, 80, 100); 
  delay(100);
  
  noTone(BUZZER_PIN);
}

void play_config_sound() {
    tone(BUZZER_PIN, 300, 100);
    delay(50);
    tone(BUZZER_PIN, 600, 50);

    noTone(BUZZER_PIN);
}