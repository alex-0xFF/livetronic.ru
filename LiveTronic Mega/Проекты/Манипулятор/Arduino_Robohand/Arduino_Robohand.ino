/*******************************************************************************
Программа для управления большим манипулятором со смартфона на операционной системе Android

Версия платы Livetronic: Mega (RoboMega5)

Дата редакции: 17-05-2017

Разработчик: Артур Голубцов
*******************************************************************************/


#define N 2 // Количество байт в пакете передачи данных
#include <Servo.h>

// Сервоприводы servoX: H-Horizontal (горизонтальный), VL-VerticalLeft (вертикальный левый), VR-VerticalRight (вертикальный правый), GV-GripperVertical (вертикальный поворот захвата), GR-GripperRotation (горизонтальный поворот захвата), G-Gripper (захват);
Servo servoH, servoVL, servoVR, servoGV, servoGR, servoG;

// degreeX - начальный угол сервопривода, dminX - минимальный угол поворота сервопривода, dmaxX - максимальный угол поворота сервопривода
int degreeH = 90, dminH = 10, dmaxH = 170, degreeVL = 30, dminVL = 10, dmaxVL = 120, degreeVR = 160, dminVR = 50, dmaxVR = 170,
    degreeGV = -degreeVL+90, dminGV = 10, dmaxGV = 170, degreeGR = 90, dminGR = 10, dmaxGR = 170, degreeG = 90, dminG = 70, dmaxG = 110;

// pinX - номера портов для подключения сервоприводов к манипулятору  
int pinH = 2, pinVL = 3, pinVR = 4, pinGV = 5, pinGR = 6, pinG = 7; 

int inc = 2;              // Приращение угла за один такт поворота
unsigned char input[N];   // Массив для хранения входных данных

void setup()
{
  // Подключаем сервоприводы к портам контроллера
  servoH.attach(pinH);
  servoVL.attach(pinVL);
  servoVR.attach(pinVR);
  servoGV.attach(pinGV);
  servoGR.attach(pinGR);
  servoG.attach(pinG);
  // Выставляем их в начальное положение
  servoVL.write(degreeVL);
  servoVR.write(degreeVR);
  servoH.write(degreeH);
  servoGV.write(degreeGV);
  servoGR.write(degreeGR);
  servoG.write(degreeG);
  // Запускаем работу Bluetooth-модуля
  Serial2.begin(9600);
}

void loop() {
  if (Serial2.available() == N) {
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
            degreeGV = -degreeVL+90;
            break;
          case 'd':
            degreeVL -= inc;
            degreeGV = -degreeVL+90;
            break;
        }
        break;
      case 's': // Изменение скорости изменения угла сервопривода
        inc = input[1];
        break;
    }
    // Ограничиваем области поворотов сервоприводов
    degreeH = constrain(degreeH, dminH, dmaxH);
    degreeVL = constrain(degreeVL, dminVL, dmaxVL);
    degreeVR = constrain(degreeVR, dminVR, dmaxVR);
    degreeGV = constrain(degreeGV, dminGV, dmaxGV);
    degreeGR = constrain(degreeGR, dminGR, dmaxGR);
    degreeG = constrain(degreeG, dminG, dmaxG);
    // Записываем изменённые значения углов
    servoH.write(degreeH);
    servoVL.write(degreeVL);
    servoVR.write(degreeVR);
    servoGV.write(degreeGV);
    servoGR.write(degreeGR);
    servoG.write(degreeG);
  }
}
