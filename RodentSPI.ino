//TODO: Some comments here which describe what this sketch does, what is expected to be connected and how
// is communications.


#include <NewPing.h>


// Pin Definitions:
#define ECHO_PIN     7  // Arduino pin tied to echo pin on the ultrasonic sensor.
#define TRIGGER_PIN  8  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define LED_PIN 12 // Connected to LED
#define POT_PIN A0 // Connected to POT
#define VOLTAGE_PIN A1 // Connected to battery voltage divider
#define BUTTON_PIN 10 // Connected to shutdown push button


#define MAX_DISTANCE 200 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.
#define BUTTON_HOLD_TIME 3000 // Hold push button for 3 seconds before issuing SHUTDOWN
#define SHUTDOWN_VOLTAGE  11.7   // Shutdown if voltage less than 11.7V
#define VOLTAGE_DIVIDER_FACTOR 4.0
#define VOLTAGE_READ_PERIOD 60000  // Update voltage every minute


NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);

int distance;
int threshold;
int pot_value;

long unsigned int next_voltage_read = 0;


void setup() {
  Serial.begin(9600);
  distance = 0;
  threshold = 50;
  pinMode(POT_PIN, INPUT);
  pinMode(VOLTAGE_PIN, INPUT );

  pinMode(LED_PIN, OUTPUT);
  digitalWrite (LED_PIN, LOW);

  pinMode(BUTTON_PIN, INPUT);
  digitalWrite(BUTTON_PIN, HIGH);

  readVoltage();
}

void loop() {


  delay(50);

  readVoltage();


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
      // Send "SHUTDOWN" message to the Pi
      Serial.println("SHUTDOWN");
      // Flash LED several times:
      for ( int i = 0; i < 10; i++ ) {
        digitalWrite(LED_PIN, HIGH);
        delay(500);
        digitalWrite(LED_PIN, LOW);
        delay(500);

      }
    }

  }
}

void readVoltage()
// Measure the voltage off pin A1 and pass it to Raspberry Pi. If it's less than
// the critical battery voltage then issue an orderly shutdown of the Pi.
{
  if ( next_voltage_read > millis() ) {
    return;
  }

  next_voltage_read = millis() + VOLTAGE_READ_PERIOD;

  int val = analogRead(VOLTAGE_PIN);
  double voltage = (double)val * 5.0 / 1023.0;
  voltage = voltage * VOLTAGE_DIVIDER_FACTOR;
  Serial.print( "VOLTAGE ");
  Serial.println( voltage, DEC );

  if ( voltage > 1.0 && voltage < SHUTDOWN_VOLTAGE ) {
    Serial.println("SHUTDOWN");
  }


}






