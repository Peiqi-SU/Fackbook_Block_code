#include <Servo.h> 
#define Servo_pin 9
#define FeedBack A5
#define GLED 13 
#define Gnd 3
#define RLED 2
#define ON 0
#define OFF 90
#define RESET 180

Servo myservo;
//int feedback = analogRead(FeedBack);
int reset_time = 5000;
int pos = 0;  
char input; 
boolean sand_timer_on = false;


void setup() 
{ 
  Serial.begin(9600);

  // set servo to OFF
  myservo.attach(Servo_pin);  
  // back to 0
  for (int i=0;i<100;i++){
    myservo.write(OFF);             
    delay(20);  
  }
  myservo.detach();
  Serial.print("start at: ");
  Serial.println(analogRead(FeedBack)); //449

  // LED
  Serial.print("\r\nStart");
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
    Serial.println(input); 
    // close facebook --> turn off sand timer
    if (input == 'N') {
      sand_timer_on = false;
      myservo.attach(Servo_pin); // attach the servo
      for (int i=0;i<100;i++){
        digitalWrite(GLED, HIGH);
        myservo.write(OFF);             
        delay(20);  
      }
      myservo.detach(); // detach the servo, so you can turn it by hand
      Serial.print("off position: ");
      Serial.println(analogRead(FeedBack)); //449
    } 
    // open facebook --> turn on sand timer
    if (input == 'Y') {
      sand_timer_on = true;
      myservo.attach(Servo_pin);
      for (int i=0;i<100;i++){
        digitalWrite(GLED, LOW);
        myservo.write(ON); 
        delay(20);  
      }
      myservo.detach();
      Serial.print("on position: ");
      Serial.println(analogRead(FeedBack)); //161
    }
    // new day --> reset sand timer
    if (input == 'R') {
      sand_timer_on = false;
      myservo.attach(Servo_pin);
      for (int i=0;i<100;i++){
        digitalWrite(RLED, LOW);
        digitalWrite(GLED, HIGH);
        myservo.write(RESET);             
        delay(20);  
      }
      delay(reset_time);
      for (int i=0;i<100;i++){
        digitalWrite(RLED, LOW);
        digitalWrite(GLED, HIGH);
        myservo.write(OFF);             
        delay(20);  
      }
      myservo.detach();
      Serial.print("reset position: ");
      Serial.println(analogRead(FeedBack)); //449
    }
  }
  // turn the sand timer by hand
  pos = analogRead(FeedBack);
  if(sand_timer_on && pos > 200){
    sand_timer_on = false;
    myservo.attach(Servo_pin);
    for (int i=0;i<100;i++){
      digitalWrite(GLED, HIGH);
      myservo.write(OFF);             
      delay(20);  
    }
    myservo.detach();
    Serial.println("B"); // send B to processing
    Serial.print("turn by hand position: ");
    Serial.println(analogRead(FeedBack)); //449
  }
} 
