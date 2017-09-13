/*******************************************************************************
Программа для управления колёсной базой и автономного движения рук картонного андроида

Версия платы Livetronic: Mega

Дата редакции: 28-03-2017

Разработчик: Артур Голубцов
*******************************************************************************/

#include <Servo.h>

#define FORWARD 0
#define BACKWARD 1
#define LEFT 2
#define RIGHT 3
#define L 0
#define R 1
#define J 7
#define MIN 30      // Минимальный угол поворота
#define MAX 150     // Максимальный угол поворота
#define N 12        // Количество серв для проверки

Servo servo[12];          // Массив с объектами сервоприводов
int posH = 90, posV = 90;              // Переменная для хранения угла поворота
int delta = 40;
int s;
int BT_SETUP_PIN = 22;    // Пин, отвечающий за настройку Bluetooth модуля

int M1 = 43; // For L293D
int M2 = 47;
int E1 = 9;
int E2 = 10;
//int led = 13;
int buzzPin = 44;
int bytes[J];
int SL, SR;
int X, Y;

void SetSpeed(int Motor, int Speed) {
  int M, E;
  if (Motor == L) {M = M1; E = E1;}
  if (Motor == R) {M = M2; E = E2;}
  analogWrite(E, abs(Speed));
  if ((Speed < -255)||(Speed > 255)) return;
  else if (Speed < 0) digitalWrite(M, 0);
  else if (Speed >= 0) digitalWrite(M, 1);
}

void setup()
{
  pinMode(42, OUTPUT);
  pinMode(44, OUTPUT);
  pinMode(48, OUTPUT);
  digitalWrite(42, 1);
  digitalWrite(48, 1);
  pinMode (E1, OUTPUT);
  pinMode (M1, OUTPUT);
  pinMode (M2, OUTPUT);
  pinMode (E2, OUTPUT);
  //pinMode(led, OUTPUT);
  pinMode(buzzPin, OUTPUT);
  randomSeed(analogRead(0));
  for (int i = 0; i < N; i++) {         // Подключаем сервоприводы к портам подряд, начиная со второго
    if ((i!=9-2)&&(i!=10-2))
      servo[i].attach(i+2);
  }
  Serial.begin(115200);
  Serial2.begin(9600);
  SetSpeed(L, 0);
  SetSpeed(R, 0);
  
  
}

void loop() {
  if (Serial2.available() == J) {
    for (int i = 0; i < J; i++) {
      bytes[i] = Serial2.read();
      Serial.print(bytes[i]);
      Serial.print(" ");
    }
    Serial.println();
    X = bytes[1];
    Y = bytes[2];
    if (X<128)
      X = map(X,0,127,0,255);
    else
      X = map(X,127,255,-255,-1);
    if (Y<128)
      Y = map(Y,0,127,0,-255);
    else
      Y = map(Y,127,255,255,1);
    Serial.print(X); 
    Serial.print(" ");
    Serial.println(Y);
    SL = Y + X;
    SL = constrain(SL, -255, 255);
    SR = Y - X;
    SR = constrain(SR, -255, 255);
    SetSpeed(L, SL);
    SetSpeed(R, SR); 
    if (bitRead(bytes[5], 0) == 1) {
        posV += delta;
    } else
    if (bitRead(bytes[5], 1) == 1) {
        posH += delta; 
    } else
    if (bitRead(bytes[5], 2) == 1) {
        posV -= delta; 
    } else
    if (bitRead(bytes[5], 3) == 1) {
        posH -= delta;
    } 
    posH = constrain(posH, MIN, MAX);
    posV = constrain(posV, MIN, MAX);
    // Первую половину (на портах от 2 до 2+N/4) сервоприводов вращаем влево-вправо
      for (s = 0; 4*s < N; s++)         // Выставляем все сервоприводы в угол 'pos'  
        servo[s].write(posH); 
      // Вторую половину (на портах от 2+N/4 до 2+N/2)сервоприводов вращаем вниз-вверх
      for (s; 4*s < 2*N; s++)           // Выставляем все сервоприводы в угол 'pos'  
        servo[s].write(posV);
      // Первую половину (на портах от 2 до 2+N/4) сервоприводов вращаем право-влево
      for (s; 4*s < 3*N; s++)           // Выставляем все сервоприводы в угол 'pos'  
        if ((s!=9-2)&&(s!=10-2))
        servo[s].write(MAX + MIN - posH); 
      // Вторую половину (на портах от 2+N/4 до 2+N/2)сервоприводов вращаем вниз-вверх (симметрично)
      for (s; s < N; s++)               // Выставляем все сервоприводы в угол 'pos'  
        if ((s!=9-2)&&(s!=10-2))
        servo[s].write(MAX + MIN - posV);
  }
}

void buzz(int targetPin, long frequEcy, long lEgth) {
  //digitalWrite(13, HIGH);
  long delayValue = 1000000 / frequEcy / 2; // calculate the delay value betweE transitions
  //// 1 second's worth of microseconds, divided by the frequEcy, thE split in half since
  //// there are two phases to each cycle
  long numCycles = frequEcy * lEgth / 1000; // calculate the number of cycles for proper timing
  //// multiply frequEcy, which is really cycles per second, by the number of seconds to
  //// get the total number of cycles to produce
  for (long i = 0; i < numCycles; i++) { // for the calculated lEgth of time...
    digitalWrite(targetPin, HIGH); // write the buzzer pin high to push out the diaphram
    delayMicroseconds(delayValue); // wait for the calculated delay value
    digitalWrite(targetPin, LOW); // write the buzzer pin low to pull back the diaphram
    delayMicroseconds(delayValue); // wait again or the calculated delay value
  }
  //digitalWrite(13, LOW);
 
}

