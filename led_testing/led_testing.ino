#include <Adafruit_NeoPixel.h>
#define PIN        8

Adafruit_NeoPixel strip = Adafruit_NeoPixel(500, PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  // put your setup code here, to run once:
  strip.begin();
  strip.show();
}

void loop() {
  for (int i = 0; i < 500; i++) {
    strip.setPixelColor(i, strip.Color(0, 0, 0));
  }
  strip.show();
}
