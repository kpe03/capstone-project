// libraries used in adafruit example
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include <Arduino.h>
#include <Mhz19.h>
#include <SoftwareSerial.h>


#define DHTPIN 7     // Digital pin connected to the DHT sensor
#define DHTTYPE    DHT11

#define RX_PIN 3     //RX pin
#define TX_PIN 2     //TX pin
#define MQ2PIN A0


SoftwareSerial softwareSerial(RX_PIN, TX_PIN); 
Mhz19 sensor_co2;

DHT_Unified dht(DHTPIN, DHTTYPE);

#define outputPIN 4


void setup() {
  Serial.begin(9600);
  dht.begin();

  softwareSerial.begin(9600);

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

  sensor_t sensor;
  dht.temperature().getSensor(&sensor);
  pinMode(outputPIN, OUTPUT); // set pin 4 to OUTPUT mode
}

void loop() {
  // put your main code here, to run repeatedly:

  sensors_event_t event;
  dht.temperature().getEvent(&event);
  float temp = event.temperature;
  dht.humidity().getEvent(&event);
  float hum = event.relative_humidity;

  auto CO2 = sensor_co2.getCarbonDioxide();

  float smoke = analogRead(MQ2PIN);
  

  // if temp less than 70F send high signal to light
  if(CO2 > 4000 || smoke > 4000){ // CO2 and smoke take highest priority
    digitalWrite(outputPIN, HIGH);
  }
  else if (hum > 40) {
    digitalWrite(outputPIN, HIGH);
  }
  else {
    digitalWrite(outputPIN, LOW);
  }

  Serial.print("tem = "); Serial.println(temp,1);
  Serial.print("hum = "); Serial.println(hum,1);
  Serial.print("CO2 = "); Serial.println(CO2,1);
  Serial.print("Smoke = "); Serial.println(smoke,1);
  Serial.println("--------------");

  delay(2000);
}

