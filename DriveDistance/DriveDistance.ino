//LCD 
#include <LiquidCrystal.h>
LiquidCrystal lcd(37,36,35,34,33,32);

//Motor pins
#define LeftPWM  9  //sets L motor speed
#define LeftDir 7   //sets L motor direction
#define RightPWM 10 //sets R motor speed
#define RightDir 8  //sets R motor direction

/////////////////////////
// Compass Definitions //
/////////////////////////

#include <TimerOne.h>
#include <Wire.h>

#define CMPS14_address 0x60

//Math constants
const float pi = 3.14;
const float R = 3.25;
const int N = 400;

float distance = 0;

const int LeftVal = 0;
const int RightVal = 0;

int i = 0;

bool DriveDistance(int displace);
void PrintDriveCommand();
void PrintTurnCommand();
int compass_val(); //reads raw compass value, translates it from 0 - 360 and returns as int
int ReadCompass();


void setup()
{
  Serial.begin(9600);
  lcd.clear();
  lcd.begin(20, 4);

  //pwd
  pinMode(25, INPUT);
  digitalWrite(25, HIGH);
  pinMode(24, INPUT);
  digitalWrite(24, HIGH);

  //compass
  Wire.begin(); //Wire library
  
  attachInterrupt(digitalPinToInterrupt(2), forwRight, RISING);
  //attachInterrupt(digitalPinToInterrupt(3), forwLeft, RISING);
  PrintTurnCommand();
}

void forwLeft(void())
{
  //if PIN 24 is LOW, => forward
  if(24==0){
    lcd.clear();
    lcd.println("forward left");
    lcd.println(i--);
    distance = ((2*pi*R)/N) * i;
  
    lcd.setCursor(0,2);
    lcd.print( distance);
  
    lcd.setCursor(0,3);
    lcd.print("cm  ");} 
  else {    
    lcd.clear();
    lcd.println("backward left");
    lcd.println(i++);
    distance = ((2*pi*R)/N) * i;
  
    lcd.setCursor(0,2);
    lcd.print( distance);
  
    lcd.setCursor(0,3);
    lcd.print("cm  ");
  }
} 

void forwRight()
{
  //if PIN 25 is LOW, => forward
  if(25==0){
    lcd.clear();
    Serial.println("forward right");
    Serial.println(i--); 
    //distance = ((2*pi*R)/N) * i;
  
    //lcd.setCursor(0,2);
    //lcd.print(distance);
  
    //lcd.setCursor(0,3);
    //lcd.print("cm  ");
  } else {  
    lcd.clear();
    Serial.println("backward right");
    Serial.println(i++);
    //distance = ((2*pi*R)/N) * i;

    //lcd.setCursor(0,2);
    //lcd.print( distance);
  
    //lcd.setCursor(0,3);
    //lcd.print("cm  ");
  
  }
}
          
void loop()
{
  int command;
    
  if(Serial.available()) {
    command = Serial.parseInt();
    Serial.read();  //clear buffer
    
    if(TurnDegrees(command)) {
      Serial.print("Successfully turned ");
      Serial.print(command);
      Serial.println(" degrees");
    }
    else Serial.println("Failed to turn");

    //Print again to allow user to see they can enter another command
    PrintTurnCommand();
  }
}

//negative = reverse
//positive = forwards
//argument = distance (cm)
bool DriveDistance(int displace) {
  //convert desired displacement in cm into pulses
  const int pulses = abs(displace) * (N /(2*pi*R)); 
  Serial.print("Pulses: ");
  Serial.println(pulses); //showing correcly
  
  //set amount of pulses needed to reach distance from current pulses
  const int destPulseCnt = i + pulses;

  //forwards > 0, else backwards
  if(displace > 0) {
    digitalWrite(LeftDir, 1);
    digitalWrite(RightDir, 1);
  } else {
    digitalWrite(LeftDir, 0);
    digitalWrite(RightDir, 0);
  }

  analogWrite(LeftPWM, 255/4);
  analogWrite(RightPWM, 255/4);
  
  while( abs(i) <= destPulseCnt) {
    Serial.print(i); Serial.print('/'); Serial.println(destPulseCnt);
    if(abs(i) >= destPulseCnt) break;
    //in attach interrupt, i increments pulses
  }

  //stop driving
  analogWrite(LeftPWM, 1);
  analogWrite(RightPWM, 1);  
  return true;
}

