#define N 2
#define L 1
#define R 2
#include <Servo.h>

Servo servoG;
int pinG = 7;

int degreeG = 90, dminG = 30, dmaxG = 90;

int inc = 2;
unsigned char input[N];
int SR = 0, SL = 0;

int DIR1 = 2;  // Цифровые порты, отвечающие за направление движения
int DIR2 = 4;
int PWM1 = 3;  // ШИМ порты, отвечающие за скорость вращения мотора
int PWM2 = 5;

// Функция установки скорости на левый (L) или правый (R) мотор
// Диапазон допустимых значений скорости Speed [-255; 255]
void SetSpeed(int Motor, int Speed) {
  int DIR, PWM;
  Speed = constrain(Speed, -255, 255);
  if (Motor == R) {DIR = DIR1; PWM = PWM1;}
  if (Motor == L) {DIR = DIR2; PWM = PWM2;}
  analogWrite(PWM, abs(Speed));
  if (Speed < 0) digitalWrite(DIR, 0);
  else if (Speed >= 0) digitalWrite(DIR, 1);
}

void setup() {
  pinMode (PWM1, OUTPUT);
  pinMode (DIR1, OUTPUT);
  pinMode (PWM2, OUTPUT);
  pinMode (DIR2, OUTPUT);

  SetSpeed(R, 0);
  SetSpeed(L, 0);
    
  servoG.attach(pinG);  
  servoG.write(degreeG);
  Serial.begin(9600);
}

void loop() {
  if (Serial.available() >= N) {
    for (int i = 0; i < N; i++)
      input[i] = Serial.read();
    switch (input[0]) {      
      case '6': // Шестой сервопривод (захват, servoG)
        switch (input[1]) {
          case 'i':
            degreeG += inc;
            break;
          case 'd':
            degreeG -= inc;
            break;
          case 'a':
            degreeG = dminG;
            break;
          case 'z':
            degreeG = dmaxG;
            break;
        }
      case '7': // Шестой сервопривод точное положение (захват, servoG)
        degreeG = dminG + input[1];
        break;      
      case 's': // Изменение скорости изменения угла сервопривода
        inc = input[1];
        break;
      case 'L': // Изменение скорости левого мотора
        SL = (input[1] - 127)*2;
        break;
      case 'R': // Изменение скорости правого мотора
        SR = (input[1] - 127)*2;
        break;              
    }
    SetSpeed(L, SL);
    SetSpeed(R, SR);    
    degreeG = constrain(degreeG, dminG, dmaxG);
    servoG.write(degreeG);
  }
}
