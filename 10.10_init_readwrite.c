////////////////////////////////////
//Included: Wire, LCD, Keypad, DHT//
////////////////////////////////////
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
#include "DHT.h"


////////////////
//Defining LCD//
////////////////
LiquidCrystal_I2C lcd(0x27, 16, 2);



/////////////////
//DHT 22 sensor//
/////////////////
#define DHTPIN 12
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);



////////////////////////
//Soil moisture sensor//
////////////////////////
int blueLED = 11;
int redLED = 10;
int sInput = 0;
int value = 0;



//////////////////////////////////////////////////////////////////////////////////////
//Keypad (defining number of rows and collumns, defining key meaning, defining pins)//
//////////////////////////////////////////////////////////////////////////////////////
const byte ROWS = 4;
const byte COLS = 4;
char hexaKeys[ROWS][COLS] = {
  {'B', 'U', 'U', 'A'},
  {'B', 'U', 'U', 'A'},
  {'B', 'D', 'D', 'A'},
  {'B', 'D', 'D', 'A'}
};
byte rowPins[ROWS] = {9, 8, 7, 6};
byte colPins[COLS] = {5, 4, 3, 2};

Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);


/////////////
//Variables//
/////////////
//screens
bool welcome = true;
bool mainScreen = true;
//int variables
int choice = 1;
float tempCels = 0;
float tempFar = 0;
float humidity = 0;



////////////////////////////////////////
//All methods that are written in code//
////////////////////////////////////////
void startScreen();
int choiceNumber(int choice);
void mainScreenP(int choice);
bool mainScreenCS(bool mainScreen);
bool mainScreenBack(bool mainScreen);
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
void printValue(int value);



/////////////////////////////////////////////////////
//Setting up all of the things that is used in code//
/////////////////////////////////////////////////////
void setup() {
  Serial.begin(96000);
  //setting up LCD
  lcd.backlight();
  lcd.init();
  //setting up DHT sensor
  dht.begin();
  //setting up diodes
  pinMode(redLED, OUTPUT);
  pinMode(blueLED, OUTPUT);
}


////////////////////////
//MAIN PART OF PROJECT//
////////////////////////
void loop() {
  readSensorData();
  //Starting arduino
  if (welcome == true) {
    startScreen();
    welcome = false;
    lcd.clear();
  }
  //Project
  else {
    if (mainScreen == true) {

      digitalWrite(redLED, LOW);
      digitalWrite(blueLED, LOW);
      lcd.clear();
      lcd.setCursor(0, 0);
      choice = choiceNumber(choice);
      mainScreenP(choice);
      mainScreen = mainScreenCS(mainScreen);

      delay(100);

    }
    if (mainScreen == false) {
      mainScreen = mainScreenBack(mainScreen);
      lcd.clear();
      if (choice == 1) {
        readSensorData();
        tempCelsius();
      }
      else if (choice == 2) {
        tempFarenheit();
      }
      else if (choice == 3) {
        printHumidity();
      }
      else if (choice == 4) {
        mainScreen = mainScreenBack(mainScreen);
        soilMoisture();
      }
      delay(100);
    }
  }
}




///////////////////////////
//METHODS USED IN PROJECT//
///////////////////////////
//start screen writing
void startScreen() {
  lcd.setCursor(0, 0);
  lcd.print("DHT22 AND SOIL");
  lcd.setCursor(0, 1);
  lcd.print("SENSOR PROJECT");
  delay(3000);
}


