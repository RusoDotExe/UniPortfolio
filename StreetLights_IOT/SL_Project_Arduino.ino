#include <TinyGPSPlus.h>
#include <SoftwareSerial.h>
#include <ArduinoJson.h>
#include "FS.h"
#include "SD.h"
#include "SPI.h"

// Choose two Arduino pins to use for software serial
int RXPin = 18; //arduino Pin 2 //// ESP32 Pin 16
int TXPin = 19; //Arduino Pin 3 //// ESP32 Pin 17

#define LIGHT_SENSOR_PIN  3  // ESP32 pin GIOP36 (ADC0) connected to light sensor
//light values
int light = 0;
bool dummy = false;
bool print = true;

int GPSBaud = 9600;

double lat = 0;
double lng = 0;

long int ID = 0;

bool send = false;

// Create a TinyGPS++ object
TinyGPSPlus gps;
char latChar[sizeof(long double)];
char lngChar[sizeof(long double)];
// Create a software serial port called "gpsSerial"
SoftwareSerial gpsSerial(RXPin, TXPin);

//JsonCreation
StaticJsonDocument<256> doc;

void setup()
{
  // Start the Arduino hardware serial port at 9600 baud
  Serial.begin(9600);

  // Start the software serial port at the GPS's default baud
  gpsSerial.begin(GPSBaud);

  //set up the sd card
  SD.begin(7);
  if(!SD.begin()){
        Serial.println("Card Mount Failed");
        return;
    }
    uint8_t cardType = SD.cardType();

    if(cardType == CARD_NONE){
        Serial.println("No SD card attached");
        return;
    }
    
    uint64_t cardSize = SD.cardSize() / (1024 * 1024);
    Serial.printf("SD Card Size: %lluMB\n", cardSize);
    Serial.print("SD Card Type: ");

    if(cardType == CARD_MMC){
        Serial.println("MMC");
    } else if(cardType == CARD_SD){
        Serial.println("SDSC");
    } else if(cardType == CARD_SDHC){
        Serial.println("SDHC");
    } else {
        Serial.println("UNKNOWN");
    }
}

void loop()
{
  // This sketch displays information every time a new sentence is correctly encoded.
  while (gpsSerial.available() > 0)
    if (gps.encode(gpsSerial.read()))
    {
      getGPSInfo();
      int analogValue = analogRead(LIGHT_SENSOR_PIN); // read the value on analog pin
      if(send)
      {
      //transform the data from double to char array
      dtostrf(lat, 6,6, latChar);
      dtostrf(lng, 6,6, lngChar);

      //fill it in a Json file
      JsonObject Data = doc.to<JsonObject>();
      char charID = ID;

      Data["name"] = charID;
      Data["lat"] = lat;
      Data["lon"] = lng;
      Data["lValue"] = analogValue;

      if(analogValue > 3000)
      light = 1;
      else if(analogValue < 1500)
      light = 0;
      Data["light"] = light;

      ID = ID + 1;
      char buffer [256];

      serializeJson(doc, buffer);
      appendFile(SD, "/lightData.txt", buffer);
      appendFile(SD, "/lightData.txt", ",");

      // Serial.println(latChar);
      // Serial.println(lngChar);
      Serial.print("recorded data with id: ");
      Serial.println(ID);
      //shows the information from gps to the serial monitor
      displayInfo();
      }
    }

  // If 5000 milliseconds pass and there are no characters coming in
  // over the software serial port, show a "No GPS detected" error
  if (millis() > 5000 && gps.charsProcessed() < 10)
  {
    Serial.println("No GPS detected");
    while(true);
  }
}

void appendFile(fs::FS &fs, const char * path, const char * message){
    Serial.printf("Appending to file: %s\n", path);

    File file = fs.open(path, FILE_APPEND);
    if(!file){
        Serial.println("Failed to open file for appending");
        return;
    }
    if(file.print(message)){
        Serial.println("Message appended");
    } else {
        Serial.println("Append failed");
    }
    file.close();
}

void getGPSInfo()
{
  if (gps.location.isValid())
  {
    lat = gps.location.lat();
    lng = gps.location.lng();
    send = true;
  }
  else
  {
    Serial.println("Location: Not Available");
    send = false;
  }
  delay(3000);
}

void displayInfo()
{
  if (gps.location.isValid())
  {
    Serial.print("Latitude: ");
    Serial.println(latChar);
    // Serial.println(gps.location.lat(), 6);
    Serial.print("Longitude: ");
    Serial.println(lngChar);
    // Serial.println(gps.location.lng(), 6);
    // Serial.print("Altitude: ");    
  }
  else
  {
    Serial.println("Location: Not Available");
  }
  
  Serial.println();
  delay(1000);
}