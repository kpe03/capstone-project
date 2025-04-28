// libraries used in adafruit example
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include <Arduino.h>
#include <Mhz19.h>
#include <SoftwareSerial.h>


int TEMP_HIGH = 29;
int TEMP_LOW = 10;
int HUM_HIGH = 60;
int HUM_LOW = 30;
int AUTOMODE = 1;

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

int checkHumComfortAlgorithm(int hum) {
      if (hum > HUM_HIGH) {
        Serial.println("High outdoor humidity detected");
        //digitalWrite(outputPIN, HIGH);
        return 1;
      }
      // Check for low outdoor humidity
      else {
        //digitalWrite(outputPIN, LOW);
        return 0;
      }
    }

  /**
   * Checks if temperature is above or below comfort thresholds.
   * @param temp The temperature value 
   * @return 1 if above TEMP_HIGH, 0 if below TEMP_HIGH.
   * @author Kazi Priom
   */

  int checkTempComfortAlgorithm(int temp) {
    // Check for high outdoor temperature
    if (temp > TEMP_HIGH) {
      Serial.println("High outdoor temperature detected");
      //digitalWrite(outputPIN, HIGH);
      return 1;
    }
    // Check for low outdoor temperature
    else {
      //digitalWrite(outputPIN, LOW);
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
         //digitalWrite(outputPIN, HIGH);
      return 1;
    } else {
      //digitalWrite(outputPIN, LOW);
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
      //digitalWrite(outputPIN, HIGH);
      return 1;

    } else {
      //digitalWrite(outputPIN, LOW);
      return 0;
    }
  }


void checkAuto() {
  if (Serial.available()) {
    String command = Serial.readStringUntil('\n');
    int value = command.substring(command.indexOf('=') + 1).toInt();

    if (command.startsWith("AUTOMODE=")){
      AUTOMODE = value;
    } 
    
  }
}

void updateVariables() {
  if (Serial.available()) {
    String command = Serial.readStringUntil('\n');
    int value = command.substring(command.indexOf('=') + 1).toInt();

    if (command.startsWith("AUTOMODE=")){
      AUTOMODE = value;
      Serial.print(command + " " + AUTOMODE);
    } 

    if(AUTOMODE == 0){
      if (command.startsWith("TEMP_HIGH=")) TEMP_HIGH = value;
      else if (command.startsWith("TEMP_LOW=")) TEMP_LOW = value;
      else if (command.startsWith("HUM_HIGH=")) HUM_HIGH = value;
      else if (command.startsWith("HUM_LOW=")) HUM_LOW = value;
    }
    else{
      TEMP_HIGH = 29;
      TEMP_LOW = 10;
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

  /*
  Serial.print("Preheating");  // Preheating, 3 minutes
  while (!sensor_co2.isReady()) {
    Serial.print(".");
    delay(5000);

  }*/

  /*
  Serial.println("MQ2 warming up!");
  delay(20000); // allow the MQ2 to warm up*/

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
  //auto CO2 = sensor_co2.getCarbonDioxide();
  int CO2 = 0;

  // Get smoke sensor reading in ppm
  //float smoke = analogRead(MQ2PIN);
  int smoke = 0;  


  updateVariables(); 
   

  int tempCheck = checkTempComfortAlgorithm(temp_in);

  int humCheck = checkHumComfortAlgorithm(hum_in);

  int CO2Check = checkCO2ComfortAlgorithm(CO2);

  int smokeCheck = checkSmokeComfortAlgorithm(smoke);

  if(CO2Check == 1 || smokeCheck == 1){
    digitalWrite(outputPIN, HIGH);
    delay(smoke_CO2_delay);
    digitalWrite(outputPIN, LOW);
  }
  else if(tempCheck == 1 || humCheck == 1){
    digitalWrite(outputPIN, HIGH);
    delay(temp_hum_delay);
    digitalWrite(outputPIN, LOW);
  }
  else {
    digitalWrite(outputPIN, LOW);
  }

  // if temp less than 70F send high signal to light
  /*
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
  }*/

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

