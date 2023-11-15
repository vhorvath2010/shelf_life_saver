#include <Adafruit_NeoPixel.h>

// Delay in ms before turning light on
const unsigned long LED_DELAY = 5000;

// Place indicator length in ms
const unsigned long PLACED_IND_DUR = 1000;

// PIN for addressable LED strip
const unsigned int LED_OUT = 12;
const unsigned int NUM_LEDS = 14; // Change this based on number of LED pixels
Adafruit_NeoPixel strip(NUM_LEDS, LED_OUT);

// const values for strip colors
const uint32_t TIMER_UP_COLOR = strip.Color(0, 0, 255);
const uint32_t PLACED_COLOR = strip.Color(0, 255, 0);

// Switch/Pressure sensor input
const unsigned int SWITCH_IN = 7;

// Keep track of when an object was last placed (not pressed -> pressed)
bool pressed = false;
unsigned long last_placed = 0;

void setup() {
  // setup pins
  pinMode(SWITCH_IN, INPUT);

  // init LEDs to off
  strip.begin();
  strip.show();

  last_placed = millis();

  Serial.begin(9600);
}

void loop() {
  // update placements
  update_last_placed();

  // Light up if placed recently or too long ago
  unsigned int curr = millis();
  unsigned int time_since_placed = curr - last_placed;
  if (pressed && (time_since_placed < PLACED_IND_DUR || time_since_placed > LED_DELAY)) {
    if (time_since_placed < PLACED_IND_DUR) {
      strip.fill(PLACED_COLOR);
    } else if (time_since_placed > LED_DELAY) {
      strip.fill(TIMER_UP_COLOR);
    }
  } else {
    strip.clear();
  }
  strip.show();
}

void update_last_placed() {
  int switch_input = digitalRead(SWITCH_IN);
  Serial.println(switch_input);

  // If pressed check for new place and track pressed
  if (switch_input == HIGH) {
    if (!pressed) {
      last_placed = millis();
    }
    pressed = true;
  } else {
    pressed = false;
  }
}