//choice number
int choiceNumber(int choice) {
  char customKey = customKeypad.getKey();
  if (choice == 1 && customKey == 'D') {
    choice = 2;
  }
  else if (choice == 1 && customKey == 'U') {
    choice = 4;
  }
  else if (choice == 2 && customKey == 'D') {
    choice = 3;
  }
  else if (choice == 2 && customKey == 'U') {
    choice = 1;
  }
  else if (choice == 3 && customKey == 'D') {
    choice = 4;
  }
  else if (choice == 3 && customKey == 'U') {
    choice = 2;
  }
  else if (choice == 3 && customKey == 'D') {
    choice = 4;
  }
  else if (choice == 3 && customKey == 'U') {
    choice = 2;
  }
  else if (choice == 4 && customKey == 'D') {
    choice = 1;
  }
  else if (choice == 4 && customKey == 'U') {
    choice = 3;
  }
  else
  {
    choice = choice;
  }
  return choice;
}

//main screen writing
void mainScreenP(int choice) {
  if (choice == 1)
  {
    lcd.setCursor(0, 0);
    lcd.print(">Temperature C");
    lcd.setCursor(0, 1);
    lcd.print(" Temperature F");
  }
  else if (choice == 2) {
    lcd.setCursor(0, 0);
    lcd.print(">Temperature F");
    lcd.setCursor(0, 1);
    lcd.print(" Humidity");
  }
  else if (choice == 3) {
    lcd.setCursor(0, 0);
    lcd.print(">Humidity");
    lcd.setCursor(0, 1);
    lcd.print(" Soil moisture");
  }
  else
  {
    lcd.setCursor(0, 0);
    lcd.print(" Humidity");
    lcd.setCursor(0, 1);
    lcd.print(">Soil moisture");
  }
}
//Main screen status change
bool mainScreenCS(bool mainScreen) {
  char customKey2 = customKeypad.getKey();
  if (mainScreen == true && customKey2 == 'A') {
    mainScreen = false;
  }
  return mainScreen;
}
bool mainScreenBack(bool mainScreen) {
  char customKey3 = customKeypad.getKey();
  if (mainScreen == false && customKey3 == 'B') {
    mainScreen = true;
  }
  return mainScreen;
}



////////////////////////////////
//DHT sensor stuff and reading//
////////////////////////////////
void tempCelsius() {
  readSensorData();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(getTemperatureC());
  lcd.setCursor(0, 1);
  lcd.print(">Back");
}
void tempFarenheit() {
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
//DHT read sensor data
void readSensorData() {
  humidity = dht.readHumidity();
  tempCels = dht.readTemperature();
  tempFar = dht.readTemperature(true);
}
//DHT getTemperature C
String getTemperatureC() {
  String txt = "TempC:";
  txt.concat(String(tempCels, 0));
  txt.concat((char)223);
  txt.concat("C");
  return txt;
}
String getTemperatureF() {
  String txt = "TempF:";
  txt.concat(String(tempFar, 0));
  txt.concat((char)223);
  txt.concat("F");
  return txt;
}
String getHumidity() {
  return "Humidity:" + String(humidity, 0) + "%";
}


////////////////////////
//SOIL MOISTURE SENSOR//
////////////////////////
void soilMoisture() {
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
void printSensorSoilMoisture() {
  lcd.clear();
  mainScreen = mainScreenBack(mainScreen);
  lcd.setCursor(0, 0);
  lcd.print("Soil moisture:");
  lcd.setCursor(0, 1);
}
void diodeLights(int value) {
  if (value < 50) {
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
void printValue(int value) {
  lcd.clear();
  mainScreen = mainScreenBack(mainScreen);
  lcd.setCursor(0, 0);
  lcd.print("Sensor value:");
  lcd.print(value);
  lcd.setCursor(0, 1);
  lcd.print(">BACK");
  mainScreen = mainScreenBack(mainScreen);
  delay(2000);
  mainScreen = mainScreenBack(mainScreen);
  lcd.setCursor(0, 0);
  //this part tells user if he needs to water plant or not
  if (value < 50)
  {
    lcd.print("Water your plant");
  }
  else
  {
    lcd.clear();
    lcd.print("Plant is good");
  }
  lcd.setCursor(0, 1);
  lcd.print(">BACK");
  mainScreen = mainScreenBack(mainScreen);
}