//TODO: Some comments here which describe what this sketch does, what is expected to be connected and how
// is communications.


#include <NewPing.h>

#define TRIGGER_PIN  8  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN     7  // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE 200 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.
#define LED_PIN 12 // Connected to LED
#define POT_PIN A0 // Connected to POT
#define BUTTON_PIN 10 // Connected to shutdown push button
#define BUTTON_HOLD_TIME 3000 // Hold push button for 3 seconds before issuing SHUTDOWN


// TODO #define a delay time after triggering the camera and use below
// TODO #define for the "TRIGGERED" string


NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);

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

  pinMode(BUTTON_PIN, INPUT);
  digitalWrite(BUTTON_PIN, HIGH);
}

void loop() {

  delay(50);
  pot_value = analogRead(POT_PIN);
  threshold = map (pot_value, 0, 1023, 10, 150);

  distance = sonar.ping_cm();
  if ( distance > 0 && distance < threshold) {
    digitalWrite (LED_PIN, HIGH);
    Serial.println("TRIGGERED");
    delay(2000);
    digitalWrite(LED_PIN, LOW);
  }

  // Check if the "shutdown" pin has been pressed and held for three seconds:
  if ( digitalRead(BUTTON_PIN) == LOW ) {
    delay(BUTTON_HOLD_TIME);
    if ( digitalRead(BUTTON_PIN) == LOW ) {
      Serial.println("SHUTDOWN");
      for ( int i = 0; i < 10; i++ ) {
        digitalWrite(LED_PIN, HIGH);
        delay(500);
        digitalWrite(LED_PIN, LOW);
        delay(500);

      }
    }

  }

}

