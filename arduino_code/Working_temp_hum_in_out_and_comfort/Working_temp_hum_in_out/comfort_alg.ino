#define TEMP_HIGH 29   
#define TEMP_LOW  10   
#define HUM_HIGH 50
#define HUM_LOW 30


class CheckComfortAlgorithm{ 
int checkHumidityComfortAlgorith(int hum){
if (hum > HUM_HIGH) {
    Serial.println("High outdoor humidity detected, delay is now 1 second");
    //digitalWrite(outputPIN, HIGH);
    return 1;
  }
  // Check for low outdoor humidity
else{
    //digitalWrite(outputPIN, LOW);
    return 0; 
  }
}
int checkTempComfortAlgorithm(int temp){
    // Check for high outdoor temperature
  if (temp > TEMP_HIGH) {
    Serial.println("High outdoor temperature detected, delay is now 1 second");
    //digitalWrite(outputPIN, HIGH);
    return 1;
  }
  // Check for low outdoor temperature
  else{
    //digitalWrite(outputPIN, LOW);
    return 0;
  }
}

int checkSmokeComfortAlgorith(int smoke){
   if(smoke > 4000){ 
     Serial.println("High smoke levels detected, delay time is now 1 second");
  //   digitalWrite(outputPIN, HIGH);
    return 1;
   }
   else{
     //digitalWrite(outputPIN, LOW);
     return 0;
   }

  }

void checkCO2ComfortAlgorithm(int co2){
  if (co2 > 4000){
    Serial.println("High CO2 levels detected, delay time is now 1 second");
    //digitalWrite(outputPIN, HIGH);
    return 1;

  }
  else{
    //digitalWrite(outputPIN, LOW);
    return 0;
  }
}
}
}

