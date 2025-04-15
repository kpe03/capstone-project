// Arduino code for sensors: temperature and humidity
// can add sound and light

// this code works with kaitlyn's arduino
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include <Arduino.h>
#include <Mhz19.h>
#include <SoftwareSerial.h>
#include "CheckComfortAlgorithm.cpp"

#define DHTPIN 2     // Digital pin connected to the DHT sensor
#define DHTPIN2 3     // dht 22 sensor 2
#define DHTTYPE1 DHT11   // DHT 22
#define DHTTYPE2 DHT11   // DHT 22

// Timing variables
unsigned long lastDataSent = 0;
unsigned long dataSendInterval = 1000;   // Send data every 1 second
unsigned long pauseDuration = 3000;      // Pause sending for 3 seconds
unsigned long cycleTime = 10000;         // Complete cycle time (active + pause)
bool serialActive = true;                // Flag to track if we're in active or pause mode

// -- delay variable --
// change depending on the weather status (for example: high humidity, shorter delay)
// by default: 2000 (~ 2 seconds)
unsigned long delayTime = 2000;

//sensor variables
int hum1, temp1;
int hum2, temp2;

//SoftwareSerial softwareSerial(RX_PIN, TX_PIN); 


DHT_Unified dht(DHTPIN, DHTTYPE1);
DHT_Unified dht2(DHTPIN2, DHTTYPE2);

// to set the time dynamically
void setDelayTime(unsigned long newTime) {
  delayTime = newTime;
}
//Set up sensors for arduino
void setup() {
  Serial.begin(9600);

  dht.begin();
  dht2.begin();

  // softwareSerial.begin(9600);

  sensor_t sensor;
  dht.temperature().getSensor(&sensor);
  //pinMode(outputPIN, OUTPUT); // set pin 4 to OUTPUT mode

  Serial.println("CLEARDATA"); //clears up any data left from previous projects
  Serial.println("LABEL,Time,T-in,H-in,T-out,H-out"); //always write LABEL, to indicate it as first line
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

  //indoor temp (pin 2)
  dht.temperature().getEvent(&event);
  temp1 = event.temperature;

  //outdoor temp (pin 3)
  dht2.temperature().getEvent(&event);
  temp2 = event.temperature;

  dht.humidity().getEvent(&event);
  hum1 = event.temperature;

  dht2.humidity().getEvent(&event);
  hum2 = event.relative_humidity;

  // auto CO2 = sensor_co2.getCarbonDioxide();
  //float smoke = analogRead(MQ2PIN);

  //run the comfort point algorithm
  CheckComfortAlgorithm checker;
  int tempCheck = checker.checkTempComfortAlgorithm(temp2);
  int humCheck = checker.checkHumComfortAlgorithm(hum1);


  if (tempCheck  == 1 || humCheck == 1){
    setDelayTime(1000);
  }
  else{
    setDelayTime(3000);
  }
    /* * * * *  * * * *  * * * * 
  * Print to serial monitor  *
  * * * * * * * * * *  * * * */
  //read data if serial output is active
   if (serialActive && (currentMillis - lastDataSent >= dataSendInterval)) {
      //prints data to an excel file called plx_co2
      Serial.print("DATA,TIME,"); //always write "DATA" to indicate the following as data

      // Sensor data
      Serial.print(temp1);
      Serial.print(",");
      Serial.print(hum1);
      Serial.print(",");

      Serial.print(temp2);
      Serial.print(",");
      Serial.print(hum2);
      Serial.print(",");

      Serial.println(" ");
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

  //delay of opening/closing the window
  delay(delayTime);
}

