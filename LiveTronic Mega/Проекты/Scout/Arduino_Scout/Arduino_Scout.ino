#define N 2
#define L 1
#define R 2
#include <Servo.h>

Servo servoH, servoVL, servoVR, servoGV, servoGR, servoG; // VerticalLeft, VerticalRight, Horizontal, GripperVertical, GripperRotation, Gripper;
int degreeH = 95, dminH = 10, dmaxH = 170, degreeVL = 90, dminVL = 45, dmaxVL = 135, degreeVR = 90, dminVR = 30, dmaxVR = 150,
    degreeGV = -degreeVL+180, dminGV = 10, dmaxGV = 170, degreeGR = 80, dminGR = 10, dmaxGR = 170, degreeG = 90, dminG = 30, dmaxG = 90;
int pinH = 2, pinVL = 3, pinVR = 4, pinGV = 5, pinGR = 6, pinG = 7;
int inc = 2;
unsigned char input[N];
//unsigned char input_next;
int buzzPin = 44;
int SR = 0, SL = 0;

int DIR1 = 43;  // Цифровые порты, отвечающие за направление движения
int DIR2 = 47;
int PWM1 = 9;  // ШИМ порты, отвечающие за скорость вращения мотора
int PWM2 = 10;
int EN1 = 42;   // Цифровые порты включения/выключения драйверов двигателей
int EN2 = 48;

// Функция установки скорости на левый (L) или правый (R) мотор
// Диапазон допустимых значений скорости Speed [-255; 255]
void SetSpeed(int Motor, int Speed) {
  int DIR, PWM;
  Speed = constrain(Speed, -255, 255);
  if (Motor == L) {DIR = DIR1; PWM = PWM1;}
  if (Motor == R) {DIR = DIR2; PWM = PWM2;}
  analogWrite(PWM, abs(Speed));
  if (Speed < 0) digitalWrite(DIR, 0);
  else if (Speed >= 0) digitalWrite(DIR, 1);
}

void setup() {
  pinMode(24, INPUT_PULLUP);  // Устанавливаем кнопку D24 в режим ввода с подтягивающим резистором
  pinMode(EN1, OUTPUT);       // Устанавливаем порты драйвера в режим вывода
  pinMode(EN2, OUTPUT);
  pinMode (PWM1, OUTPUT);
  pinMode (DIR1, OUTPUT);
  pinMode (PWM2, OUTPUT);
  pinMode (DIR2, OUTPUT);
  digitalWrite(EN1, 1);       // Включаем драйвер
  digitalWrite(EN2, 1);
  
  pinMode(buzzPin, OUTPUT);
  servoH.attach(pinH);
  servoVL.attach(pinVL);
  servoVR.attach(pinVR);
  servoGV.attach(pinGV);
  servoGR.attach(pinGR);
  servoG.attach(pinG);
  servoVL.write(degreeVL);
  servoVR.write(degreeVR);
  servoH.write(degreeH);
  servoGV.write(degreeGV);
  servoGR.write(degreeGR);
  servoG.write(degreeG);
  randomSeed(analogRead(0));
  Serial2.begin(9600);
}

void loop() {
  if (Serial2.available() >= N) {
    for (int i = 0; i < N; i++)
      input[i] = Serial2.read();
    switch (input[0]) {
      case '1': // Первый сервопривод (горизонтальный, servoH)
        switch (input[1]) {
          case 'i': // increase - увеличиваем угол поворота сервопривода
            degreeH += inc;
            break;
          case 'd': // decrease - уменьшаем угол поворота сервопривода
            degreeH -= inc;
            break;
        }
        break;
      case '2': // Второй сервопривод (левый вертикальный, servoVL)
        switch (input[1]) {
          case 'i':
            degreeVL += inc;
            break;
          case 'd':
            degreeVL -= inc;
            break;
        }
        break;
      case '3': // Третий сервопривод (правый вертикальный, servoVR)
        switch (input[1]) {
          case 'i':
            degreeVR += inc;
            break;
          case 'd':
            degreeVR -= inc;
            break;
        }
        break;
      case '4': // Четвёртый сервопривод (захват поворотный, servoGR)
        switch (input[1]) {
          case 'i':
            degreeGR += inc;
            break;
          case 'd':
            degreeGR -= inc;
            break;
        }
        break;
      case '5': // Пятый сервопривод (захват вертикальный, servoGV)
        switch (input[1]) {
          case 'i':
            degreeGV += inc;
            break;
          case 'd':
            degreeGV -= inc;
            break;
        }
        break;
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
      case '8': // Параллельный перенос (левый вертикальный + захват вертикальный, servoVL + servoGV)
        switch (input[1]) {
          case 'i':
            degreeVL += inc;
            degreeGV = -degreeVL+180;
            break;
          case 'd':
            degreeVL -= inc;
            degreeGV = -degreeVL+180;
            break;
        }
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
      case 'v': // Голос!
        make_voice();
        break;
        
    }
    SetSpeed(L, SL);
    SetSpeed(R, SR);
    degreeH = constrain(degreeH, dminH, dmaxH);
    degreeVL = constrain(degreeVL, dminVL, dmaxVL);
    degreeVR = constrain(degreeVR, dminVR, dmaxVR);
    degreeGV = constrain(degreeGV, dminGV, dmaxGV);
    degreeGR = constrain(degreeGR, dminGR, dmaxGR);
    degreeG = constrain(degreeG, dminG, dmaxG);
    servoH.write(degreeH); // Write values to servos
    servoVL.write(degreeVL);
    servoVR.write(degreeVR);
    servoGV.write(degreeGV);
    servoGR.write(degreeGR);
    servoG.write(degreeG);
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

  void make_voice() {
    for (int i = 0; i < random(5, 15); i++) {
      buzz(buzzPin, random(3200 - 500 * i, 3600 - 500 * i), random(30, 100));
      delay(random(5, 20));
    }
  }
