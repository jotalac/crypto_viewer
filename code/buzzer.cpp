#include "buzzer.h"
#include "config.h"
#include "utils.h"

void play_startup_sound() {  
  if (get_play_sounds() != "T") return;

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
    if (get_play_sounds() != "T") return;

    tone(BUZZER_PIN, 300, 100);
    delay(50);
    tone(BUZZER_PIN, 600, 50);

    noTone(BUZZER_PIN);
}

void play_ath_sound() {
    if (get_play_sounds() != "T") return;

    tone(BUZZER_PIN, 523, 70); // C5 (Middle C)
    delay(70);
    
    tone(BUZZER_PIN, 659, 70); // E5 (Major 3rd)
    delay(70);
    
    tone(BUZZER_PIN, 784, 70); // G5 (Perfect 5th)
    delay(70);
    
    tone(BUZZER_PIN, 1047, 300); // C6 (Octave Up - The "Resolution")
    delay(300);

    noTone(BUZZER_PIN);
}