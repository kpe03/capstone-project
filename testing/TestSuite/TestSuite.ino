/*
  Test cases:
    [1] TEST_DHT: Reads and verifies temperature and humidity from DHT11 sensor
    [2] TEST_CO2: Checks MH-Z19 sensor’s ability to report valid CO2 levels
    [3] TEST_MQ2: Ensures MQ-2 analog smoke sensor provides expected readings
    [4] TEST_OUTPUT: Confirms output toggles correctly based on sensor threshold logic
*/

#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include <Mhz19.h>
#include <SoftwareSerial.h>

// Pin definitions
#define DHTPIN     7      // Digital pin connected to the DHT sensor
#define DHTTYPE    DHT11  // Sensor model
#define RX_PIN     3      // RX pin
#define TX_PIN     2      // TX pin
#define MQ2PIN     A0     // MQ-2 pin
#define OUTPUT_PIN 4      // Output pin

// State machine for tests
enum TestStep {
  TEST_START,   // Setup
  TEST_DHT,     // Test temperature/humidity readings
  TEST_CO2,     // Test CO2 readings
  TEST_MQ2,     // Test smoke sensor
  TEST_OUTPUT,  // Test output logic
  TEST_DONE     // End
};

// Timing variables
unsigned long lastDataSent = 0;
unsigned long dataSendInterval = 1000;  // Print data every 1 second
unsigned long pauseDuration = 3000;     // 3 second pause
unsigned long cycleTime = 10000;        // Complete cycle time (active + pause)
bool serialActive = true;               // Flag to track if we're in active or pause mode

// Sensor objects
DHT_Unified dht(DHTPIN, DHTTYPE);
SoftwareSerial softSerial(RX_PIN, TX_PIN);
Mhz19 sensor_co2;

// Helper: Wait for user to press ENTER in serial monitor
void waitForUserInput(const char* message) {
  Serial.println(message);
  while (!Serial.available()) { /* Wait until user types something */ }
  // Clear buffer
  while (Serial.available()) {
    Serial.read();
  }
  Serial.println();
}

TestStep currentStep = TEST_START;

void setup() {
  Serial.begin(9600);
  pinMode(OUTPUT_PIN, OUTPUT);

  Serial.println("--------------------");
  Serial.println("  TEST SUITE START  ");
  Serial.println("--------------------");
  delay(1000);
  
  currentStep = TEST_DHT;
}


void loop() {
  switch (currentStep) {

    // --------------------------------------------
    // 1. TEST_DHT: Initialize and read DHT sensor
    // --------------------------------------------
    case TEST_DHT: {
      Serial.println("TEST 1: DHT11 (Temp/Humidity)");
      dht.begin();
      Serial.println("DHT11 initialized. Now taking readings...");

      for(int i = 1; i <= 3; i++){
        sensors_event_t event;
        dht.temperature().getEvent(&event);
        float tempC = event.temperature;

        dht.humidity().getEvent(&event);
        float hum = event.relative_humidity;

        Serial.print("  Reading #");
        Serial.print(i);
        Serial.print(": Temp(C) = ");
        Serial.print(tempC);
        Serial.print(", Hum(%) = ");
        Serial.println(hum);
        delay(2000);
      }

      waitForUserInput("[Press ENTER to continue to CO2 test]");
      currentStep = TEST_CO2;
    }
    break;

    // -----------------------------------------
    // 2. TEST_CO2: Warm up MH-Z19 --> Read CO2
    // -----------------------------------------
    case TEST_CO2: {
      Serial.println("TEST 2: MH-Z19 CO2 Sensor");
      softSerial.begin(9600);
      sensor_co2.begin(&softSerial);
      sensor_co2.setMeasuringRange(Mhz19MeasuringRange::Ppm_5000);
      sensor_co2.enableAutoBaseCalibration();

      Serial.print(F("Warming up..."));
      while (!sensor_co2.isReady()) {
        Serial.print(".");
        delay(5000);
      }
      Serial.println("\nSensor ready. Now taking readings...");

      for(int i = 1; i <= 3; i++) {
        int co2 = sensor_co2.getCarbonDioxide();
        Serial.print("  Reading #");
        Serial.print(i);
        Serial.print(": CO2(ppm) = ");
        Serial.println(co2);
        delay(2000);
      }

      waitForUserInput("[Press ENTER to continue to MQ-2 test]");
      currentStep = TEST_MQ2;
    }
    break;

    // ------------------------------------------------
    // 3. TEST_MQ2: Warm up MQ-2 --> Read smoke sensor
    // ------------------------------------------------
    case TEST_MQ2: {
      Serial.println("TEST 3: MQ-2 Smoke Sensor");
      Serial.println("Warming up...");
      delay(20000);
      Serial.println("Sensor warmup complete. Now taking readings...");

      for(int i = 1; i <= 3; i++) {
        int smokeVal = analogRead(MQ2PIN);
        Serial.print("  Reading #");
        Serial.print(i);
        Serial.print(": MQ-2 value = ");
        Serial.println(smokeVal);
        delay(2000);
      }

      waitForUserInput("[Press ENTER to continue to Output Pin test]");
      currentStep = TEST_OUTPUT;
    }
    break;

    // -----------------------------------
    // 4. TEST_OUTPUT: Check output logic
    // -----------------------------------
    case TEST_OUTPUT: {
      Serial.println("TEST 4: Output Logic (Pin 4)");
      Serial.println("Logic used:");
      Serial.println("  if CO2 > 4000 OR smoke > 4000 => HIGH");
      Serial.println("  else if humidity > 40 => HIGH");
      Serial.println("  else => LOW");
      Serial.println("Taking 5 readings to demonstrate...");

      for(int i = 1; i <= 5; i++) {
        // Read sensors
        sensors_event_t event;
        dht.temperature().getEvent(&event);
        float tempC = event.temperature;

        dht.humidity().getEvent(&event);
        float hum = event.relative_humidity;

        int co2 = sensor_co2.getCarbonDioxide();
        int smokeVal = analogRead(MQ2PIN);

        // Apply logic
        if (co2 > 4000 || smokeVal > 4000) {
          digitalWrite(OUTPUT_PIN, HIGH);
        } 
        else if (hum > 40) {
          digitalWrite(OUTPUT_PIN, HIGH);
        } 
        else {
          digitalWrite(OUTPUT_PIN, LOW);
        }

        // Print results
        Serial.print("  Reading #");
        Serial.print(i);
        Serial.print(": Temp = ");
        Serial.print(tempC);
        Serial.print("C, Hum = ");
        Serial.print(hum);
        Serial.print("%, CO2 = ");
        Serial.print(co2);
        Serial.print("ppm, Smoke = ");
        Serial.print(smokeVal);
        Serial.print(" => OUTPUT = ");
        Serial.println(digitalRead(OUTPUT_PIN) == HIGH ? "HIGH" : "LOW");
        delay(2000);
      }

      waitForUserInput("[Press ENTER to continue to Timing test]");
      currentStep = TEST_DONE;
    }
    break;

    // End of tests
    case TEST_DONE: {
      Serial.println("-----------------------");
      Serial.println("  ALL TESTS COMPLETED  ");
      Serial.println("-----------------------");
      
      while(true) { /* Halt here */ }
    }
    break;

  }
}
