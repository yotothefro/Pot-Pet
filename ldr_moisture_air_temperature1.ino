// C++ code
//
#include<math.h>
static int MOISTURE_MIN = 234;
static int MOISTURE_MAX = 1008;
int moisture = 0;

int Light = 0;

int temp = 0;

void setup()
{
  pinMode(A0, OUTPUT);
  pinMode(A1, INPUT);
  Serial.begin(9600);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(A2, INPUT);
  pinMode(3, OUTPUT);
}

void loop()
{
  // Apply power to the soil moisture sensor
  digitalWrite(A0, HIGH);
  delay(10); // Wait for 10 millisecond(s)
  moisture = analogRead(A1);
  moisture = floor(10000*((moisture - MOISTURE_MIN) / (MOISTURE_MAX - MOISTURE_MIN)) * 100)/10000;
  // Turn off the sensor to reduce metal corrosion
  // over time
  digitalWrite(A0, LOW);
  Serial.print("Moisture = ");
  Serial.print(moisture);
  Serial.print(" %");
  Serial.println();
               
  delay(1000); // Wait for 500 millisecond(s)
  
  Light = analogRead(A2);
  analogWrite(3, map(Light, 0, 1023, 0, 225));
  Serial.print("Light = ");
  Serial.println(Light);
  delay(1000); // Wait for 500 millisecond(s)
  
  temp = analogRead(A3);
  temp = (temp * 0.48828125)-49.5;
  Serial.print("Temperature = ");
  Serial.print(temp); // display temperature value
  Serial.print(" C");
  Serial.println();
  delay(1000); // update sensor reading each one second
}
