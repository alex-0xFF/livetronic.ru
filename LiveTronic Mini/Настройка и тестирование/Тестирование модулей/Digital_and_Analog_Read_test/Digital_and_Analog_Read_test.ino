/*******************************************************************************
Программа для проверки работоспособности кнопок, цифровых и аналоговых портов:
D18-D21, D24 (кнопка 1), D25 (кнопка 2), A0-A7, A8 (заряд аккумулятора).

Версия платы Livetronic: Mini (Robolight)

Дата редакции: 25-05-2017

Разработчик: Артур Голубцов
*******************************************************************************/

void setup() {
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
  Serial.begin(115200);
}

void loop() {
  
  Serial.println("D18 D19 D20 D21 D24 D25 A0   A1   A2   A3   A4   A5   A6   A7   A8   ");
  Serial.printf("%3d %3d %3d %3d %3d %3d %4d %4d %4d %4d %4d %4d %4d %4d %4d\n", digitalRead(18), digitalRead(19), digitalRead(20), digitalRead(21), digitalRead(24), digitalRead(25), analogRead(A0), analogRead(A1), analogRead(A2), analogRead(A3), analogRead(A4), analogRead(A5), analogRead(A6), analogRead(A7), analogRead(A8));
  delay(200);

}
