////////////////////TO DOs////////////////////////////////////////////////
// IMPLEMENT THE READINGS INTO THE LCD /////////////////////////////////// DONE, STILL NEED TO CHECK IF IT ACTUALLY WORKS WHEN EVERYTHING IS CONNECTED
// OPTIMIZE MEMORY USAGE (MOSTLY FOR STRING ARRAYS) ////////////////////// WE'LL SEE 
// TEST WITH ALL OF THE PARTS ATTACHED TO CHECK IF 9V BATTERY IS ENOUGH // WE'LL SEE
//////////////////////////////////////////////////////////////////////////
//////CREATED BY TOMAS ŠUTAVIČIUS :))))))))/////////////////////////////////////////////
//////WITH HELP FROM RIM CRUTZEN, THOMAS ILLIG//////////////////////////////////////////
// REFERENCES : https://www.circuitbasics.com/how-to-setup-ps2-joysticks-on-the-arduino////
// https://www.electronics-lab.com/project/menu-nokia-5110-lcd-display-arduino/ ///////////
///////////////////////////////////////////////////////////////////////////////////////////

#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
#include <SPI.h>

#define ONE_WIRE_BUS 5

// Array that holds all the sizes of the arrays //
// Im doing this part manually to avoid a fuck ton of declarations that would use memory //
int amounts[5] = {4, 4, 3, 4, 1};

// Menu navigations ///////////
int menuitem = 1;
int submenu = 0;
int page = 1;
int lastPage = 1;

volatile boolean up = false;
volatile boolean down = false;
volatile boolean middle = false;
volatile boolean left = false;
////////////////////////////////

// Joystick pins //////
const int jx = A0;
const int jy = A1;
const int swPin = 12;
///////////////////////

// Joystick min, mid, max values //
const int jLow = 0;
const int jHigh = 1023;
const int jCenter = 607;
///////////////////////////////////

// Joystick movement tracking //
int downJoystickState = 0;
int upJoystickState = 0;  
int selectJoystickState = 0;
int leftJoystickState = 0;          
int lastDownJoystickState = 0;
int lastSelectJoystickState = 0;
int lastUpJoystickState = 0;
int lastLeftJoystickState = 0;
////////////////////////////////

//////SOIL TEMP CONSTANTS////////
const int t_dat = A2;
// OneWire oneWire(ONE_WIRE_BUS);
// DallasTemperature sensors_temp(&oneWire);

// float Celcius=0;
// float Fahrenheit=0;
/////////////////////////////////

/////SOIL MOISTURE CONSTANTS//////
const int m_pow = A5;
const int m_dat = A4;
static int MOISTURE_MIN = 234;
static int MOISTURE_MAX = 1008;
//////////////////////////////////

////////////////////////////LDR CONSTANTS//////////////////////////////////
#define R 10000 //ohm resistance value
const int ldrPin = A3; // Pin connected to sensor
// int lux; //Lux value
// int ldrValue = 0; // variable to store the value coming from the sensor
////////////////////////////////////////////////////////////////////////////

///// D/C, SCE, RST /////
Adafruit_PCD8544 display = Adafruit_PCD8544(8, 10, 6); /// <- THIS TAKES 30% OF DYNAMIC MEMORY ;-;
/////////////////////////

/// Function that converts LDR values to lumen ///
int sensorRawToPhys(int raw){
  // Conversion rule
  float Vout = float(raw) * (5 / float(1023));// Conversion analog to voltage
  float RLDR = (R * (5 - Vout))/Vout; // Conversion voltage to resistance
  int phys=500/(RLDR/1000); // Conversion resitance to lumen
  return phys;
}
//////////////////////////////////////////////////

