/*******************************************************************************
Программа для общего тестирования работоспособности платы Livetronic Mega

Версия платы Livetronic: Mega (RoboMega5)

Дата редакции: 25-05-2017

Разработчик: Артур Голубцов
*******************************************************************************/

#define BT_NAME "LTMega 000016"               // Имя bluetooth модуля (можно изменять)
#define BT_PSWD "1234"                        // Пароль bluetooth модуля (можно изменять)
#define BT_SETUP_PIN 22                       // Номер порта для настройки bluetooth модуля

#define BUZZ_PIN 44                           // Номер порта, к которому подключена пищалка

#define MIN 0                                 // Минимальный угол поворота сервоприводов
#define MAX 180                               // Максимальный угол поворота сервоприводов
#define N 12                                  // Количество сервоприводов для проверки

#define L 1                                   // Обозначение левого мотора
#define R 2                                   // Обозначение правого мотора
#define DIR1 43                               // Цифровые порты, отвечающие за направление движения
#define DIR2 47
#define PWM1 9                                // ШИМ порты, отвечающие за скорость вращения мотора
#define PWM2 10
#define EN1 42                                // Цифровые порты включения/выключения драйверов двигателей
#define EN2 48


#include "pitches.h"                          // Файл с частотами нот
#include <Servo.h>                            // Библиотека для работы с сервоприводами

void setup() {
  Serial.begin(115200);                       // Подключаемся к основному последовательному порту платы
  Serial2.begin(9600);                        // Подключаемся к последовательному порту на котором расположен HC-05 модуль
  Serial.println("This is the perfomance test for Livetronic Mega. Please, follow the instructions carefully!");
  Serial.println("If you are ready to start - just send any character [or PRESS ENTER].\n");
  while (Serial.available())                  // Чистим буфер Serial, если там что-то есть
    Serial.read();
  while (!Serial.available());                // Ждём, пока придёт символ
  while (Serial.available())                  // Чистим буфер Serial, если там что-то есть
    Serial.read();
  BluetoothSetup();
  BuzzTest();
  MotorsAndServosTest();
  DigitalAndAnalogReadTest();
  Serial.println("\nCongratulations! All tests are completed!");
  
}

void loop() {
  digitalWrite(13, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);              // wait for a second
  digitalWrite(13, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);              // wait for a second
}

// Функция настройки bluetooth модуля
void BluetoothSetup() {
  pinMode(BT_SETUP_PIN, OUTPUT);              // Режим работы порта - вывод
  digitalWrite(BT_SETUP_PIN, HIGH);           // Устанавливаем высокий сигнал режима работы порта
  Serial.println("1. Bluetooth module is setting up.");
  Serial2.print("AT+NAME=");                  // Команда для изменения имени модуля
  Serial2.println(BT_NAME);                   // Меняем имя модуля
  Serial2.print("AT+PSWD=");                  // Команда смены пароля
  Serial2.println(BT_PSWD);                   // Устанавливаем пароль
  while (Serial.available())                  // Чистим буфер Serial, если там что-то есть
    Serial.read();
  Serial.printf("Bluetooth module is set up with NAME = %s and PSWD = %s.\n", BT_NAME, BT_PSWD);  // Если функция не работает, нужно заменить файл Print.h (инструкция в папке с этим скетчем)
  Serial.print("Try to find the bluetooth device with these parameters.\nSend any character to continue [or PRESS ENTER].\n\n");
  while (Serial.available())                  // Чистим буфер Serial, если там что-то есть
    Serial.read();
  while (!Serial.available());                // Ждём, пока придёт символ
  while (Serial.available())                  // Чистим буфер Serial, если там что-то есть
    Serial.read();  
}

// Функция проверки пьезопищалки
void BuzzTest() {
  // Настройка последовательности нот для мелодии
  int melody[] = {
    NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3, 0, NOTE_B3, NOTE_C4
  };
  // Длительность нот: 4 = четверть, 8 = восьмая, и т.д.:
  int noteDurations[] = {
    4, 8, 8, 4, 4, 4, 4, 4
  };
  Serial.println("2. Buzzer test.");  
  // Идём последовательно по нотам мелодии:
  for (int thisNote = 0; thisNote < 8; thisNote++) {
  
    // Для расчёта длительности звучания ноты делим 1000мс на тип длительности нот
    // Например четверть будет по длительности в мс равна 1000 / 4, восьмая = 1000/8, и т.д.
    int noteDuration = 1000 / noteDurations[thisNote];
    tone(BUZZ_PIN, melody[thisNote], noteDuration);
  
    // Чтобы отличать ноты друг от друга, нужно раздельть их паузами
    // Длительность ноты + 30% подходит вполне неплохо:
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    
    // Останавливаем проигрывание звука
    noTone(BUZZ_PIN);
  }
  Serial.print("Buzzer test complete. Did you hear the melody? If not - there is something wrong with the buzzer on the pin 44.\nSend any character to continue [or PRESS ENTER].\n\n");
  while (Serial.available())                  // Чистим буфер Serial, если там что-то есть
    Serial.read();
  while (!Serial.available());                // Ждём, пока придёт символ
  while (Serial.available())                  // Чистим буфер Serial, если там что-то есть
    Serial.read();
}

// Функция установки скорости на левый (L) или правый (R) мотор
// Диапазон допустимых значений скорости Speed [-255; 255]
void SetSpeed(int Motor, int Speed) {
  int DIR, PWM;
  if (Motor == L) {DIR = DIR1; PWM = PWM1;}
  if (Motor == R) {DIR = DIR2; PWM = PWM2;}
  analogWrite(PWM, abs(Speed));
  if ((Speed < -255)||(Speed > 255)) return;
  else if (Speed < 0) digitalWrite(DIR, 0);
  else if (Speed >= 0) digitalWrite(DIR, 1);
}

