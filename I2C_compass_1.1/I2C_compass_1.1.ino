/*I only added the essentials to this assignment.
 * 
 *You may need to add the motor and it's pins
 *I didn't add that because it would be cool if the compass module can work without it
 */

#include <LiquidCrystal.h>

LiquidCrystal lcd(12, 11, 5, 4, 25, 26); //CHECK IF YOU GUYS CHANGED THE WIRES

/////////////////////////
// Compass Definitions //
/////////////////////////

#include <TimerOne.h>
#include <Wire.h>

#define CMPS14_address 0x60

int compass_val(); //reads raw compass value, translates it from 0 - 360 and returns as int

void setup() {
  Serial.begin(9600); // open the serial port at 9600 bps:
  Serial.println("+----------------------------------+");
  Serial.println("|         Software Americans       |");
  Serial.println("+----------------------------------+"); 

  lcd.begin(20,4); //LCD is initialized but I'm only printing to serial

  Wire.begin(); //Wire library
}

void loop() {
  // put your main code here, to run repeatedly:
  int degree = compass_val(); 
  string bearing;

  //assign bearing
  if(degree >= 22 && degree < 67) bearing = "NE";
  else if(degree >= 67 && degree < 112) bearing = "E";
  else if(degree >= 112 && degree < 157) bearing = "SE";
  else if(degree >= 157 && degree < 202) bearing = "S";
  else if(degree >= 202 && degree < 247) bearing = "SW";
  else if(degree >= 247 && degree < 292) bearing = "W";
  else if(degree >= 292 && degree < 337) bearing = "NW";
  else bearing = "N";

  print("bearing: ");
  println(bearing);  
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
