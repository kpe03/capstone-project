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
#define temp_hum_delay1 5000  // Delay time for open/ close for out of range temp/ hum levels
#define temp_hum_delay2 3000


float TEMP_HIGH = 24.0;
float TEMP_LOW = 18.0;
float HUM_HIGH = 60.0;
float HUM_LOW = 30.0;
int AUTOMODE = 1;


// initialization
SoftwareSerial softwareSerial(RX_PIN, TX_PIN); 
Mhz19 sensor_co2;

DHT_Unified dht_in(DHTPIN_IN, DHTTYPE);
DHT_Unified dht_out(DHTPIN_OUT, DHTTYPE);

/**
   * Checks if humidity is above or below comfort thresholds.
   * @param hum The humidity value 
   * @return 1 if above HUM_HIGH, 0 if below HUM_HIGH.
   * @author Kazi Priom
   */
int checkHumComfortAlgorithm(float hum_in, float hum_out) {
      if ((hum_in > HUM_HIGH && hum_out < HUM_HIGH) || (hum_in < HUM_LOW && hum_out > HUM_LOW)) {
        Serial.println("Indoor humidity outside of comfort range");
        return 1;
      }
      else {
        return 0;
      }
    }

  /**
   * Checks if temperature is above or below comfort thresholds.
   * @param temp The temperature value 
   * @return 1 if above TEMP_HIGH, 0 if below TEMP_HIGH.
   * @author Kazi Priom
   */
  int checkTempComfortAlgorithm(float temp_in, float temp_out) {
    // Check for high outdoor temperature
    if ((temp_in > TEMP_HIGH && temp_out < TEMP_HIGH) || (temp_in < TEMP_LOW && temp_out > TEMP_LOW)) {
      Serial.println("Indoor temperature outside of comfort range");
      return 1;
    }
    else {
      return 0;
    }
  }

  /**
   * Checks if smoke level is above or below comfort thresholds.
   * @param smoke The smoke value 
   * @return 1 if above SMOKE_HIGH, 0 if below SMOKE_HIGH.
   * @author Kazi Priom
   */
  int checkSmokeComfortAlgorithm(int smoke) {
    if (smoke > 100) {
      Serial.println("High smoke levels detected");
      return 1;
    } else {
      return 0;
    }
  }

  /**
   * Checks if CO2 level is above or below comfort thresholds.
   * @param co2 The CO2 value
   * @return 1 if above CO2_HIGH, 0 if below CO2_HIGH.
   * @author Kazi Priom
   */
  int checkCO2ComfortAlgorithm(int co2) {
    if (co2 > 1000) {
      Serial.println("High CO2 levels detected");
      return 1;
    } else {
      return 0;
    }
  }

// Updating user set variables from the GUI
void updateVariables() {
  if (Serial.available()) {
    String command = Serial.readStringUntil('\n');
    int value = command.substring(command.indexOf('=') + 1).toInt();

    // check automode value first
    if (command.startsWith("AUTOMODE=")){
      AUTOMODE = value;
    } 

    //only update temp and humidity ranges if automode is off
    if(AUTOMODE == 0){
      if (command.startsWith("TEMP_HIGH=")) TEMP_HIGH = value;
      else if (command.startsWith("TEMP_LOW=")) TEMP_LOW = value;
      else if (command.startsWith("HUM_HIGH=")) HUM_HIGH = value;
      else if (command.startsWith("HUM_LOW=")) HUM_LOW = value;
    }
    else{ // if automode is on reset variables to default
      TEMP_HIGH = 24;
      TEMP_LOW = 18;
      HUM_HIGH = 60;
      HUM_LOW = 30;
    }
  }
}


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

  Serial.println("CLEARDATA"); //clears up any data left from previous projects
  Serial.println("LABEL,Time,T-in,H-in,T-out,H-out,CO2,Smoke"); //always write LABEL, to indicate it as first line
  Serial.println("RESETTIMER");
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
  int CO2 = sensor_co2.getCarbonDioxide();

  // Get smoke sensor reading in ppm
  int smoke = analogRead(MQ2PIN);


  updateVariables(); 
   

  int tempCheck = checkTempComfortAlgorithm(temp_in, temp_out);

  int humCheck = checkHumComfortAlgorithm(hum_in, hum_out);

  int CO2Check = checkCO2ComfortAlgorithm(CO2);

  int smokeCheck = checkSmokeComfortAlgorithm(smoke);
  

  // CO2 and smoke levels have first priority
  if(CO2Check == 1 || smokeCheck == 1){
    digitalWrite(outputPIN, HIGH);
    delay(smoke_CO2_delay);
    digitalWrite(outputPIN, LOW);
  }
  else if(tempCheck == 1 || humCheck == 1){ 
    digitalWrite(outputPIN, HIGH);
    // Check the difference between indoor hum/temp and desired values
    if(abs(TEMP_HIGH - temp_in) < 5 || abs(TEMP_LOW - temp_in) < 5 || abs(HUM_HIGH - hum_in) < 5 || abs(HUM_LOW - hum_in) < 5){
      delay(temp_hum_delay1); // if less than 5 degrees c or 5% hum, delay is longer
    }
    else delay(temp_hum_delay2); // if more than 5 degrees c or 5% hum, delay is shorter
    
    digitalWrite(outputPIN, LOW);
  }
  else {
    digitalWrite(outputPIN, LOW);
  }

  // Print sensor readings to serial output
  Serial.print("DATA,TIME,"); //always write "DATA" to indicate the following as data

  // Sensor data
  Serial.print(temp_in);
  Serial.print(",");
  Serial.print(hum_in);
  Serial.print(",");

  Serial.print(temp_out);
  Serial.print(",");
  Serial.print(hum_out);
  Serial.print(",");

  Serial.print(CO2);
  Serial.print(",");
  Serial.print(smoke);
  Serial.print(",");

  Serial.println(" ");

  delay(1000);
}

