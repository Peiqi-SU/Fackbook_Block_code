#include <Servo.h> 
// Servo pin
int SERVO_PIN = 7, SERVO_PIN2 = 8; // self, other
int LED1 = 5, LED2 = 6;  // self, other
#define FEEDBACK A0 // self
#define FEEDBACK2 A1 // other's

// Servo threshod 
int ON = 0, OFF = 90, RESET = 180;
int ON2 = 180, OFF2 = 90, RESET2 = 0;
#define RESET_TIME 5000

/* Feedback:
 2-1: on 114; off 311
 2-2: on 474; off 307
 */
#define TURN 160
#define TURN2 440

Servo myservo;
Servo myservo2;

int reset_time = 0, reset_time2 = 0;
int pos = 0, pos2 = 0; 
char input; 
boolean sand_timer_on = false, sand_timer_on2 = false;

int brightness1 = 0, brightness2 = 0;
int fadeAmount_slow1 = 3, fadeAmount_fast1 = 6;
int fadeAmount_slow2 = 3, fadeAmount_fast2 = 6;
/* LED_status
 0 = lignt on; 1 = fast blink; 2 = breath slow; 3 = breath fast
 */
int LED_status1 = 0, LED_status2 = 0;

void setup() {
  Serial.begin(9600);

  // set servo to OFF
  myservo.attach(SERVO_PIN); 
  myservo2.attach(SERVO_PIN2);  
  // back to 0
  for (int i=0;i<100;i++){
    myservo.write(OFF);  
    myservo2.write(OFF2);      
    delay(10);  
  }
  myservo.detach();
  myservo2.detach();

  // LED
  pinMode(LED1, OUTPUT);
  digitalWrite(LED1,HIGH);
  pinMode(LED2, OUTPUT);
  digitalWrite(LED2,HIGH);
} 

void loop() {
  // turn servo based on FEEDBACK from facebook
  if (Serial.available() > 0) {
    input = Serial.read();
    //    Serial.println(input);
    // Open the chrome 
    if (input == 'O') {
      LED_status1 = 2;
    }
    if (input == 'o') {
      LED_status2 = 2;
    }

    // close facebook --> turn off sand timer
    if (input == 'N' || input == 'C') {
      sand_timer_on = false;
      turn_sandtimer(myservo, SERVO_PIN, OFF, LED1);
      LED_status1 = 2;
    } 

    if (input == 'n' || input == 'c') {
      sand_timer_on2 = false;
      turn_sandtimer(myservo2, SERVO_PIN2, OFF2, LED2);
      LED_status2 = 2;
    } 

    // open facebook --> turn on sand timer
    if (input == 'Y') {
      sand_timer_on = true;
      turn_sandtimer(myservo, SERVO_PIN, ON, LED1);
      LED_status1 = 3;
    }

    if (input == 'y') {
      sand_timer_on2 = true;
      turn_sandtimer(myservo2, SERVO_PIN2, ON2, LED2);
      LED_status2 = 3;
    }

    // new day --> reset sand timer
    if (input == 'R') {
      sand_timer_on = false;
      turn_sandtimer(myservo, SERVO_PIN, RESET, LED1);
      reset_time = millis();
      LED_status1 = 0;
    }

    if (input == 'r') {
      sand_timer_on2 = false;
      turn_sandtimer(myservo2, SERVO_PIN2, RESET2, LED2);
      reset_time2 = millis();
      LED_status2 = 0;
    }
  }

  // turn the sand timer by hand
  pos = analogRead(FEEDBACK);
  if(sand_timer_on && pos > TURN){
    sand_timer_on = false;
    turn_sandtimer(myservo, SERVO_PIN, OFF, LED1);
    Serial.print("C"); // send B to processing
    LED_status1 = 2;
  }

  pos2 = analogRead(FEEDBACK2);
  if(sand_timer_on2 && pos2 < TURN2){
    sand_timer_on2 = false;
    turn_sandtimer(myservo2, SERVO_PIN2, OFF2, LED2);
    Serial.print("c"); // send B to processing
    LED_status2 = 2;
  }

  // return the reset timer
  if( reset_time >0 && (millis() - reset_time > RESET_TIME)){
    reset_time = 0;
    turn_sandtimer(myservo, SERVO_PIN, OFF, LED1);
    LED_status1 = 2;
  }

  if( reset_time2 >0 && (millis() - reset_time2 > RESET_TIME)){
    reset_time2 = 0;
    turn_sandtimer(myservo2, SERVO_PIN2, OFF2, LED2);
    LED_status2 = 2;
  }

  if(LED_status1 == 0){
    digitalWrite(LED1, HIGH);
  }
  if(LED_status2 == 0){
    digitalWrite(LED2, HIGH);
  }
  if(LED_status1 == 1){
  }
  if(LED_status1 == 2){
    if (brightness1 >= 0 && brightness1 <= 255) analogWrite(LED1, brightness1); 
    brightness1 += fadeAmount_slow1;
    if (brightness1 < 0 || brightness1 > 255) fadeAmount_slow1 = -fadeAmount_slow1;
  }
  if(LED_status2 == 2){
    if (brightness2 >= 0 && brightness2 <= 255) analogWrite(LED2, brightness2);  
    brightness2 += fadeAmount_slow2;
    if (brightness2 < 0 || brightness2 > 255) fadeAmount_slow2 = -fadeAmount_slow2; 
  }
  if(LED_status1 == 3){
    if (brightness1 >= 0 && brightness1 <= 255) analogWrite(LED1, brightness1); 
    brightness1 += fadeAmount_fast1;
    if (brightness1 < 0 || brightness1 > 255) fadeAmount_fast1 = -fadeAmount_fast1;
  }
  if(LED_status2 == 3){
    if (brightness2 >= 0 && brightness2 <= 255) analogWrite(LED2, brightness2);  
    brightness2 += fadeAmount_fast2;
    if (brightness2 < 0 || brightness2 > 255) fadeAmount_fast2 = -fadeAmount_fast2; 
  }
  delay(20);
}

void turn_sandtimer(Servo servo, int servo_pin, int to_pos, int led){
  servo.attach(servo_pin); // attach the servo
  for (int i=0;i<100;i++){
    servo.write(to_pos);             
    delay(10);
    if(i%20==0) digitalWrite(led, LOW);
    if(i%20==10) digitalWrite(led, HIGH);
  }
  servo.detach(); // detach the servo, so you can turn it by hand
}
