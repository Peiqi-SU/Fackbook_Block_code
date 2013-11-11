#include <Servo.h> 
#define Servo_pin 9
#define FeedBack A5
#define Servo_pin2 11
#define FeedBack2 A3
#define GLED 13 
#define Gnd 3
#define RLED 2
#define ON 0
#define ON2 180
#define OFF 90
#define OFF2 90
#define RESET 180
#define RESET2 0
#define TURN 180
#define TURN2 650
#define RESET_TIME 5000

Servo myservo;
Servo myservo2;
//int feedback = analogRead(FeedBack);
int reset_time = 0, reset_time2 = 0;
int pos = 0, pos2 = 0; 
char input; 
boolean sand_timer_on = false, sand_timer_on2 = false;

void setup() 
{ 
  Serial.begin(9600);

  // set servo to OFF
  myservo.attach(Servo_pin); 
  myservo2.attach(Servo_pin2);  
  // back to 0
  for (int i=0;i<100;i++){
    myservo.write(OFF);  
    myservo2.write(OFF2);      
    delay(10);  
  }
  myservo.detach();
  myservo2.detach();
//  Serial.print("start at: ");
//  Serial.println(analogRead(FeedBack)); //449
//  Serial.println(analogRead(FeedBack2)); //449

  // LED
//  Serial.print("\r\nStart");
  pinMode(GLED, OUTPUT);
  digitalWrite(GLED,LOW);
  pinMode(RLED, OUTPUT);
  digitalWrite(RLED,LOW);
  pinMode(Gnd, OUTPUT);
  digitalWrite(Gnd,LOW);
} 

void loop() {
  // turn servo based on feedback from facebook
  if (Serial.available() > 0) {
    input = Serial.read();
//    Serial.println(input); 
    // close facebook --> turn off sand timer
    if (input == 'N') {
      sand_timer_on = false;
      myservo.attach(Servo_pin); // attach the servo
      for (int i=0;i<100;i++){
        digitalWrite(GLED, HIGH);
        myservo.write(OFF);             
        delay(10);  
      }
      myservo.detach(); // detach the servo, so you can turn it by hand
//      Serial.print("off position: ");
//      Serial.println(analogRead(FeedBack)); //449
    } 

    if (input == 'n') {
      sand_timer_on2 = false;
      myservo2.attach(Servo_pin2); // attach the servo
      for (int i=0;i<100;i++){
        digitalWrite(GLED, HIGH);
        myservo2.write(OFF2);             
        delay(10);  
      }
      myservo2.detach(); // detach the servo, so you can turn it by hand
//      Serial.print("off position: ");
//      Serial.println(analogRead(FeedBack2)); //449
    } 


    // open facebook --> turn on sand timer
    if (input == 'Y') {
      sand_timer_on = true;
      myservo.attach(Servo_pin);
      for (int i=0;i<100;i++){
        digitalWrite(GLED, LOW);
        myservo.write(ON); 
        delay(10);  
      }
      myservo.detach();
//      Serial.print("on position: ");
//      Serial.println(analogRead(FeedBack)); //161
    }

    if (input == 'y') {
      sand_timer_on2 = true;
      myservo2.attach(Servo_pin2);
      for (int i=0;i<100;i++){
        digitalWrite(GLED, LOW);
        myservo2.write(ON2); 
        delay(10);  
      }
      myservo2.detach();
//      Serial.print("on position: ");
//      Serial.println(analogRead(FeedBack2)); //161
    }

    // new day --> reset sand timer
    if (input == 'R') {
      sand_timer_on = false;
      myservo.attach(Servo_pin);
      for (int i=0;i<100;i++){
        digitalWrite(RLED, LOW);
        digitalWrite(GLED, HIGH);
        myservo.write(RESET);             
        delay(10);  
      }
      myservo.detach();
      reset_time = millis();
    }

    if (input == 'r') {
      sand_timer_on2 = false;
      myservo2.attach(Servo_pin2);
      for (int i=0;i<100;i++){
        digitalWrite(RLED, LOW);
        digitalWrite(GLED, HIGH);
        myservo2.write(RESET2);             
        delay(10);  
      }
      myservo2.detach();
      reset_time2 = millis();
    }
  }
  // turn the sand timer by hand
  pos = analogRead(FeedBack);
  if(sand_timer_on && pos > TURN){
    sand_timer_on = false;
    myservo.attach(Servo_pin);
    for (int i=0;i<100;i++){
      digitalWrite(GLED, HIGH);
      myservo.write(OFF);             
      delay(10);  
    }
    myservo.detach();
    Serial.print("C"); // send B to processing
//    Serial.print("turn by hand position: ");
//    Serial.println(analogRead(FeedBack)); //449
  }

  pos2 = analogRead(FeedBack2);
  if(sand_timer_on2 && pos2 < TURN2){
    sand_timer_on2 = false;
    myservo2.attach(Servo_pin2);
    for (int i=0;i<100;i++){
      digitalWrite(GLED, HIGH);
      myservo2.write(OFF2);             
      delay(10);  
    }
    myservo2.detach();
    Serial.print("c"); // send B to processing
//    Serial.print("turn by hand position: ");
//    Serial.println(analogRead(FeedBack2)); //449
  }

  // return the reset timer
  if( reset_time !=0 && (millis() - reset_time > RESET_TIME)){
    reset_time = 0;
    myservo.attach(Servo_pin);
    for (int i=0;i<100;i++){
      digitalWrite(RLED, LOW);
      digitalWrite(GLED, HIGH);
      myservo.write(OFF);             
      delay(10);  
    }
    myservo.detach();
//    Serial.print("reset position: ");
//    Serial.println(analogRead(FeedBack)); //449
  }

  if( reset_time2 !=0 && (millis() - reset_time2 > RESET_TIME)){
    reset_time2 = 0;
    myservo2.attach(Servo_pin2);
    for (int i=0;i<100;i++){
      digitalWrite(RLED, LOW);
      digitalWrite(GLED, HIGH);
      myservo2.write(OFF2);             
      delay(10);  
    }
    myservo2.detach();
//    Serial.print("reset position: ");
//    Serial.println(analogRead(FeedBack2)); //449
  }
}








