/*

  Uses a for loop to print numbers in various formats.

*/
#include <LiquidCrystal.h>
  LiquidCrystal lcd(12, 11, 5, 4, 3, 2);


void setup() {

  Serial.begin(9600); // open the serial port at 9600 bps:

}

 

void loop() {


  int sensorValueX;
  int sensorValueY;
  
  sensorValueX = analogRead(A7);
  sensorValueY = analogRead(A6);

  //lcd.setCursor(0, 1);
  lcd.print("X-value:");
  lcd.print(sensorValueX);
  Serial.print(sensorValueX);

  //lcd.setCursor(0, 2);
  lcd.print("Y-value:");
  lcd.print(sensorValueY);
  Serial.print(sensorValueY);

  delay(500);
  
  lcd.clear();
}
