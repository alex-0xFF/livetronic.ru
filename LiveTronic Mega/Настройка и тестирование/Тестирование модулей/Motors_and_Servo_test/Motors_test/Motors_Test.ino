/*******************************************************************************
Программа для проверки работоспособности драйверов двигателей

Версия платы Livetronic: Mega (RoboMega5)

Дата редакции: 22-03-2017

Разработчик: Артур Голубцов
*******************************************************************************/

#define L 1
#define R 2

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
  if (Motor == L) {DIR = DIR1; PWM = PWM1;}
  if (Motor == R) {DIR = DIR2; PWM = PWM2;}
  analogWrite(PWM, abs(Speed));
  if ((Speed < -255)||(Speed > 255)) return;
  else if (Speed < 0) digitalWrite(DIR, 0);
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
}

void loop() {
  while(digitalRead(24));   // Ждём нажатия кнопки
  SetSpeed(L, 255);         // Едем полный вперёд (проверка работы моторов в режиме полной мощности)
  SetSpeed(R, 255);
  delay(500);
  while(digitalRead(24));   // Опять ждём нажатия кнопки
  SetSpeed(L, -150);        // Едем чуть медленнее назад (проверка работы драйвера с ШИМ сигналом управления 
  SetSpeed(R, -150);
  delay(500);
  while(digitalRead(24));   // Угадайте, что в этот раз
  SetSpeed(L, -100);        // Проверка работы моторов в разных направлениях
  SetSpeed(R, 100);
  delay(500);
  while(digitalRead(24));   // ...
  SetSpeed(L, 50);
  SetSpeed(R, -50);
  delay(500);
  while(digitalRead(24));   // Полная остановка моторов
  SetSpeed(L, 0);
  SetSpeed(R, 0);
  delay(500);
}
