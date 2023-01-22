#include <FastLED.h>
#define WIDTH 36
#define HEIGHT 16
#define LED_PIN 2

const int NUM_LEDS = WIDTH * HEIGHT;

CRGB leds[NUM_LEDS];

void setup() 
{
  FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS);
  Serial.begin(115200);
  leds[0].r = 100;
  FastLED.show();
}

void loop() 
{
  switch(Serial.read()) //read header from LMCSDH
  {
    case 0x05: //request for matrix definition
      Serial.println(WIDTH);
      Serial.println(HEIGHT);
      break;

    case 0x41: //frame data
      //Serial.readBytes((char*)leds, NUM_LEDS * 3);
      for (int i = 0; i < NUM_LEDS; i++){
        leds[i].r = Serial.read();
        leds[i].g = Serial.read();
        leds[i].b = Serial.read();
      }
      FastLED.show();
      Serial.write(0x06); //acknowledge
      break;
  }
}
