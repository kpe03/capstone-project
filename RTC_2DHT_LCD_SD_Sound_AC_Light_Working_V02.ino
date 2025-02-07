#include <Bonezegei_DS3231.h>
//#include <LiquidCrystal_I2C.h>
#include <BH1750.h>
#include <Wire.h>
BH1750 lightMeter;

int ac = A1;
int ac_percentage;

#include <SPI.h>
#include <SD.h>
const int chipSelect = 10; //SD card CS pin connected to pin 10 of Arduino

const int sampleWindow = 50;    // Sample window width in mS (50 mS = 20Hz)
unsigned int sample;
#define SENSOR_PIN A0 // Sound sensor

Bonezegei_DS3231 rtc(0x68);
#include <LCD_I2C.h>
LCD_I2C lcd(0x27, 16, 2); 
//LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 if 0x3F not working

//Libraries
#include <DHT.h>;

//Pin Assignments
#define DHTPIN2 2     // dht 22 sensor 1
#define DHTPIN1 3     // dht 22 sensor 2

//Declarations
#define DHTTYPE1 DHT11   // DHT 22
#define DHTTYPE2 DHT11   // DHT 22
DHT dht1(DHTPIN1, DHTTYPE1);
DHT dht2(DHTPIN2, DHTTYPE2);

//Variables
int h1, t1;
int h2, t2;

void setup() {
  pinMode (SENSOR_PIN, INPUT); // Set the signal pin as input  
  int error;
  int show=-1;
  delay(500); // Please do not alter these. these delays will make sure your arduino will run properly
  Serial.begin(9600);
  delay(500);
  dht1.begin();
  dht2.begin();
  Serial.println("2 Dht11 Sensors - 1 Arduino");
  Serial.println("Coded by Lanz Cimeni");

   // setup for the SD card
  Serial.print("Initializing SD card...");

  if(!SD.begin(chipSelect)) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");
    
  //open file
  File dataFile = SD.open("datalog.txt", FILE_WRITE);

  // if the file opened ok, write to it:
  if (dataFile) {
    Serial.println("File opened ok");
    // print the headings for our data
    dataFile.println("Date,Time,Light,T-in,H-in,T-out,H-out,AC%,Sound");
  }
 dataFile.close();

  Serial.println("CLEARDATA"); //clears up any data left from previous projects
  Serial.println("LABEL,Date,Logging Time,Light,T-in,H-in,T-out,H-out,AC%,Sound"); //always write LABEL, to indicate it as first line
  Serial.println("RESETTIMER");

  rtc.begin();
  //rtc.setFormat(12);        //Set 12 Hours Format
  //rtc.setAMPM(0);           //Set AM or PM    0 = AM  1 =PM
  //rtc.setTime("1:48:30");  //Set Time    Hour:Minute:Seconds
  //rtc.setDate("1/30/25");   //Set Date    Month/Date/Year

  Wire.begin();
  Wire.beginTransmission(0x27);  // set the LCD address to 0x27 if 0x3F not working
  error = Wire.endTransmission();
  Serial.print("Error: ");
  Serial.print(error);

  if (error == 0) {
    Serial.println(": LCD found.");
    show = 0;
     lcd.begin();  // initialize the lcd
    

  } else {
    Serial.println(": LCD not found.");
  } // if

lightMeter.begin();

Serial.println(F("BH1750 Test begin"));

}

