/*
* This code connects with an Arduino to collect temperature and other data
* The components of this code: Sound, Light, Humidity, Temperature
*/
#include <Bonezegei_DS3231.h>
#include <BH1750.h>
#include <Wire.h>
#include <LCD_I2C.h>
#include <SPI.h>
#include <SD.h>
#include <DHT.h>

//Declarations
#define SENSOR_PIN A0 // Sound sensor
//Pin Assignments
#define DHTPIN2 2     // dht 22 sensor 1
#define DHTPIN1 3     // dht 22 sensor 2
#define DHTTYPE1 DHT11   // DHT 22
#define DHTTYPE2 DHT11   // DHT 22

const int chipSelect = 10; //SD card CS pin connected to pin 10 of Arduino
const int sampleWindow = 50;    // Sample window width in mS (50 mS = 20Hz)
unsigned int sample;

//time, LCD, and light objects
Bonezegei_DS3231 rtc(0x68);
LCD_I2C lcd(0x27, 16, 2); 
BH1750 lightMeter;
//LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 if 0x3F not working

//heat and temperature library
DHT dht1(DHTPIN1, DHTTYPE1);
DHT dht2(DHTPIN2, DHTTYPE2);

//Variables for temperature and humidity
int h1, t1;
int h2, t2;

// Set up arduino code
void setup() {

  pinMode (SENSOR_PIN, INPUT); // Set the signal pin as input  
  int error;
  int show=-1;

  delay(500); // do not alter, ensures the arduino code runs properly
  Serial.begin(9600);
  delay(500);

  //start temperature sensors
  dht1.begin();
  dht2.begin();

  // setup for the SD card
  Serial.print("Initializing SD card...");

  //error checking for SD card
  if(!SD.begin(chipSelect)) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");
    
  //open SD card file
  // File dataFile = SD.open("datalog.txt", FILE_WRITE);

  // if the file opened ok, write to it:
  if (dataFile) {
    Serial.println("File opened ok");
    // print the headings for our data
    dataFile.println("Date,Time,Light,T-in (C),H-in (%),T-out (C),H-out (%),Sound");
  }
  dataFile.close();

  //Print to PLX DAQ
  Serial.println("CLEARDATA"); //clears up any data left from previous projects
  Serial.println("LABEL,Date,Logging Time,Light,T-in (C),H-in (%),T-out (C),H-out (%),Sound"); //always write LABEL, to indicate it as first line
  Serial.println("RESETTIMER");

  //start recording time using rtc library

  //NOTE:  time needs to be MANUALLY set !!
  rtc.begin();
  rtc.setFormat(12);        //Set 12 Hours Format
  rtc.setAMPM(1);           //Set AM or PM    0 = AM  1 =PM
  rtc.setTime("7:06:00");  //Set Time    Hour:Minute:Seconds
  rtc.setDate("3/3/25");   //Set Date    Month/Date/Year

  Wire.begin();
  Wire.beginTransmission(0x27);  // set the LCD address to 0x27 if 0x3F not working
  error = Wire.endTransmission();
  // Serial.print("Error: ");
  // Serial.print(error);

  //check intiailized LCD
  if (error == 0) {
    Serial.println(": LCD found.");
    show = 0;
     lcd.begin();  // initialize the lcd
  } 
  else {
    Serial.println(": LCD not found.");
  } 

  lightMeter.begin();
}

/*
 * Main code segment
 */
