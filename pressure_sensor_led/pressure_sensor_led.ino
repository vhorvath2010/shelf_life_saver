// Delay in ms before turning light on
const unsigned long LED_DELAY = 10000;

// Place indicator length in ms
const unsigned long PLACED_IND_DUR = 1000;

// Sensitivity of pressure sensor (1000 is max)
const unsigned int PRESSURE_SENSITIVITY = 1000;

const unsigned int LED_OUT = 11;
const unsigned int PRESSURE_SENSOR_IN = A0;

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
  // check if pressure is in
  int pressure_v = analogRead(PRESSURE_SENSOR_IN);

  // Pressure sensor logic
  bool next_pressed = pressure_v > PRESSURE_SENSITIVITY;
  if (next_pressed) {
    digitalWrite(LED_OUT, HIGH);
  } else {
    digitalWrite(LED_OUT, LOW);
  }
  pressed = next_pressed;
}
