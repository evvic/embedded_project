/*

  Uses a for loop to print numbers in various formats.

*/
#include <LiquidCrystal.h>
  LiquidCrystal lcd(12, 11, 5, 4, 3, 2);


void setup() {

  Serial.begin(9600); // open the serial port at 9600 bps:
  lcd.begin(20,4);
}

 

void loop() {

  int LEDpinDim = 9;
  int LEDpinDirection = 8;





  ///////////////////////////////////
  //////   X-Value             //////
  ///////////////////////////////////
  //reads about 4.91V, never really hits full 5V
    float sensorValueX = analogRead(A7);
  
    int lightX = ((sensorValueX - 491) / 532) * 255;
    analogWrite(9,abs(lightX)); //LED power is on the longer metal leg
  
    sensorValueX = (sensorValueX - 510) / 5.1;
  
    int colX = (sensorValueX / 10) + 10;


  ///////////////////////////////////
  //////   Y-Value             //////
  ///////////////////////////////////
    float sensorValueY = analogRead(A6);
  
    int lightY = ((sensorValueY - 512) / 511) * 255;
    analogWrite(8, abs(lightY)); //LED power is on the longer metal leg
    
    sensorValueY = (sensorValueY - 513) / 5.13;

  lcd.setCursor(0, 0);
  lcd.print("X-value:");
  lcd.print(sensorValueX);
  lcd.print(" %");
  Serial.print(sensorValueX);

  lcd.setCursor(0, 1);
  lcd.print("-100%     0%     100%");

  lcd.setCursor(colX, 2);
  lcd.print("I");

  lcd.setCursor(0, 3);
  lcd.print("Y-value:");
  lcd.print(sensorValueY);
  lcd.print(" %");
  Serial.print(sensorValueY);

  delay(500);
  
  lcd.clear();
}
