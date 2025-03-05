#include <Arduino.h>
#include <Mhz19.h>
#include <SoftwareSerial.h>

#define RX_PIN 3     //RX pin
#define TX_PIN 2     //TX pin

SoftwareSerial softwareSerial(RX_PIN, TX_PIN); 
Mhz19 sensor;

void setup() {
  Serial.begin(9600);
  softwareSerial.begin(9600);

  sensor.begin(&softwareSerial);
  sensor.setMeasuringRange(Mhz19MeasuringRange::Ppm_5000);
  sensor.enableAutoBaseCalibration();

  Serial.print("Preheating");  // Preheating, 3 minutes
  while (!sensor.isReady()) {
    Serial.print(".");
    delay(5000);
  }

  Serial.println("\nReady...");
}

void loop() {
  auto CO2 = sensor.getCarbonDioxide();
  if (CO2 >= 0) {
    Serial.println("CO2: " + String(CO2) + " ppm");
  }

  delay(2000);
}
