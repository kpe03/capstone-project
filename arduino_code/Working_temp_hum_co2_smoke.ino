// Code with temp, humidity, and CO2
// working with PLX DAQ
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

// Timing variables
unsigned long lastDataSent = 0;
unsigned long dataSendInterval = 1000;   // Send data every 1 second
unsigned long pauseDuration = 3000;      // Pause sending for 3 seconds
unsigned long cycleTime = 10000;         // Complete cycle time (active + pause)
bool serialActive = true;                // Flag to track if we're in active or pause mode


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

  //Set up CO2 sensor
  // Must warm up for 3 minutes before using:
  Serial.print("Preheating");  // Preheating, 3 minutes
  while (!sensor_co2.isReady()) {
    Serial.print(".");
    delay(5000);
  }

  //Set up Smoke sensor
  Serial.println("MQ2 warming up!");
  delay(20000); // allow the MQ2 to warm up

  Serial.println("\nReady...");

  sensor_t sensor;
  dht.temperature().getSensor(&sensor);
  pinMode(outputPIN, OUTPUT); // set pin 4 to OUTPUT mode

  Serial.println("CLEARDATA"); //clears up any data left from previous projects
  Serial.println("LABEL,Time,T-in,H-in,C02,Smoke"); //always write LABEL, to indicate it as first line
  Serial.println("RESETTIMER");

}

//Main code
void loop() {

  //to keep track of when to stop/start serial output
  unsigned long currentMillis = millis();
  unsigned long cyclePosition = currentMillis % cycleTime;

  //determine if we should be in active or pause mode
  serialActive = (cyclePosition < (cycleTime - pauseDuration));


  sensors_event_t event;
  dht.temperature().getEvent(&event);
  int temp = event.temperature;
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

  // Printing for testing values
  // Serial.print("tem = "); Serial.println(temp,1);
  // Serial.print("hum = "); Serial.println(hum,1);
  // Serial.print("CO2 = "); Serial.println(CO2,1);
  // Serial.print("Smoke = "); Serial.println(smoke,1);
  // Serial.println("--------------");

  /* * * * * 
  * PLX DAQ *
  * * * * * */
  //read data if serial output is active
   if (serialActive && (currentMillis - lastDataSent >= dataSendInterval)) {
      //prints data to an excel file called plx_co2
      Serial.print("DATA,TIME,"); //always write "DATA" to indicate the following as data

      // Sensor data
      Serial.print(temp);
      Serial.print(",");

      Serial.print(hum);
      Serial.print(",");

      Serial.print(CO2);
      Serial.print(",");

      Serial.println(smoke);
   }
  
  // STOP printing data to serial to allow python script to read data
  if (!serialActive && (cyclePosition == (cycleTime - pauseDuration))) {
    Serial.println("PAUSE_BEGIN");  //Indicator that Arduino is pausing output
    Serial.flush();                 //Ensure all data is sent before pausing
  }
  
  // Resume printing to serial 
  if (serialActive && (cyclePosition == 0)) {
    Serial.println("PAUSE_END");  //Indicator that Arduino is resuming output
  }
  delay(2000);
}

