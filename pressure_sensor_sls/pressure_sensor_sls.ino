#include <Adafruit_NeoPixel.h>

// Delay in ms before turning light on
const unsigned long LED_DELAY = 7000;

// Place indicator length in ms
const unsigned long PLACED_IND_DUR = 1000;

// PIN for addressable LED strip
const unsigned int LED_OUT = 12;
const unsigned int NUM_LEDS = 14; // Change this based on number of LED pixels
Adafruit_NeoPixel strip(NUM_LEDS, LED_OUT);

// const values for strip colors
const uint32_t TIMER_UP_COLOR = strip.Color(0, 0, 255);
const uint32_t PLACED_COLOR = strip.Color(0, 255, 0);
const uint32_t INTERM_COLOR = strip.Color(1, 1, 1);

// Switch/Pressure sensor input
const unsigned int PSR_PIN = A0;

// Keep track of when an object was last placed (not pressed -> pressed)
bool pressed = false;
unsigned long last_placed = 0; // time object was last placed (from millis())

// Track the sensor readings for the past SENSOR_INTERVALS to smooth noise
const unsigned int SENSOR_INTERVALS = 100;  // Based off 16MHz, so track last SENSOR_INTERVALS/16MHz seconds of data
boolean *sensor_readings;    // Circular array to track sensor readings
unsigned int curr_reading_pos = 0;            // Current position in the sensor readings circular array
unsigned int curr_reading_sum = 0;            // Current sum of the readings for the past SENSOR_INTERVALS timesteps
const unsigned int PRESSED_THRESHOLD = SENSOR_INTERVALS * 0.10; // Require X% of current readings to be true to count as pressed (lower if flickering too much)
                                                                // (raise if seemingly detecting nothing)

void setup() {
  Serial.begin(9600);

  // initialize sensor readings to 0
  sensor_readings = calloc(SENSOR_INTERVALS, sizeof(boolean));

  // Stop execution if no dynamic memory space for our sensor readings
  if (sensor_readings == NULL) {
    Serial.println("Unable to allocate space for sensor readings, exiting");
    exit(1);
  }

  // setup pins
  // needed if digital: pinMode(LED_IN, INPUT);

  // init LEDs to off
  strip.begin();
  strip.show();

  // Serial.print("Threshold: ");
  // Serial.println(PRESSED_THRESHOLD);

  last_placed = millis();
}

void loop() {
  // update placements
  update_last_placed();

  // Light up if placed recently or too long ago
  unsigned long curr = millis();
  unsigned long time_since_placed = curr - last_placed;
  // Serial.print("Time since placed: ");
  // Serial.print(time_since_placed);
  // Serial.println();
  if (pressed) {
    if (time_since_placed < PLACED_IND_DUR) {
      strip.fill(PLACED_COLOR);
    } else if (time_since_placed > LED_DELAY) {
      strip.fill(TIMER_UP_COLOR);
    } else {
      strip.clear();
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
  // Serial.print("Curr reading sum: ");
  // Serial.println(curr_reading_sum);
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
  long psr_reading =  analogRead(PSR_PIN);
  bool psr_input = psr_reading > 800;
  Serial.println(psr_reading);

  // Update current position with reading, and update sum accordingly
  if (sensor_readings[curr_reading_pos]) {
    curr_reading_sum--; // remove the previously counted "pressed" reading
  }
  sensor_readings[curr_reading_pos] = psr_input;
  // Add newly "pressed" reading if needed
  if (psr_input) {
    Serial.println("Pressure sensor down");
    curr_reading_sum++;
  }

  // move to next spot in circular array
  curr_reading_pos = (curr_reading_pos + 1) % SENSOR_INTERVALS;
}