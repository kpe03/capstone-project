// libraries used in adafruit example
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include <Arduino.h>
#include <Mhz19.h>
#include <SoftwareSerial.h>


#define DHTPIN_IN 7     // Digital pin used for indoor temperature/ humidity
#define DHTPIN_OUT 12   //Digital pin used for outdoor temperature/ humidity
#define DHTTYPE DHT11

#define RX_PIN 3     //RX pin for Arduino, connected to the TX pin of CO2 sensor
#define TX_PIN 2     //TX pin for Arduino, connected to the RX pin of CO2 sensor


#define MQ2PIN A0 // Analog pin for MQ2 smoke sensor

#define outputPIN 4 // Arduino pin being used to send output signal

#define smoke_CO2_delay 2000 // Delay time for open/ close for high smoke or CO2 levels
#define temp_hum_delay 5000  // Delay time for open/ close for out of range temp/ hum levels


// initialization
SoftwareSerial softwareSerial(RX_PIN, TX_PIN); 
Mhz19 sensor_co2;

DHT_Unified dht_in(DHTPIN_IN, DHTTYPE);
DHT_Unified dht_out(DHTPIN_OUT, DHTTYPE);





void setup() {
  Serial.begin(9600);

  dht_in.begin();
  dht_out.begin();

  softwareSerial.begin(9600);

  // initialize MHZ19B CO2 sensor using softwareSerial
  sensor_co2.begin(&softwareSerial);
  sensor_co2.setMeasuringRange(Mhz19MeasuringRange::Ppm_5000);
  sensor_co2.enableAutoBaseCalibration();

  Serial.print("Preheating");  // Preheating, 3 minutes
  while (!sensor_co2.isReady()) {
    Serial.print(".");
    delay(5000);
  }

  Serial.println("MQ2 warming up!");
  delay(20000); // allow the MQ2 to warm up

  Serial.println("\nReady...");

  sensor_t sensor_in;
  sensor_t sensor_out;

  dht_in.temperature().getSensor(&sensor_in);
  dht_out.temperature().getSensor(&sensor_out);

  pinMode(outputPIN, OUTPUT); // set pin 4 to OUTPUT mode
}

void loop() {

  // Get indoor temperature
  sensors_event_t event_in;
  dht_in.temperature().getEvent(&event_in);
  float temp_in = event_in.temperature;
  dht_in.humidity().getEvent(&event_in);
  float hum_in = event_in.relative_humidity;

  // Get outdoor temperature
  sensors_event_t event_out;
  dht_out.temperature().getEvent(&event_out);
  float temp_out = event_out.temperature;
  dht_out.humidity().getEvent(&event_out);
  float hum_out = event_out.relative_humidity;

  // Get CO2 level in ppm
  auto CO2 = sensor_co2.getCarbonDioxide();

  // Get smoke sensor reading in ppm
  float smoke = analogRead(MQ2PIN);
  

  // if temp less than 70F send high signal to light
  if(CO2 > 1000 || smoke > 100){ // CO2 and smoke take highest priority
    digitalWrite(outputPIN, HIGH);
    delay(smoke_CO2_delay);
    digitalWrite(outputPIN, LOW);
  }
  else if (hum_in > 60.0) {
    digitalWrite(outputPIN, HIGH);
    delay(temp_hum_delay);
    digitalWrite(outputPIN, LOW);
  }
  else {
    digitalWrite(outputPIN, LOW);
  }

  // Print sensor readings to serial output
  Serial.print("temp_in = "); Serial.println(temp_in,1);
  Serial.print("hum_in = "); Serial.println(hum_in,1);
  Serial.print("temp_out = "); Serial.println(temp_out,1);
  Serial.print("hum_out = "); Serial.println(hum_out,1);
  Serial.print("CO2 = "); Serial.println(CO2,1);
  Serial.print("Smoke = "); Serial.println(smoke,1);
  Serial.println("--------------");

  delay(1000);
}

