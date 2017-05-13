// Arduino sketch to control a Raspberry Pi, via Serial communication, which has an IR camera.
// The Arduino uses a pulsed IR beam break detector to know when to take the photo.

#include <IRremote.h>


// Pin Definitions:

#define IR_BEAM_PIN 3        // IR Beam LED
#define IR_BEAM_DETECT_PIN 2    // IR Beam Detector
#define LED_PIN 12 // Connected to LED
#define VOLTAGE_PIN A1 // Connected to battery voltage divider
#define BUTTON_PIN 10 // Connected to shutdown push button


#define BUTTON_HOLD_TIME 3000 // Hold push button for 3 seconds before issuing SHUTDOWN
#define SHUTDOWN_VOLTAGE  11.5   // Shutdown if voltage less than 11.5V
#define VOLTAGE_DIVIDER_FACTOR 4.0   // Resistor network is 30K/10K
#define VOLTAGE_READ_PERIOD 60000  // Update voltage every minute
#define TRIGGER_DELAY_TIME 30000  // Wait 30 seconds after triggering


IRsend irsend;

long unsigned int next_voltage_read = 0;


void setup() {
  Serial.begin(9600);

  pinMode(VOLTAGE_PIN, INPUT );

  pinMode(LED_PIN, OUTPUT);
  digitalWrite (LED_PIN, LOW);

  pinMode(BUTTON_PIN, INPUT);
  digitalWrite(BUTTON_PIN, HIGH);


  pinMode(IR_BEAM_DETECT_PIN, INPUT);
  irsend.enableIROut(38);
  irsend.mark(0);
  readVoltage();
}

void loop() {


  // Update the voltage every minute:
  readVoltage();

 
  // Pin goes HIGH when the beam is broken:
  if ( digitalRead(IR_BEAM_DETECT_PIN) == HIGH ) {
    Serial.println("Triggered with IR");
    triggerCamera();
  }



  // Check for push button presses:
  if ( digitalRead(BUTTON_PIN) == LOW ) {
    // Check if the "shutdown" pin has been pressed and held for three seconds:
    delay(BUTTON_HOLD_TIME);
    if ( digitalRead(BUTTON_PIN) == LOW ) {
      // Send "SHUTDOWN" message to the Pi
      Serial.println("SHUTDOWN");
      // Flash LED several times:
      for ( int i = 0; i < 30; i++ ) {
        digitalWrite(LED_PIN, HIGH);
        delay(50);
        digitalWrite(LED_PIN, LOW);
        delay(50);

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

void triggerCamera() {

  digitalWrite (LED_PIN, HIGH);
  Serial.println("TRIGGERED");
  delay(TRIGGER_DELAY_TIME);
  digitalWrite(LED_PIN, LOW);


}




