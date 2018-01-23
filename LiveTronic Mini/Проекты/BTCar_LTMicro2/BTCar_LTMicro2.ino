#define J 7
#define STEP 1
//#include <TimerOne.h>

#include <Servo.h>

Servo steer;

int pos0 = 90;
int pos = pos0;
int sign = 0;
unsigned char bytes[J];

void setup() {
  //Timer1.initialize(10000);
  //Timer1.attachInterrupt(rotate);
  Serial.begin(9600);
  Serial.flush();
  //Serial.println("Test");
  pinMode(3, OUTPUT);
  pinMode(2, OUTPUT);
  steer.attach(6);
  steer.write(pos0);
  /*delay(1000);
  for (int i = 0; i <= 90; i+=10) {
    Serial.println(i);
    steer.write(i);
    delay(500);
  }*/
  //analogWrite(6,128);
}

void loop() {
  if (Serial.available() >= J) {
    for (int i = 0; i < J; i++)
      bytes[i] = Serial.read();
    digitalWrite(2,(bytes[2]-128)>0);
    analogWrite(3, abs((bytes[2])*2));
    if (bitRead(bytes[5], 3) == 1) {
       steer.write(pos0+42); 
    } else
    //else Timer1.stop();
    if (bitRead(bytes[5], 1) == 1) {
      steer.write(pos0-42); 
    } else
      steer.write(pos0);
  }
  
}
