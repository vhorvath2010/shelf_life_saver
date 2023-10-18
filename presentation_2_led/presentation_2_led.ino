// Delay in ms before turning light on
const unsigned long LED_DELAY = 1200000;

// Place indicator length in ms
const unsigned long PLACED_IND_DUR = 1000;

const unsigned int LED_OUT = 11;
const unsigned int SWITCH_IN = 6;

// Keep track of when an object was last placed (not pressed -> pressed)
bool pressed = false;
unsigned long last_placed = 0;

void setup() {
  // setup pins
  pinMode(LED_OUT, OUTPUT);
  // pressure_sensor_in is already setup as analog input pinMode(pressure_sensor_in, INPUT);

  last_placed = millis();
}

void loop() {
  // update placements
  update_last_placed();

  // Light up if placed recently or too long ago
  unsigned int curr = millis();
  unsigned int diff = curr - last_placed;
  if (pressed && (diff < PLACED_IND_DUR || diff > LED_DELAY)) {
    digitalWrite(LED_OUT, HIGH);
  } else {
    digitalWrite(LED_OUT, LOW);
  }
}

void update_last_placed() {
  int switch_input = digitalRead(SWITCH_IN);

  // If pressed check for new place and track pressed
  if (switch_input == 1) {
    if (!pressed) {
      last_placed = millis();
    }
    pressed = true;
  } else {
    pressed = false;
  }
}