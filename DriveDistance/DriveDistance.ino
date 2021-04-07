//LCD 
#include <LiquidCrystal.h>
LiquidCrystal lcd(37,36,35,34,33,32);

//Motor pins
#define LeftPWM  9  //sets L motor speed
#define LeftDir 7   //sets L motor direction
#define RightPWM 10 //sets R motor speed
#define RightDir 8  //sets R motor direction

//Math constants
const float pi = 3.14;
const float R = 3.25;
const int N = 400;

float distance = 0;

const int LeftVal = 0;
const int RightVal = 0;

int i = 0;

bool DriveDistance(int displace);
void PrintCommand();

void setup()
{
  Serial.begin(9600);
  lcd.clear();
  lcd.begin(20, 4);
  
  pinMode(25, INPUT);
  digitalWrite(25, HIGH);
  pinMode(24, INPUT);
  digitalWrite(24, HIGH);
  
  attachInterrupt(digitalPinToInterrupt(2), forwRight, RISING);
  //attachInterrupt(digitalPinToInterrupt(3), forwLeft, RISING);
  PrintCommand();
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
    
    if(DriveDistance(command)) {
      Serial.print("Successfully drove ");
      Serial.print(command);
      Serial.println(" cm");
    }
    else Serial.println("Failed to drive distance");

    //Print again to allow user to see they can enter another command
    PrintCommand();
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

void PrintCommand() {
  Serial.println("Enter distance to travel in cm.");
  Serial.println("(negative is backwards)");
  Serial.print("> ");
}
