#include <FastLED.h>
#define WIDTH 36
#define HEIGHT 16
#define LED_PIN 2

const int NUM_LEDS = WIDTH * HEIGHT;
CRGB leds[NUM_LEDS];

void setup() {
  delay(3000); // Powerup delay
  FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS);
}

void loop() {
  for (int i = 0; i < NUM_LEDS; i++){
    leds[i] = CRGB::White;
    FastLED.show();
    delay(15);
  }
  for (int i = 0; i < NUM_LEDS; i++){
    leds[i] = CRGB::Black;
  }
  FastLED.show();
}