void loop() {

    // Read data and store it to variables h (humidity) and t (temperature)
  // Reading temperature or humidity takes about 250 milliseconds!
  // But in this project we will read the sensors with 1 second interval.

 int lux = lightMeter.readLightLevel();

  h1 = dht1.readHumidity();
  t1 = dht1.readTemperature();
  h2 = dht2.readHumidity();
  t2 = dht2.readTemperature();

  int ac_val = analogRead(ac);
  int ac_percentage = map(ac_val, 0, 1023, 0, 100);//5 volt 10k registor

  
   unsigned long startMillis= millis();                   // Start of sample window
   float peakToPeak = 0;                                  // peak-to-peak level
 
   unsigned int signalMax = 0;                            //minimum value
   unsigned int signalMin = 1024;                         //maximum value
 
                                                          // collect data for 50 mS
   while (millis() - startMillis < sampleWindow)
   {
      sample = analogRead(SENSOR_PIN);                    //get reading from microphone
      if (sample < 1024)                                  // toss out spurious readings
      {
         if (sample > signalMax)
         {
            signalMax = sample;                           // save just the max levels
         }
         else if (sample < signalMin)
         {
            signalMin = sample;                           // save just the min levels
         }
      }
   }
 
   peakToPeak = signalMax - signalMin;                    // max - min = peak-peak amplitude
   int db = map(peakToPeak,20,900,45,90);               //calibrate for deciBels
   

  //Print temp and humidity values to serial monitor.



File dataFile = SD.open("datalog.txt", FILE_WRITE);

  // if the file is available, write to it:
  if (dataFile) {
    dataFile.print(twoDigit(rtc.getDate())); //Store date on SD card
    dataFile.print(","); //Move to next column using a ","

    dataFile.print(twoDigit(rtc.getHour())); //Store date on SD card
    dataFile.print(","); //Move to next column using a ","

    dataFile.print(lux); //Store date on SD card
    dataFile.print(","); //Move to next column using a ","

    dataFile.print(t1); //Store date on SD card
    dataFile.print(","); //Move to next column using a ","

    dataFile.print(h1); //Store date on SD card
    dataFile.print(","); //Move to next column using a ","

    
    dataFile.print(t2); //Store date on SD card
    dataFile.print(","); //Move to next column using a ","

    dataFile.print(h2); //Store date on SD card
    dataFile.print(","); //Move to next column using a ","

    dataFile.print(ac_percentage); //Store date on SD card
    dataFile.print(","); //Move to next column using a ","

    dataFile.print(db); //Store date on SD card
    dataFile.print(","); //Move to next column using a ","


    dataFile.println(); //End of Row move to next row
    dataFile.close(); //Close the file
  }
  else
  Serial.println("OOPS!! SD card writing failed");
  
    //write to plx-dax
    
  Serial.print("DATA, TIMER,"); //always write "DATA" to Indicate the following as Data



    Serial.print(lux); //Store date on Excel
    Serial.print(","); //Move to next column using a ","

    Serial.print(t1); //Store date on Excel
    Serial.print(","); //Move to next column using a ","

    
    Serial.print(h1); //Store date on Excel
    Serial.print(","); //Move to next column using a ","

    Serial.print(t2); //Store date on Excel
    Serial.print(","); //Move to next column using a ","

    Serial.print(h2); //Store date on Excel
    Serial.print(","); //Move to next column using a ","

    Serial.print(ac_percentage); //Store date on SD card
    Serial.print(","); //Move to next column using a ","

    Serial.print(db); //Store date on SD card
    Serial.print(","); //Move to next column using a ","




    Serial.println(); //End of Row move to next row

  if (rtc.getTime()) {
      lcd.clear(); //Here after clearing the LCD we take the time from the module and print it on the screen with usual LCD functions
      lcd.backlight();
        


  }
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Ti");
  lcd.print(t1); //Showing temperature value in Farenheit

  lcd.print("H");
  lcd.print(h1); //Showing temperature value in Farenheit

   lcd.print("L");
  lcd.print(lux); //Showing temperature value in Farenheit

  lcd.print(" S:");
  lcd.println(db); //Showing humidity percentage
 

 lcd.setCursor(0,1);
  lcd.print("To:");
  lcd.print(t2); //Showing temperature value in Farenheit
  lcd.print("C");
  lcd.print("Ho:");
  lcd.print(h2); //Showing temperature value in Farenheit
  lcd.print("%");
  lcd.print("A");
  lcd.print(ac_percentage); //Showing temperature value in Farenheit
  lcd.print("%");


 
  


  delay(1000);
}

String twoDigit(int number) {
  if (number < 10) {  // If it's a single-digit number
    return "0" + String(number);  // Add a leading zero
  } else {
    return String(number);  // Otherwise, just return the number as is
  }
}