void setup() {
  Serial.begin(9600);
 
  pinMode(swPin, INPUT_PULLUP); // Joystick button setup


  // LCD SETUP ///////////////////////////////
  pinMode(7, OUTPUT); //Set lcd to output
  digitalWrite(7,HIGH); //Turn Backlight ON
  display.begin();      
  display.setContrast(50); //Set contrast to 50
  display.clearDisplay(); 
  display.display();
  ////////////////////////////////////////////

  /////SOIL MOISTURE SETUP/////
  pinMode(m_pow, OUTPUT);
  pinMode(m_dat, INPUT);
  /////////////////////////////

  //////LDR SETUP/////////
  pinMode(ldrPin, INPUT);

  /////SOIL TEMP SETUP/////
  pinMode(t_dat, INPUT);
  /////////////////////////
}

void loop() {
  drawMenu();

  //READING THE ANALOG VALUES FROM THE JOYSTICK//
  int xDir = analogRead(jx);
  int yDir = analogRead(jy);
  boolean switchState = digitalRead(swPin);
  ///////////////////////////////////////////////

  ////CONVERTING JOYSTICK ANALOG VALUES INTO A USABLE DIGITAL VALUE////
  if ((yDir > jHigh - 50) && (xDir != jHigh) && (xDir != jLow)) {
    upJoystickState = 1;
  } else {
    upJoystickState = 0;
  }

  if ((xDir < jLow + 50) && (yDir != jHigh) && (yDir != jLow)) {
    leftJoystickState = 1;
  } else {
    leftJoystickState = 0;
  }

  if ((yDir < jLow + 50) && (xDir != jHigh) && (xDir != jLow)) {
    downJoystickState = 1;
  } else {
    downJoystickState = 0;
  }

  if (switchState == LOW) {
    selectJoystickState = 1;
  } else {
    selectJoystickState = 0;
  }
  ///////////////////////////////////////////////////////////////////
  checkIfDownJoystick();
  checkIfUpJoystick();
  checkIfLeftJoystick();
  checkIfSelectJoystick();

//////////PAGE 1 STATE MACHINE ALL FRUITS//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Essentially, since the screen can only hold 3 menu items per frame, we have different if statements ensuring that cycling through the menu is circular
// meaning that going up from the first item will lead you to the final item and the inverse. Also checks if the button is pressed on a menu item so we can
// move to a different page. Aiming the joystick left sends us back to a previous page.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
if (page == 1){
    if (menuitem == 1){
      if (up){
        up = false;
        menuitem = amounts[page-1];
      }
      if (down){
        down = false;
        menuitem++;
      }
      if (middle){
        middle = false;
        lastPage = page;
        page = menuitem + 1;
        menuitem = 1;
      }
    } else if (menuitem == amounts[page-1]){
      if (up){
        up = false;
        menuitem--;
      }
      if (down){
        down = false;
        menuitem = 1;
      }
      if (middle){
        middle = false;
        lastPage = page;
        page = menuitem + 1;
        menuitem = 1;
      }
    } else {
      if (up){
        up = false;
        menuitem--;
      }
      if (down){
        down = false;
        menuitem++;
      }
      if (middle){
        middle = false;
        lastPage = page;
        page = menuitem + 1;
        menuitem = 1;
      }
    }
    // submenu = menuitem / 4;
}
/////////////////////////////////////////////////////////////////////////////////
//////// PAGE 2 - 5 STATE MACHINE FRUITS/HERBS/HOUSE PLANTS/VEGETABLES //////////
/////////////////////////////////////////////////////////////////////////////////

if (page == 2 || page == 3 || page == 4 || page == 5){
  if (menuitem == 1){
      if (up){
        up = false;
        menuitem = amounts[page - 1];
      }
      if (down){
        down = false;
        menuitem++;
      }
      if (middle){
        middle = false;
        lastPage = page;
        page = 6;
      }
      if (left){
        left = false;
        page = 1;
        menuitem = 1;
      }
    } else if (menuitem == amounts[page - 1]){
      if (up){
        up = false;
        menuitem--;
      }
      if (down){
        down = false;
        menuitem = 1;
      }
      if (middle){
        middle = false;
        lastPage = page;
        page = 6;
      }
      if (left){
        left = false;
        page = 1;
        menuitem = 1;
      }
    } else {
      if (up){
        up = false;
        menuitem--;
      }
      if (down){
        down = false;
        menuitem++;
      }
      if (middle){
        middle = false;
        lastPage = page;
        page = 6; 
      }
      if (left){
        left = false;
        page = 1;
        menuitem = 1;
      }
    }
    // submenu = menuitem / 4;
}

/////////////// PAGE 6 STATE MACHINE COMPARISONS //////////////////////////////////////////////////////////////////////////////////////////////
// There is not much to do in this page excpet look at the values presented, and turn the joystick to the left to go back to a previous page //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
if (page == 6){
  if (left){
    left = false;
    page = lastPage;
  }
  if (middle){
    middle = false;
  }
  if (up){
    up = false;
  }
  if (down){
    down = false;
  }
}
submenu = menuitem / 4; // <-- SUPER DUPER IMPORTANT
////////////////////////////////////

}

