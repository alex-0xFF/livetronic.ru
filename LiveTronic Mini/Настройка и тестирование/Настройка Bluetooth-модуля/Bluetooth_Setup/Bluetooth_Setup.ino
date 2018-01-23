/*******************************************************************************
Программа для проверки работоспособности и настройки BT модуля HC-05

Версия платы Livetronic: Mini (Robolight)

Дата редакции: 22-03-2017

Разработчик: Артур Голубцов
*******************************************************************************/

//int BT_SETUP_PIN = A1;                    // Пин, отвечающий за настройку Bluetooth модуля

void setup() {
  delay(3000);
  pinMode(A0, OUTPUT);                     // Режим работы порта - вывод
  pinMode(A1, OUTPUT);
  //Serial.begin(115200);                   // Подключаемся к основному последовательному порту платы
  //digitalWrite(A0, LOW);
  //digitalWrite(A1, LOW);
  //delay(1000);
  Serial.begin(9600);                       // Подключаемся к последовательному порту на котором расположен HC-05 модуль
  digitalWrite(A0, HIGH);
  digitalWrite(A1, HIGH);
  delay(100);
  Serial.println("AT+NAME=LTLite 000001");    // Здесь можно поменять название модуля
  Serial.println("AT+PSWD=1234");             // Устанавливаем пароль
  Serial.println("AT+BAUD=9600");             // Устанавливаем скорость обмена данными   
  digitalWrite(A0, LOW);
  digitalWrite(A1, LOW);
  
}

void loop () { 
  if (Serial.available()) {
    Serial.write(Serial.read());                              
  }
}
