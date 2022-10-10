////////////////////////////////////
// Included: Wire, LCD, Keypad, DHT//
////////////////////////////////////
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
#include "DHT.h"
#include <PCD8544.h>

/////////////////
// DHT 22 sensor//
/////////////////
#define DHTPIN 12
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);
PCD8544 lcd;

////////////////////////
// Soil moisture sensor//
////////////////////////
int blueLED = 11;
int redLED = 10;
int sInput = 0;
int value = 0;

////////////////////////////////////////
// All methods that are written in code//
void tempCelsius();
void tempFarenheit();
void printHumidity();
void readSensorData();
String getTemperatureC();
String getTemperatureF();
String getHumidity();
void soilMoisture();
void printSensorSoilMoisture();
void diodeLights(int value);

/////////////////////////////////////////////////////
// Setting up all of the things that is used in code//
/////////////////////////////////////////////////////
void setup()
{
    Serial.begin(96000);
    // setting up DHT sensor
    dht.begin();
    lcd.begin(84, 48);
    // setting up diodes
    pinMode(redLED, OUTPUT);
    pinMode(blueLED, OUTPUT);
}

////////////////////////
// MAIN PART OF PROJECT//
////////////////////////

void loop()
{
    lcd.setCursor(0, 0);
    lcd.print("   WELCOME  ");
    lcd.setCursor(0, 1);
    lcd.print("     To");
    lcd.setCursor(0, 2);
    lcd.print("ElectronicsHobbyists.com");
    delay(200);
    readSensorData();
    // Project
    else
    {
        digitalWrite(redLED, LOW);
        digitalWrite(blueLED, LOW);
        delay(100);
    }
}
}

///////////////////////////
// METHODS USED IN PROJECT//
///////////////////////////

////////////////////////////////
// DHT sensor stuff and reading//
////////////////////////////////
void tempCelsius()
{
    readSensorData();
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(getTemperatureC());
    lcd.setCursor(0, 1);
    lcd.print(">Back");
}
void tempFarenheit()
{
    readSensorData();
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(getTemperatureF());
    lcd.setCursor(0, 1);
    lcd.print(">Back");
}
void printHumidity()
{
    readSensorData();
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(getHumidity());
    lcd.setCursor(0, 1);
    lcd.print(">Back");
}
// DHT read sensor data
void readSensorData()
{
    humidity = dht.readHumidity();
    tempCels = dht.readTemperature();
    tempFar = dht.readTemperature(true);
}
// DHT getTemperature C
String getTemperatureC()
{
    String txt = "TempC:";
    txt.concat(String(tempCels, 0));
    txt.concat((char)223);
    txt.concat("C");
    return txt;
}
String getTemperatureF()
{
    String txt = "TempF:";
    txt.concat(String(tempFar, 0));
    txt.concat((char)223);
    txt.concat("F");
    return txt;
}
String getHumidity()
{
    return "Humidity:" + String(humidity, 0) + "%";
}

////////////////////////
// SOIL MOISTURE SENSOR//
////////////////////////
void soilMoisture()
{
    lcd.clear();
    mainScreen = mainScreenBack(mainScreen);
    printSensorSoilMoisture();
    value = analogRead(sInput);
    value = constrain(value, 485, 1024);
    value = map(value, 485, 1024, 100, 0);

    diodeLights(value);
    delay(2000);
    printValue(value);
    delay(3000);
}
void printSensorSoilMoisture()
{
    lcd.clear();
    mainScreen = mainScreenBack(mainScreen);
    lcd.setCursor(0, 0);
    lcd.print("Soil moisture:");
    lcd.setCursor(0, 1);
}
void diodeLights(int value)
{
    if (value < 50)
    {
        digitalWrite(redLED, HIGH);
        digitalWrite(blueLED, LOW);
        lcd.print("NOT ENOUGH");
    }
    else
    {
        digitalWrite(blueLED, HIGH);
        digitalWrite(redLED, LOW);
        lcd.print("ENOUGH");
    }
}