/*
  Uses a joystick to move a car around
*/
#include <LiquidCrystal.h>
  //LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
  LiquidCrystal lcd(12, 11, 5, 4, 25, 26); //changed wires to make space for encoder

/////////////////////////
// Compass Definitions //
/////////////////////////

#include <TimerOne.h>
#include <Wire.h>

#define CMPS14_address 0x60

/////////////////////////
//  Motor Definitions  //
/////////////////////////

#define R_count_inter_pin 3   // R encoder pulse counting interrupt pin
#define L_count_inter_pin 2

#define direction_R 23        //encoder direction pins
#define direction_L 24

//switched the pwm and direction pins for left & right motors..
#define PWM_Motor_R 9 
#define Dir_Motor_R 7
#define PWM_Motor_L 10
#define Dir_Motor_L 8

#define forward 1
#define backward 0

//pin 10 is for left motor

volatile int R_pulse = 0, L_pulse = 0;
volatile byte count = 0;

//Joystick funciton
void Joystick(int& x_val, int& y_val);
//Perform drive distance
void DriveDistance(int distL, int distR);

void setup() {
  Serial.begin(9600); // open the serial port at 9600 bps:
  Serial.println("+----------------------------------+");
  Serial.println("|         Software Americans       |");
  Serial.println("+----------------------------------+"); 
  
  lcd.begin(20,4);

  Wire.begin(); //Wire library

  pinMode(R_count_inter_pin, INPUT);
  pinMode(L_count_inter_pin, INPUT);

  pinMode(direction_R,INPUT);    
  digitalWrite(direction_R,HIGH); 

  pinMode(direction_L,INPUT);    
  digitalWrite(direction_L,HIGH);

  attachInterrupt(digitalPinToInterrupt(R_count_inter_pin), pulsing_R, RISING);
  attachInterrupt(digitalPinToInterrupt(L_count_inter_pin), pulsing_L, RISING);
  //attachInterrupt(digitalPinToInterrupt(CMPS14_address), compass_interupt, RISING);
  

}

void loop() {
    int pw1L, pw2R;
    Joystick(pw1L, pw2R);

    lcd.setCursor(0, 0);
    lcd.print("Sup jaakko");

    Serial.print(" pw1L = ");
    Serial.print(pw1L);

    //ASK USER FOR DESIRED LEFT AND RIGHT MOTOR DRIVE DISTANCE
    DriveDistance(5, 7);
    //if (pw1L>0){ digitalWrite(Dir_Motor_L, forward); } else { digitalWrite(Dir_Motor_L, backward);};
    //if (pw2R>0){ digitalWrite(Dir_Motor_R, forward); } else { digitalWrite(Dir_Motor_R, backward);};

    //analogWrite( PWM_Motor_L, pw1L); 
    //analogWrite( PWM_Motor_R, pw2R);

    //current problem: 1 motor seems to work while the other doesn't
    lcd.clear();

}


//rotates car to the desired degree
bool TurnTo(int desiredDegree) {
  //read_compass_raw()
  int difference = desiredDegree - compass_val();
  for(int i = 0; difference > 2 || difference < -2; i++) {
    //accomplish turning here
    if(turnRight(desiredDegree)) {
      //left motor -> forward & right motor -> reverse
      DriveDistance(-5, 5); //because a motor isn't working, the broken motor may have to have its driveDistance = 0
    }
    else {
      //left motor -> reverse & right motor -> forward
      DriveDistance(-5, 5);
    }
    if(i > 180) {
      //error taking too long
      return false;
    }
    difference = desiredDegree - compass_val();
  }
  return true;
}

//determines if car should rotate left or right to achieve desired degree
bool turnRight(int desiredDegree) {
  int currentDegree = compass_val();
  int difference = desiredDegree - currentDegree;

  if(difference == 0) {
    //error: send an error message car is already alligned
    return false;
  }
      
  if(abs(difference) > 180 && difference > 0) {
    //desiredDegree is ahead in degrees and should turn left
    return false;
  }
  else if(abs(difference) > 180 && difference < 0) {
    //currentDegree is ahead in degrees and should turn right
    return true;
  }
  else if(abs(difference) < 180 && difference > 0) {
    //desiredDegree is ahead in degrees and should turn right
    return true;
  }
  else if(abs(difference) < 180 && difference < 0) {
    //currentDegree is ahead in degrees and should turn left
    return false;
  }
  else {
    //error: send error message with difference, desiredDegree, & currentDegree\\
    return false;
  }
}

void pulsing_R(void) {
  if(digitalRead(direction_R) == 0) { R_pulse--; } else R_pulse++;
  lcd.setCursor(0, 1);
  lcd.println("pulsing_R"); 
}

void pulsing_L(void) {
  if(digitalRead(direction_L) == 0) { L_pulse--; } else L_pulse++;
  lcd.setCursor(0, 1);
  lcd.println("pulsing_L");
}

//rotate joystick 45 degrees and it will work
void Joystick(int& x_val, int& y_val) {
  float sensorValueX = analogRead(A8);
  float sensorValueY = analogRead(A9);

  x_val = ((sensorValueX - 491.0) / 532.0) * 255; //adjusted for joystick calibration
  y_val = ((sensorValueY - 512.0) / 511.0) * 255;

  //returns range from -255 to 255
}

//function that accepts int distance for each motor and performs the task
void DriveDistance(int distL, int distR) {
  // distance units in this function are PULSES
  R_pulse=0;
  L_pulse=0;

  //determine forwards or reverse for each motor
  if (distL > 0) digitalWrite(Dir_Motor_L, forward); 
  else digitalWrite(Dir_Motor_L, backward);
  if (distR > 0) digitalWrite(Dir_Motor_R, forward); 
  else digitalWrite(Dir_Motor_R, backward);

  analogWrite( PWM_Motor_L, 69);
  analogWrite( PWM_Motor_R, 69);

  byte bk_L=1;
  byte bk_R=1;
  
  while(bk_R != 0 && bk_L != 0) {
   if (abs(L_pulse)>abs(distL)) {    
        analogWrite( PWM_Motor_L, 0);  // set PWM value M2PWM  pin 10
        bk_L = 0; 
        Serial.print(" Stop L = "); Serial.println(L_pulse);  
        
     }

    if (abs(R_pulse)>abs(distR)) {    
        analogWrite( PWM_Motor_R, 0);  // set PWM value M2PWM  pin 10
        bk_R = 0;
        Serial.print(" Ptop R = "); Serial.println(R_pulse); 
     }

  }
}

//reads the raw compass value & converts it from int to degrees
long int compass_val()
{
 
  Wire.beginTransmission( (uint8_t) CMPS14_address);
  Wire.write( (uint8_t) 1);
  Wire.endTransmission(false);

  Wire.requestFrom( (uint8_t) CMPS14_address,(uint8_t) 1, (uint8_t) true);

  if ( Wire.available() >=1 )
  {
    byte OrigCompassVal = Wire.read();//compass value between 0 - 256
  
    long int TrueCompassVal = OrigCompassVal; //cast the orig compass value into a long int
  
    TrueCompassVal = TrueCompassVal * 360/256; //calculation to get the actual degrees
  
    return TrueCompassVal; //final value to get the compass value between 0 - 360
  };
}


/*
 * 1.  165 pulses in one round
 * 
 * 2.  One round is 14cm in distance
 * 
 * 3.  You would get 1.178,57 pulses after driving 1 meter
 * 
 * 4.  You would get 11,78 pulses after driving 1 cm.
 */