void loop() {

  // Read data and store it to variables h (humidity) and t (temperature)
  // Reading temperature or humidity takes about 250 milliseconds!
  // But in this project we will read the sensors with 1 second interval.

 int lux = lightMeter.readLightLevel();

  h1 = dht1.readHumidity();
  t1 = dht1.readTemperature();
  h2 = dht2.readHumidity();
  t2 = dht2.readTemperature();

  // int ac_val = analogRead(ac);
  // int ac_percentage = map(ac_val, 0, 1023, 0, 100);//5 volt 10k registor

  
   unsigned long startMillis= millis();                   // Start of sample window
   float peakToPeak = 0;                                  // peak-to-peak level
 
   unsigned int signalMax = 0;                            //minimum value
   unsigned int signalMin = 1024;                         //maximum value
 
  //read microphone
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
   

  /* * * * * * * * * *
   * PRINT TO SD CARD *
   * * * * * * * * * */

  // File dataFile = SD.open("datalog.txt", FILE_WRITE);

  // // if the file is available, write to it:
  // if (dataFile) {

  //   //print to SD card --> date
  //   dataFile.print(twoDigit(rtc.getMonth()));
  //   dataFile.print("/");
  //   dataFile.print(twoDigit(rtc.getDate()));
  //   dataFile.print(","); //Move to next column using a ","

  
  //   String time = " " + String(rtc.getHour()) + ":" + String(twoDigit(rtc.getMinute())); 
  //   dataFile.print(time + ",");     // Close the text string
  //   dataFile.print(","); //Move to next column using a ","

  //   //print light
  //   dataFile.print(lux);
  //   dataFile.print(","); 

  //   //print in-temperature in celsius
  //   dataFile.print(t1); 
  //   dataFile.print(","); 

  //   //print in-humidity
  //   dataFile.print(h1); 
  //   dataFile.print(","); 

  //   //print out-temperature in celsius
  //   dataFile.print(t2); //Store date on SD card
  //   dataFile.print(","); //Move to next column using a ","

  //   //print out-humidity
  //   dataFile.print(h2); //Store date on SD card
  //   dataFile.print(","); //Move to next column using a ","

  //   //AC printing
  //   // dataFile.print(ac_percentage); //Store date on SD card
  //   // dataFile.print(","); //Move to next column using a ","

  //   //print sound level (db)
  //   dataFile.print(db); //Store date on SD card
  //   dataFile.print(","); //Move to next column using a ","


  //   dataFile.println(); //End of Row move to next row
  //   dataFile.close(); //Close the file
  // }
  // else {
  //   Serial.println("OOPS!! SD card writing failed");
  // }
    
  
  
  /* * * * * * * * * *
   * PRINT TO PLX DAQ *
   * * * * * * * * * */
  
  Serial.print("DATA,"); //always write "DATA" to Indicate the following as Data

    //print date
    Serial.print(twoDigit(rtc.getMonth()));
    Serial.print("/");
    Serial.print(twoDigit(rtc.getDate()));
    Serial.print(","); //Move to next column using a ","
 
    //print current time
    String time = " " + String(rtc.getHour()) + ":" + String(twoDigit(rtc.getMinute())); 
    Serial.print(time + ",");     // Close the text string

    //print light level
    Serial.print(lux); //Store date on Excel
    Serial.print(","); 

    //print in temperature
    Serial.print(t1); //Store date on Excel
    Serial.print(","); 
    
    //print in humidity
    Serial.print(h1); //Store date on Excel
    Serial.print(","); 

    //print out temperature
    Serial.print(t2); //Store date on Excel
    Serial.print(","); 

    //print out-humidity
    Serial.print(h2); //Store date on Excel
    Serial.print(","); 

    //print the sound level (db)
    Serial.print(db); 

    Serial.println(); //End of Row move to next row

  if (rtc.getTime()) {
      lcd.clear(); //Here after clearing the LCD we take the time from the module and print it on the screen with usual LCD functions
      lcd.backlight();
  }

  // set the LCD screen with the variables
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Ti");
  lcd.print(t1); //Showing temperature value in celsius

  lcd.print("H");
  lcd.print(h1); //Showing humidity value in celsius

   lcd.print("L");
  lcd.print(lux); //Showing light

  lcd.print(" S:");
  lcd.println(db); //Showing sound level
 

  lcd.setCursor(0,1);
  lcd.print("To:");
  lcd.print(t2); //Showing out temperature value in celsius
  lcd.print("C");
  lcd.print("Ho:");
  lcd.print(h2); //Showing out humidity value in celsius
  lcd.print("%");
  // // lcd.print("A");
  // // lcd.print(ac_percentage); //Showing temperature value in Farenheit
  // lcd.print("%");

  delay(3000);
}

/* a method to format digits and add a leading zero */
String twoDigit(int number) {
  if (number < 10) {  // If it's a single-digit number
    return "0" + String(number);  // Add a leading zero
  } else {
    return String(number);  // Otherwise, just return the number as is
  }
}