////////////// JOYSTICK CHECKER FUNCTIONS ////////////////////////////////////////////////////////////////////////////
// The functions bellow check each direction and button to see if anything has been done to the joystick. ////////////
// An action is only registered when the joystick is returned to the original position. //////////////////////////////
// NOTE : I THINK IT WOULD BE PRETTY EASY TO MAKE IT NOT HAVE TO DO THAT SO YOU COULD JUST HOLD IT IN ONE DIRECTION //
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void checkIfDownJoystick()
{
  if (downJoystickState != lastDownJoystickState) 
  {
    if (downJoystickState == 0) 
    {
      down=true;
    }
  delay(50);
  } 
  lastDownJoystickState = downJoystickState;
}

void checkIfLeftJoystick()
{
  if (leftJoystickState != lastLeftJoystickState) 
  {
    if (leftJoystickState == 0) 
    {
      left=true;
    }
    delay(50);
  }
  lastLeftJoystickState = leftJoystickState;
}

void checkIfUpJoystick()
{
  if (upJoystickState != lastUpJoystickState) 
  {
    if (upJoystickState == 0) {
      up=true;
    }
    delay(50);
  }
   lastUpJoystickState = upJoystickState;
}

void checkIfSelectJoystick()
{
   if (selectJoystickState != lastSelectJoystickState) 
  {
    if (selectJoystickState == 0) {
      middle=true;
    }
    delay(50);
  }
   lastSelectJoystickState = selectJoystickState;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
  

////////////////////// DRAWING FUNCTION FOR EACH DIFFERENT PAGE //////////////////////////////////////////
// This essentially just has different layouts of text for each page. ////////////////////////////////////
///////////////////////////////// Page 1 = the home page /////////////////////////////////////////////////
///////////////////////////////// Page 2 = fruit page ////////////////////////////////////////////////////
///////////////////////////////// Page 3 = herb page /////////////////////////////////////////////////////
///////////////////////////////// Page 4 = house plant page //////////////////////////////////////////////
///////////////////////////////// Page 5 = vegetable page ////////////////////////////////////////////////
///////////////////////////////// Page 6 = comparison page ///////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// The pages are all partitioned to hold 3 lines of text and a main header, the way this is done is quite simple,
// we can just take the modulus of the menuitem to know that every 4th item will be in the 1st position, every 5th item will be in the 2nd position, etc.
// Similarly, we chose what plants to print by referring to the different arrays that each group has and using the submenu variable which represent the amount
// of times that we have gone down a page. In lamen terms, when submenu = 1, it means that we have gone from a page with [1, 2, 3] to [4, 5, 6].
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  void drawMenu(){

  
    
  if (page==1){    
    drawMain();
  }
  else if (page==2){
    drawFruits();
  }
  else if (page==3){
    drawHerbs();
  }
  else if (page==4){
    drawHousePlants();
  }
  else if (page==5){
    drawVegetables();
  }
  else if (page==6){
   drawComparisons();
  }

  display.display();
}

void drawMain(){
  String menu[4] = {"Fruits", "Herbs", "House Plants", "Vegetables"};


  display.setTextSize(1);
  display.clearDisplay();
  display.setTextColor(BLACK, WHITE);
  display.setCursor(11, 0);
  display.print("ALL PLANTS");
  display.drawFastHLine(0,10,83,BLACK);
  display.setCursor(0, 15);
  
  if (menuitem % 3 == 1){ 
    display.setTextColor(WHITE, BLACK);
  } else {
    display.setTextColor(BLACK, WHITE);
  }
  if (0 + 3*submenu < amounts[0]){
  display.print(">"+menu[0 + 3*submenu]);
  }

  display.setCursor(0, 25);
  
  if (menuitem % 3 == 2){
    display.setTextColor(WHITE, BLACK);
  } else {
    display.setTextColor(BLACK, WHITE);
  }
  if (1 + 3*submenu < amounts[0]){
    display.print(">"+menu[1 + 3*submenu]);
  }

  display.setCursor(0, 35);
  if (menuitem % 3 == 0) { 
    display.setTextColor(WHITE, BLACK);
  } else {
    display.setTextColor(BLACK, WHITE);
  }  
  if (2 + 3*submenu < amounts[0]){
    display.print(">"+menu[2 + 3*submenu]);
  }  
}
void drawFruits(){
  String fruits[][7] = {{"Lemon","27","10","21500","3200","55","50"},
                      {"Blueberry","29","18","10000","2000","55","45"},
                      {"Strawberry","29","18","10000","5000","55","45"},
                      {"Common Fig","32","-15","21500","3200","65","30"}};


  display.setTextSize(1);
  display.clearDisplay();
  display.setTextColor(BLACK, WHITE);
  display.setCursor(25, 0);
  display.print("FRUITS");
  display.drawFastHLine(0,10,83,BLACK);
  display.setCursor(0, 15);
  
  if (menuitem % 3 == 1){ 
    display.setTextColor(WHITE, BLACK);
  } else {
    display.setTextColor(BLACK, WHITE);
  }
  if (0 + 3*submenu < amounts[1]){
  display.print(">"+fruits[0 + 3*submenu][0]);
  }

  display.setCursor(0, 25);
  
  if (menuitem % 3 == 2){
    display.setTextColor(WHITE, BLACK);
  } else {
    display.setTextColor(BLACK, WHITE);
  }
  if (1 + 3*submenu < amounts[1]){
    display.print(">"+fruits[1 + 3*submenu][0]);
  }

  display.setCursor(0, 35);
  if (menuitem % 3 == 0) { 
    display.setTextColor(WHITE, BLACK);
  } else {
    display.setTextColor(BLACK, WHITE);
  }  
  if (2 + 3*submenu < amounts[1]){
    display.print(">"+fruits[2 + 3*submenu][0]);
  }
}
void drawHerbs(){
  String herbs[][7] = {{"Basil","35","10","21500","3200","60","40"},
                      {"Rosemary","27","10","21500","3200","55","45"},
                      {"Cannabis","30","20","21500","3200","65","55"}};


  display.setTextSize(1);
  display.clearDisplay();
  display.setTextColor(BLACK, WHITE);
  display.setCursor(28, 0);
  display.print("HERBS");
  display.drawFastHLine(0,10,83,BLACK);
  display.setCursor(0, 15);
  
  if (menuitem % 3 == 1){ 
    display.setTextColor(WHITE, BLACK);
  } else {
    display.setTextColor(BLACK, WHITE);
  }
  if (0 + 3*submenu < amounts[2]){
  display.print(">"+herbs[0 + 3*submenu][0]);
  }

  display.setCursor(0, 25);
  
  if (menuitem % 3 == 2){
    display.setTextColor(WHITE, BLACK);
  } else {
    display.setTextColor(BLACK, WHITE);
  }
  if (1 + 3*submenu < amounts[2]){
    display.print(">"+herbs[1 + 3*submenu][0]);
  }

  display.setCursor(0, 35);
  if (menuitem % 3 == 0) { 
    display.setTextColor(WHITE, BLACK);
  } else {
    display.setTextColor(BLACK, WHITE);
  }  
  if (2 + 3*submenu < amounts[2]){
    display.print(">"+herbs[2 + 3*submenu][0]);
  }
}
void drawHousePlants(){
  String houseplants[][7] = {{"Lavender","30","18","21500","3200","30","20"},
                            {"Orchid","28","12","21500","3200","45","35"},
                            {"Peace Lilly","28","15","21500","3200","55","45"},
                            {"Evergreen","28","18","21500","3200","55","45"}};
                            // {"Jade Plant","35","10","21500","3200","20","0"},
                            // {"Venus Fly Trap","35","5","21500","3200","100","50"}};
  display.setTextSize(1);
  display.clearDisplay();
  display.setTextColor(BLACK, WHITE);
  display.setCursor(7, 0);
  display.print("HOUSE PLANTS");
  display.drawFastHLine(0,10,83,BLACK);
  display.setCursor(0, 15);
  
  if (menuitem % 3 == 1){ 
    display.setTextColor(WHITE, BLACK);
  } else {
    display.setTextColor(BLACK, WHITE);
  }
  if (0 + 3*submenu < amounts[3]){
  display.print(">"+houseplants[0 + 3*submenu][0]);
  }

  display.setCursor(0, 25);
  
  if (menuitem % 3 == 2){
    display.setTextColor(WHITE, BLACK);
  } else {
    display.setTextColor(BLACK, WHITE);
  }
  if (1 + 3*submenu < amounts[3]){
    display.print(">"+houseplants[1 + 3*submenu][0]);
  }

  display.setCursor(0, 35);
  if (menuitem % 3 == 0) { 
    display.setTextColor(WHITE, BLACK);
  } else {
    display.setTextColor(BLACK, WHITE);
  }  
  if (2 + 3*submenu < amounts[3]){
    display.print(">"+houseplants[2 + 3*submenu][0]);
  }
}
void drawVegetables(){
  String vegetables[][7] = {{"Kale","24","13","10000","5000","95","85"}};//,
                          // {"Green Onions","23","12","10000","5000","55","45"},
                          // {"Tomato","32","4","21500","3200","60","40"},
                          // {"Serrano","30","10","21500","3200","65","45"}};


  display.setTextSize(1);
  display.clearDisplay();
  display.setTextColor(BLACK, WHITE);
  display.setCursor(12, 0);
  display.print("VEGETABLES");
  display.drawFastHLine(0,10,83,BLACK);
  display.setCursor(0, 15);
  
  if (menuitem % 3 == 1){ 
    display.setTextColor(WHITE, BLACK);
  } else {
    display.setTextColor(BLACK, WHITE);
  }
  if (0 + 3*submenu < amounts[4]){
  display.print(">"+vegetables[0 + 3*submenu][0]);
  }

  display.setCursor(0, 25);
  
  if (menuitem % 3 == 2){
    display.setTextColor(WHITE, BLACK);
  } else {
    display.setTextColor(BLACK, WHITE);
  }
  if (1 + 3*submenu < amounts[4]){
    display.print(">"+vegetables[1 + 3*submenu][0]);
  }

  display.setCursor(0, 35);
  if (menuitem % 3 == 0) { 
    display.setTextColor(WHITE, BLACK);
  } else {
    display.setTextColor(BLACK, WHITE);
  }  
  if (2 + 3*submenu < amounts[4]){
    display.print(">"+vegetables[2 + 3*submenu][0]);
  }
}
void drawComparisons(){
  getMoisture();
  display.setTextSize(1);
  display.clearDisplay();
  display.setTextColor(BLACK, WHITE);
  display.setCursor(0, 0);
  display.print("LIVE");
  display.setCursor(45, 0);
  display.print("IDEAL");
  display.drawFastHLine(0,10,83,BLACK);
  display.drawFastVLine(35, 0, 47, BLACK);

  /////////////CURRENT READING PRINTINGS/////////////
  display.setCursor(0, 15);
  display.print(getTemp() + "C");
  display.setCursor(0, 25);
  display.print(getLight() + "lm");
  display.setCursor(0, 35);
  display.print(getMoisture() + "%");
  
  /////////////IDEAL READING PRINTINGS///////////////
  //// We have lastpage which tells us what groups we need to choose the plants from ////
  //// We also have the menu item which tells us which plant exactly it is ////
  if (lastPage == 2){
    String items[][7] ={{"Lemon","27","10","21.5k","3.2k","55","50"},
                        {"Blueberry","29","18","10k","2k","55","45"},
                        {"Strawberry","29","18","10k","5k","55","45"},
                        {"Common Fig","32","-15","21.5k","3.2k","65","30"}};
    display.setCursor(40, 15);
    display.print(items[menuitem-1][2] + "-" + items[menuitem-1][1]);
    display.setCursor(40, 25);
    display.print(items[menuitem-1][4] + "-" + items[menuitem-1][3]);
    display.setCursor(40, 35);
    display.print(items[menuitem-1][6] + "-" + items[menuitem-1][5]);
  } else if (lastPage == 3){
    String items[][7] = {{"Basil","35","10","21.5k","3.2k","60","40"},
                          {"Rosemary","27","10","21.5k","3.2k","55","45"},
                          {"Cannabis","30","20","21.5k","3.2k","65","55"}};
    display.setCursor(40, 15);
    display.print(items[menuitem-1][2] + "-" + items[menuitem-1][1]);
    display.setCursor(40, 25);
    display.print(items[menuitem-1][4] + "-" + items[menuitem-1][3]);
    display.setCursor(40, 35);
    display.print(items[menuitem-1][6] + "-" + items[menuitem-1][5]);
  } else if (lastPage == 4){
    String items[][7] = {{"Lavender","30","18","21.5k","3.2k","30","20"},
                          {"Orchid","28","12","21.5k","3.2k","45","35"},
                          {"Peace Lilly","28","15","21.5k","3.2k","55","45"},
                          {"Evergreen","28","18","21.k","3.2k","55","45"}};
    display.setCursor(40, 15);
    display.print(items[menuitem-1][2] + "-" + items[menuitem-1][1]);
    display.setCursor(40, 25);
    display.print(items[menuitem-1][4] + "-" + items[menuitem-1][3]);
    display.setCursor(40, 35);
    display.print(items[menuitem-1][6] + "-" + items[menuitem-1][5]);
  } else if (lastPage == 5){
    String items[][7] = {{"Kale","24","13","10k","5k","95","85"}};//,
                              // {"Green Onions","23","12","10000","5000","55","45"},
                              // {"Tomato","32","4","21500","3200","60","40"},
                              // {"Serrano","30","10","21500","3200","65","45"}};
    display.setCursor(40, 15);
    display.print(items[menuitem-1][2] + "-" + items[menuitem-1][1]);
    display.setCursor(40, 25);
    display.print(items[menuitem-1][4] + "-" + items[menuitem-1][3]);
    display.setCursor(40, 35);
    display.print(items[menuitem-1][6] + "-" + items[menuitem-1][5]);
  }
}

int getMoisture(){
  digitalWrite(m_pow, HIGH);
  delay(10); // Wait for 10 millisecond(s)
  float moisture = analogRead(m_dat);
  moisture = (((moisture - MOISTURE_MIN) / (MOISTURE_MIN - MOISTURE_MAX)) * 70) + 100;
  // Turn off the sensor to reduce metal corrosion
  // over time
  digitalWrite(m_pow, LOW);
  return floor(moisture);
}
int getTemp(){
  float temp = analogRead(t_dat);
  temp = (temp * 0.48828125)-49.5;
  return floor(temp);
}
int getLight(){
  int ldrValue = analogRead(ldrPin);
  return sensorRawToPhys(ldrValue);
  // analogWrite(3, map(light, 0, 1023, 0, 225));
  // return light;
}
