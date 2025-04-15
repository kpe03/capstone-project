#define TEMP_HIGH 29
#define TEMP_LOW 10
#define HUM_HIGH 50
#define HUM_LOW 30
#define CO2_HIGH 4000
#define SMOKE_HIGH 4000


#include <Arduino.h>

class CheckComfortAlgorithm {
  public:
    /**
     * Checks if humidity is above or below comfort thresholds.
     * @param hum The humidity value (e.g. from DHT sensor)
     * @return 1 if above HUM_HIGH, 0 if below HUM_HIGH
     * @author Kazi Priom
     */

    int checkHumComfortAlgorithm(int hum) {
      if (hum > HUM_HIGH) {
        Serial.println("High outdoor humidity detected, delay is now 1 second");
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
      Serial.println("High outdoor temperature detected, delay is now 1 second");
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
    if (smoke > SMOKE_HIGH) {
      Serial.println("High smoke levels detected, delay time is now 1 second");
      //   digitalWrite(outputPIN, HIGH);
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
    if (co2 > CO2_HIGH) {
      Serial.println("High CO2 levels detected, delay time is now 1 second");
      //digitalWrite(outputPIN, HIGH);
      return 1;

    } else {
      //digitalWrite(outputPIN, LOW);
      return 0;
    }
  }
};