bool TurnDegrees(int amountD) {
  const int initial_degree = ReadCompass();
  int offset, final_degree, current_degree;

  Serial.print("initial degree: ");
  Serial.println(initial_degree);
  Serial.print("final degree: ");
  Serial.println(final_degree);

  //turn direction
  if(amountD > 0) {
    //turn right
    Serial.print("right");
    digitalWrite(LeftDir, 0);
    digitalWrite(RightDir, 1);
    //turning right wrap around -> start at 0 and increase
    current_degree = 0;
    offset = initial_degree;
    final_degree = (amountD + initial_degree) % 360;
  }
  else {
    //turn left
    Serial.print("left");
    digitalWrite(LeftDir, 1);
    digitalWrite(RightDir, 0);
    //turning left wrap around -> start at 359 and decrease
    current_degree = 359;
    offset = current_degree - initial_degree;
    final_degree = current_degree - (abs(amountD) % 360);
    //modulus does weird things with negative numbers
  }

  //start motors
  analogWrite(LeftPWM, 255/5);
  analogWrite(RightPWM, 255/5);

  //turn car right or left until it hits final_degree
  if(amountD > 0) 
    while(current_degree <= final_degree - offset) {
      current_degree = (ReadCompass() - offset) % 360;
    }
  //start at 359
  else 
    while(current_degree > final_degree) {
      current_degree = (ReadCompass() + offset) % 360;
    }

  //stop driving
  analogWrite(LeftPWM, 1);
  analogWrite(RightPWM, 1);  
  
  return true;
}

//reads the raw compass value & converts it from int to degrees
int compass_val()
{
  Wire.beginTransmission( (uint8_t) CMPS14_address);
  Wire.write( (uint8_t) 1);
  Wire.endTransmission(false);

  Wire.requestFrom( (uint8_t) CMPS14_address,(uint8_t) 1, (uint8_t) true);

  if ( Wire.available() >=1 )
  {
    byte OrigCompassVal = Wire.read();//compass value between 0 - 255
  
    int TrueCompassVal = OrigCompassVal; //cast the orig compass value into a long int
  
    TrueCompassVal = TrueCompassVal * 360/256; //calculation to get the actual degrees

    Serial.print("converted compass value: ");
    Serial.println(TrueCompassVal);
  
    return TrueCompassVal; //final value to get the compass value between 0 - 360
  };
}


//2nd compass function, orientation is off
int ReadCompass()
{
  const int TWO_BYTES = 2;
  const int FOUR_BYTES = 4;
  const int COMMUNICATION_TIMEOUT = 1000;
  const int BEARING_Register = 2;
  
  byte _byteHigh;
  byte _byteLow;

  //compass data
  int bearing = 0;
  signed char pitch, roll;
  
  // Setup timeout parameter
  int timeout = COMMUNICATION_TIMEOUT;

  // Begin communication with CMPS14
  Wire.beginTransmission(CMPS14_address);

  // Tell register you want some data
  Wire.write(BEARING_Register);

  // End the transmission
  int nackCatcher = Wire.endTransmission();

  // Return if we have a connection problem 
  if(nackCatcher != 0){bearing = 0; pitch = 0;  roll = 0; return;}
  
  // Request 4 bytes from CMPS14
  Wire.requestFrom(CMPS14_address , FOUR_BYTES);

  // Wait for the bytes to arrive.
  // Don't wait forever as this will hang the whole program
  while((Wire.available() < FOUR_BYTES) && (timeout-- >0))
    delay(1);

  // Timed out so return
  if (timeout <= 0) {bearing = 0; pitch = 0;  roll = 0; return;}
  
  // Read the values
  _byteHigh = Wire.read();
  _byteLow = Wire.read();
  bearing = ((_byteHigh<<8) + _byteLow) / 10;

  // Read the values
  pitch = Wire.read();

  // Read the values
  roll = Wire.read();

  Serial.print("bearing value: ");
  Serial.println(bearing);

  return bearing;
}

void PrintDriveCommand() {
  Serial.println("Enter distance to travel in cm.");
  Serial.println("(negative is backwards)");
  Serial.print("> ");
}

void PrintTurnCommand() {
  Serial.println("Enter distance to turn in degrees.");
  Serial.println("(negative is left)");
  Serial.print("> ");
}
