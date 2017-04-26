//TODO: Some comments here which describe what this sketch does, what is expected to be connected and how
// is communications.


#include <NewPing.h>

#define TRIGGER_PIN  8  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN     7  // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE 200 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.
#define LED_PIN 13 // Connected to LED
#define POT_PIN A0 // Connected to POT

// TODO #define a delay time after triggering the camera and use below
// TODO #define for the "TRIGGERED" string


NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);

// TODO comments which describe the variables:
int distance;
int threshold;
int pot_value;

void setup() {
  Serial.begin(9600);
  distance = 0;
  threshold = 50;
  pinMode(POT_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite (LED_PIN, LOW);
}

void loop() {
  
  delay(50);
  pot_value = analogRead(POT_PIN);
  threshold = map (pot_value, 0, 1023, 25, 100);

  distance = sonar.ping_cm();
  if ( distance > 0 && distance < threshold) {
    digitalWrite (LED_PIN, HIGH);
    Serial.print("TRIGGERED");
    delay(2000);
    digitalWrite(LED_PIN, LOW);
  }
}

