#include <Servo.h>
#include <NewPing.h>
#include <SharpIR.h>

// Pins of motor 1
#define mpin00 5
#define mpin01 6

// Pins of motor 2
#define mpin10 3
#define mpin11 11

//Max read distance of the sonar.
#define MAX_DISTANCE 1000 
//Maximum distance allowed before collision
#define C_DST 20


Servo srv;
NewPing sonar(A5, A4, MAX_DISTANCE);
SharpIR (1, A0);
SharpIR laser2(1, A1);
int pos = 0; 
String lastAction="";
int distance = 0;laser1

void setup() {
 // Configuration of motor pins as output, initially 0
 Serial.begin(9600);
 digitalWrite(mpin00, 0);
 digitalWrite(mpin01, 0);
 digitalWrite(mpin10, 0);
 digitalWrite(mpin11, 0);
 pinMode (mpin00, OUTPUT);
 pinMode (mpin01, OUTPUT);
 pinMode (mpin10, OUTPUT);
 pinMode (mpin11, OUTPUT);
 // LED pin
 pinMode(13, OUTPUT);
 // Servo pin
 srv.attach(8);
 srv.write(90);
}

void StartMotor (int m1, int m2, int forward, int speed)
{

 if (speed==0) // stop
 {
 digitalWrite(m1, 0); 
 digitalWrite(m2, 0);
 }
 else
 {
 if (forward)
 {
 digitalWrite(m2, 0);
 analogWrite (m1, speed); // use PWM
 }
 else
 {
 digitalWrite(m1, 0);
 analogWrite(m2, speed);
 }
 }
}

void test(){
 // Warning code
 // Slow blink
 for (int i=0; i<10; i++)
 {
 digitalWrite(13, 1);
 delay(200);
 digitalWrite(13, 0);
 delay(200);
 }
 // Fast blink. Remove the USB cable!!!!
 for (int i=0; i<10; i++)
 {
 digitalWrite(13, 1);
 delay(100);
 digitalWrite(13, 0);
 delay(100);
 }
 digitalWrite(13, 1);
 // Start the servo motor
 //playWithServo();
 // Now start the DC motors
 StartMotor (mpin00, mpin01, 0, 128);
 StartMotor (mpin10, mpin11, 0, 128);

 delay (500); // How long the motors are on
 delayStop(500); // How long the motors are off

 StartMotor (mpin00, mpin01, 1, 128);
 StartMotor (mpin10, mpin11, 1, 128);

 delay (500);
 delayStop(500); 
 StartMotor (mpin00, mpin01, 0, 128);
 StartMotor (mpin10, mpin11, 1, 128);

 delay (500);
 delayStop(500);

 StartMotor (mpin00, mpin01, 1, 128); 
 StartMotor (mpin10, mpin11, 0, 128);

 delay (500);
 delayStop(500);
 
}

void cForward(){
  int dist = readPing();
  while((dist > C_DST || dist == 0) && laser1.getDistance() >= 11 && laser2.getDistance() >= 11 ){
      //Serial.println("aici");
      StartMotor (mpin00, mpin01, 0, 64); 
      StartMotor (mpin10, mpin11, 0, 64);
      delay(10);
      dist = readPing(); 
  } 
  Serial.println("Forwd: " +dist);
    delayStop(500); 
}

void delayStop(int ms){
 StartMotor (mpin00, mpin01, 0, 0);
 StartMotor (mpin10, mpin11, 0, 0);
 delay(ms);
}

void playWithServo(){
  int maxDist = 0;
  for(int i = 180; i >= 0; i-=18){
    srv.write(i);
    //delay(10);
    distance = readPing();
   // Serial.println(distance);

    if(distance > C_DST && maxDist < distance){
      pos = i;
      maxDist = distance;
      //It means we get too close to an object in front, so we need to start actions to avoid it.
    }
    //We arrive here only when there is no collision, but we also need to check that in the event a collison is about to happen, that we can turn
    //Still debating if we need it
    
  }
  if(maxDist != 0){
      srv.write(90);
      avoidOBS();
      return;
  }
  srv.write(90);
  rotateRob();    
  playWithServo();
}

void rotateRob(){
  delayStop(500);
  StartMotor (mpin00, mpin01, 0, 64); 
  StartMotor (mpin10, mpin11, 1, 64);
  delay(500);
  delayStop(500);
}
void changeWay(){
  if(pos < 90){
   turnRight();
  }
  if(pos > 90){
    turnLeft();
  }
}

void avoidOBS(){
  delayStop(500);
  //goes backwards
  StartMotor (mpin00, mpin01, 1, 128); 
  StartMotor (mpin10, mpin11, 1, 128);
  delay(250);
  delayStop(500);
  changeWay();
}

void turnRight(){
   for(int i = pos ; i <= 90; i++){
      StartMotor (mpin00, mpin01, 0, 42); 
     StartMotor (mpin10, mpin11, 1 ,42);
     delay(15);
  } 
  delay(100);
  StartMotor (mpin00, mpin01, 0, 64); 
  StartMotor (mpin10, mpin11, 0, 64);
}

void turnLeft(){

 for(int i = pos ; i >= 90; i--){
   StartMotor (mpin00, mpin01, 1, 42); 
   StartMotor (mpin10, mpin11, 0, 42);
    delay(15);
  }
  delay(100);
  StartMotor (mpin00, mpin01, 0, 64); 
  StartMotor (mpin10, mpin11, 0, 64);
}

int readPing() {
  delay(70);
  unsigned int uS = sonar.ping();
  int cm = uS/US_ROUNDTRIP_CM;
  return cm;
}

void loop() { 
 //test();
 cForward();
 playWithServo(); 
 //Serial.println(laser1.getDistance());
}
