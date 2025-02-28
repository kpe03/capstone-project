// libraries used in adafruit example
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

#define DHTPIN 7     // Digital pin connected to the DHT sensor
#define DHTTYPE    DHT11

DHT_Unified dht(DHTPIN, DHTTYPE);

#define outputPIN 4


void setup() {
  Serial.begin(9600);
  dht.begin();

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

  // if temp less than 70F send high signal to light
  if (hum > 40) {
    digitalWrite(outputPIN, HIGH);
  }
  else {
    digitalWrite(outputPIN, LOW);
  }

  Serial.print("tem = "); Serial.println(temp,1);
  Serial.print("hum = "); Serial.println(hum,1);

  delay(2000);
}
