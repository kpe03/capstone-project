// NOTE this code is just for unit testing
// Uses AUnit library to create unit tests
// just upload and open serial monitor to see tests

#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include <Arduino.h>
#include <Mhz19.h>
#include <SoftwareSerial.h>
#include <AUnit.h>
using aunit::TestRunner;

#define DHTPIN 2     // Digital pin connected to the DHT sensor
#define DHTPIN2 3     // dht 22 sensor 2
#define DHTTYPE1 DHT11   // DHT 22
#define DHTTYPE2 DHT11   // DHT 22

//define temp thresholds
// outside temps in Celsius
#define TEMP_HIGH 29   
#define TEMP_LOW  10   
#define HUM_HIGH 50
#define HUM_LOW 30

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

//unit test for setDelayTime
test(setDelayTime) {
    // Test case: set a new delay time
  unsigned long testTime = 5000;  // 5 seconds
  setDelayTime(testTime);  // Call the function to set the value
  
  // Assert that the global delayTime has been updated correctly
  assertEqual(delayTime, testTime);  // Expect delayTime to be 5000
}

// comfort point algorithm
void checkComfortAlgorithm() {
  // Check for high outdoor humidity
  if (hum2 > HUM_HIGH) {
    Serial.println("High outdoor humidity detected, delay is now 1 second");
    setDelayTime(1000); // Set to one second
    Serial.println(delayTime);
    //digitalWrite(outputPIN, HIGH);
  }
  // Check for low outdoor humidity
  if (hum2 < HUM_LOW) {
    Serial.println("Low outdoor humidity detected, delay is now 3 seconds");
    setDelayTime(3000); // Set to three seconds
    Serial.println(delayTime);
     //digitalWrite(outputPIN, LOW);
  }
  // Check for high outdoor temperature
  if (temp2 > TEMP_HIGH) {
    Serial.println("High outdoor temperature detected, delay is now 1 second");
    setDelayTime(1000); // Set to one second
    Serial.println(delayTime);
    //digitalWrite(outputPIN, HIGH);
  }
  // Check for low outdoor temperature
  if (temp2 < TEMP_LOW) {
    Serial.println("Low outdoor temperature detected, delay is now 3 seconds");
    setDelayTime(3000); // Set to three seconds
    Serial.println(delayTime);
    //digitalWrite(outputPIN, LOW);
  }
  //smoke/co2 stuff
   // if(CO2 > 4000 || smoke > 4000){ // CO2 and smoke take highest priority
  //   digitalWrite(outputPIN, HIGH);
  // }
}

//unit tests for checkComfort Algorithm
test(highHumidityTest) {
  // Simulate high humidity condition
  hum2 = 80;  // > HUM_HIGH
  checkComfortAlgorithm();  // Call the function to check the behavior
  delay(10); 
  bool rightTime = delayTime == 1000;
  // Assert that the delay time is set to 1 second for high humidity
  assertTrue(rightTime);
}

test(lowHumidityTest) {
  // Simulate low humidity condition
  hum2 = 5;  // < HUM_LOW
  checkComfortAlgorithm();  // Call the function to check the behavior
  delay(10); 
  bool rightTime = delayTime == 3000;
  // Assert that the delay time is set to 3 seconds for low humidity
  assertTrue(rightTime);
}

test(highTemperatureTest) {
  // Simulate high temperature condition
  temp2 = 35;  // > TEMP_HIGH
  checkComfortAlgorithm();  // Call the function to check the behavior

  bool rightTime = delayTime == 1000;
  // Assert that the delay time is set to 1 second for high humidity
  assertTrue(rightTime);
}

test(lowTemperatureTest) {
  // Simulate low temperature condition
  temp2 = -5;  // < TEMP_LOW
  checkComfortAlgorithm();  // Call the function to check the behavior

  bool rightTime = delayTime == 3000;
  // Assert that the delay time is set to 3 seconds for low humidity
  assertTrue(rightTime);
}

//

//Set up sensors for arduino
void setup() {
  Serial.begin(9600);

//   dht.begin();
//   dht2.begin();

//   // softwareSerial.begin(9600);

//   sensor_t sensor;
//   dht.temperature().getSensor(&sensor);
//   //pinMode(outputPIN, OUTPUT); // set pin 4 to OUTPUT mode

//   Serial.println("CLEARDATA"); //clears up any data left from previous projects
//   Serial.println("LABEL,Time,T-in,H-in,T-out,H-out"); //always write LABEL, to indicate it as first line
//   Serial.println("RESETTIMER");
}

//Main code
void loop() {

  TestRunner::run();
//   //to keep track of when to stop/start serial output
//   unsigned long currentMillis = millis();
//   unsigned long cyclePosition = currentMillis % cycleTime;

//   //determine if we should be in active or pause mode
//   serialActive = (cyclePosition < (cycleTime - pauseDuration));


//   sensors_event_t event;

//   //indoor temp (pin 2)
//   dht.temperature().getEvent(&event);
//   temp1 = event.temperature;

//   //outdoor temp (pin 3)
//   dht2.temperature().getEvent(&event);
//   temp2 = event.temperature;

//   dht.humidity().getEvent(&event);
//   hum1 = event.temperature;

//   dht2.humidity().getEvent(&event);
//   hum2 = event.relative_humidity;

//   // auto CO2 = sensor_co2.getCarbonDioxide();
//   //float smoke = analogRead(MQ2PIN);

//   //run the comfort point algorithm
//   checkComfortAlgorithm();

//   /* * * * *  * * * *  * * * * 
//   * Print to serial monitor  *
//   * * * * * * * * * *  * * * */
//   //read data if serial output is active
//    if (serialActive && (currentMillis - lastDataSent >= dataSendInterval)) {
//       //prints data to an excel file called plx_co2
//       Serial.print("DATA,TIME,"); //always write "DATA" to indicate the following as data

//       // Sensor data
//       Serial.print(temp1);
//       Serial.print(",");
//       Serial.print(hum1);
//       Serial.print(",");

//       Serial.print(temp2);
//       Serial.print(",");
//       Serial.print(hum2);
//       Serial.print(",");

//       Serial.println(" ");
//    }
  
//   // STOP printing data to serial to allow python script to read data
//   if (!serialActive && (cyclePosition == (cycleTime - pauseDuration))) {
//     Serial.println("PAUSE_BEGIN");  //Indicator that Arduino is pausing output
//     Serial.flush();                 //Ensure all data is sent before pausing
//   }
  
//   // Resume printing to serial 
//   if (serialActive && (cyclePosition == 0)) {
//     Serial.println("PAUSE_END");  //Indicator that Arduino is resuming output
//   }

//   //delay of opening/closing the window
//   delay(delayTime);
}

