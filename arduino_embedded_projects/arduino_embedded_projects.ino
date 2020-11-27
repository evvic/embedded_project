/*
  Uses a for loop to print numbers in various formats.
*/
#include <LiquidCrystal.h>
  //LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
  LiquidCrystal lcd(12, 11, 5, 4, 25, 26); //changed wires to make space for encoder


#define PWM1 10
#define PWM2 9


//void pulsing_R(void);
//void pulsing_L(void);

//int count = 0;

#define encoderARight 23
#define encoderALeft 24
//#define encoderBRight 2
//#define encoderBLeft 3

#define directionR 2
#define directionL 3

volatile int R_pulse, L_pulse;

void setup() {
  pinMode(PWM1, OUTPUT);
  pinMode(PWM2, OUTPUT);
  //for motor example

  //encoder pins
  /*
  pinMode(encoderARight, INPUT); //ENC A MotorRight
  pinMode(encoderALeft, INPUT); //ENC A MotorLeft
  pinMode(encoderBRight, INPUT); //ENC B MotorRight
  pinMode(encoderBLeft, INPUT); //ENC B MotorLeft
  */


  attachInterrupt(digitalPinToInterrupt(directionR), pulsing_R, RISING);
  attachInterrupt(digitalPinToInterrupt(directionL), pulsing_L, RISING);

  Serial.begin(9600); // open the serial port at 9600 bps:
  
  Serial.print("Intro");
  lcd.begin(20,4);
}

 

void loop() {

    //Serial.print(count);
    lcd.setCursor(0, 0);
    lcd.print("Sup jaakko");
    //delay(200);
    Serial.print("A count");
    Serial.print(L_pulse);
    Serial.print("\t");


    Serial.print("B count");
    Serial.print(R_pulse);

    delay(2000);
    

    //analogWrite(PWM1, 240); //thing goes vroom vroom
    //analogWrite(PWM2, 240);  //thing goes vroom vroom


    //current problem: 1 motor seems to work while the other doesn't

}

/*
void pin_ISR()
//this if statemeent currently doesnt work but is the idea of exercise isr 2.0
//get after break try to get this to work lol
//
{
  if(encoderBRight == HIGH) { //if pin 19 is presssed and pin 18 is HIGH, ++
    count++;
    lcd.setCursor(0, 1);
    lcd.print("encoder B HIGH");
  }
  else { //if pin 19 is presssed and pin 18 is LOW, --
    count--;
    lcd.setCursor(0, 1);
    lcd.print("encoder B low");
  }
  if(encoderARight == HIGH) { //if pin 19 is presssed and pin 18 is HIGH, ++
    count++;
    lcd.setCursor(0, 1);
    lcd.print("encoder A HIGH");
    
  }
  else { //if pin 19 is presssed and pin 18 is LOW, --
    count--;
    lcd.setCursor(0, 1);
    lcd.print("encoder A low");
  }
 */


 // Serial.print("Pressed");
  
  //delay(500);




void pulsing_R(void) {
  if(digitalRead(directionR) == 0) { R_pulse--; } else R_pulse++;
  lcd.setCursor(0, 1);
  lcd.println("pulsing_R"); 
}








void pulsing_L(void) {
  if(digitalRead(directionL) == 0) { L_pulse--; } else L_pulse++;
  lcd.setCursor(0, 1);
  lcd.print("pulsing_L");

}



/*
 * Homework Questions
 * 
 * 1.  165 pulses in one round
 * 
 * 2.  One round is 14cm in distance
 * 
 * 3.  You would get 1.178,57 pulses after driving 1 meter
 * 
 * 4.  You would get 11,78 pulses after driving 1 cm.
 */
