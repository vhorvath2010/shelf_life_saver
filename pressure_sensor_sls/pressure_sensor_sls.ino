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
unsigned long last_placed = 0; // time object was last placed (from millis())

// Track the sensor readings for the past SENSOR_INTERVALS to smooth noise
const unsigned int SENSOR_INTERVALS = 16000;  // Based off 16MHz, so track last 0.001 seconds of data
boolean *sensor_readings;    // Circular array to track sensor readings
unsigned int curr_reading_pos = 0;            // Current position in the sensor readings circular array
unsigned int curr_reading_sum = 0;            // Current sum of the readings for the past SENSOR_INTERVALS timesteps
const unsigned int PRESSED_THRESHOLD = SENSOR_INTERVALS * 0.50; // Require X% of current readings to be true to count as pressed (lower if flickering too much)
                                                                // (raise if seemingly detecting nothing)

void setup() {
  // initialize sensor readings to 0
  sensor_readings = calloc(SENSOR_INTERVALS, sizeof(boolean));

  // setup pins
  pinMode(SWITCH_IN, INPUT);

  // init LEDs to off
  strip.begin();
  strip.show();

  last_placed = millis();

  // Serial.begin(9600);
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
  update_pressed_count();
  
  // If pressed check for new place and track pressed
  // pressed means we've read more pressed signals than our threshold
  if (curr_reading_sum >= PRESSED_THRESHOLD) {
    if (!pressed) {
      last_placed = millis();
    }
    pressed = true;
  } else {
    pressed = false;
  }
}

void update_pressed_count() {
    int switch_input = digitalRead(SWITCH_IN);
    
    // Update current position with reading, and update sum accordingly
    if (sensor_readings[curr_reading_pos]) {
      curr_reading_sum--; // remove the previously counted "pressed" reading
    }
    sensor_readings[curr_reading_pos] = switch_input == HIGH;
    // Add newly "pressed" reading if needed
    if (switch_input == HIGH) {
      curr_reading_sum++;
    }

    // move to next spot in circular array
    curr_reading_pos = (curr_reading_pos + 1) % SENSOR_INTERVALS;
}