// Функция для проверки работы драйверов двигателей и силовых портов D2 - D13 
void MotorsAndServosTest() {
  Serial.println("3. Motors and Servos test. Please, connect battery or power supply and turn on the power switch!");
  Servo servo[12];                            // Массив с объектами сервоприводов
  int pos = 0;                                // Пересенная для хранения угла поворота
  // Подключаем сервоприводы ко всем силовым портам (D2 - D13), кроме D9 и D10, так как они используются для управления моторами
  for (int i = 0; i < N; i++) {
    if ((i+2 != PWM1)&&(i+2 != PWM2))
      servo[i].attach(i+2);
  }
  pinMode(EN1, OUTPUT);                       // Устанавливаем порты драйвера в режим вывода
  pinMode(EN2, OUTPUT);
  pinMode(PWM1, OUTPUT);
  pinMode(DIR1, OUTPUT);
  pinMode(PWM2, OUTPUT);
  pinMode(DIR2, OUTPUT);
  digitalWrite(EN1, 1);                       // Включаем драйвер
  digitalWrite(EN2, 1);
  // Выдаём на моторы неполную мощность для проверки совместной генерации ШИМ и работы библиотеки сервоприводов
  SetSpeed(L, 100);           
  SetSpeed(R, 100);
  Serial.println("Take a servo and successively try it on the pins D2 - D8 and D11 - D13. It must rotate in both directions from MIN to MAX angle and vice versa.");
  Serial.println("Then take a motor and try it on the M1 and M2 outputs. It must rotate with about a half of its maximum speed.");
  Serial.println("If a servo or a motor doesn't work on some output - check its soldering, it can be weak.") ;
  Serial.println("When you are ready to the next test - just send any character to continue [or PRESS ENTER].\n");
  while (Serial.available())                  // Чистим буфер Serial, если там что-то есть
    Serial.read();
  while (true) { 
    if (Serial.available())                   // Если пользователь отправил символ - выходим из цикла
      break;                 
    for(pos = MIN; pos <= MAX; pos += 1)      // Вращаем вал сервопривода от угла MIN до угла MAX с шагом 1 градус
    {
      for (int i = 0; i < N; i++)             // Выставляем все сервоприводы в угол 'pos'  
        servo[i].write(pos);            
      delay(15);                              // Ждём 15мс для того, чтобы сервопривод успел занять требуемое положение
    } 
    for(pos = MAX; pos>=MIN; pos-=1)          // Вращаем вал сервопривода от угла MAX до угла MIN с шагом 1 градус
    { 
      for (int i = 0; i < N; i++)             // Выставляем все сервоприводы в угол 'pos'
        servo[i].write(pos);            
      delay(15);                              // Ждём 15мс для того, чтобы сервопривод успел занять требуемое положение
    } 
  }
  servo[11].detach();
  while (Serial.available())                  // Чистим буфер Serial, если там что-то есть
    Serial.read();  
}

// Функция для проверки работоспособности кнопок, цифровых и аналоговых портов:
// D18-D21, D24 (кнопка 1), D25 (кнопка 2), A0-A7, A8 (заряд аккумулятора).
void DigitalAndAnalogReadTest() {
  Serial.println("4. Digital and analog read test for the next pins: D18-D21, D24 (button 1), D25 (button 2), A0-A7, A8 (battery charge).");
  Serial.println("You will get all values from theese pins in loop. Take some digital sensor (for example, line sensor) and successively try to read values from pins. Also try the buttons on D24 and D25.");
  Serial.println("If a sensor or a button doesn't work on some input - check its soldering, it can be weak.") ;
  Serial.println("If you are ready to start the test - just send any character [or PRESS ENTER]");
  while (Serial.available())                  // Чистим буфер Serial, если там что-то есть
    Serial.read();
  while (!Serial.available());                // Ждём, пока придёт символ
  while (Serial.available())                  // Чистим буфер Serial, если там что-то есть
    Serial.read();
  Serial.println("When you are ready to finish the test - just send any character [or PRESS ENTER].");   
  delay(2000);
  // Устанавливаем порты в режим чтения
  pinMode(18, INPUT);
  pinMode(19, INPUT);
  pinMode(20, INPUT);
  pinMode(21, INPUT);
  pinMode(24, INPUT_PULLUP);
  pinMode(25, INPUT_PULLUP);
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  pinMode(A3, INPUT);
  pinMode(A4, INPUT);
  pinMode(A5, INPUT);
  pinMode(A6, INPUT);
  pinMode(A7, INPUT);
  pinMode(A8, INPUT); 
  while (Serial.available())                  // Чистим буфер Serial, если там что-то есть
    Serial.read(); 
  while (true) { 
    if (Serial.available())                   // Если пользователь отправил символ - выходим из цикла
      break; 
    Serial.println(" D18 | D19 | D20 | D21 | D24 | D25 |  A0  |  A1  |  A2  |  A3  |  A4  |  A5  |  A6  |  A7  |  A8  ");
    Serial.printf(" %3d | %3d | %3d | %3d | %3d | %3d |", digitalRead(18), digitalRead(19), digitalRead(20), digitalRead(21), digitalRead(24), digitalRead(25));
    Serial.printf(" %4d | %4d | %4d | %4d | %4d | %4d | %4d | %4d | %4d \n", analogRead(A0), analogRead(A1), analogRead(A2), analogRead(A3), analogRead(A4), analogRead(A5), analogRead(A6), analogRead(A7), analogRead(A8)); 
    delay(200);
  }
  while (Serial.available())                  // Чистим буфер Serial, если там что-то есть
    Serial.read();
}

