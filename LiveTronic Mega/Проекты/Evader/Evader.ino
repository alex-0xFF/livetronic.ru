#include <SharpIR.h>

// Номера цифровых портов включения/выключения драйверов двигателей
const int M1_ENABLE_PIN = 42;
const int M2_ENABLE_PIN = 48;

// Номера цифровых портов, отвечающих за направление движения двигателей
const int M1_DIR_PIN = 43;
const int M2_DIR_PIN = 47;

// Номера аналоговых портов, отвечающих за скорость вращения двигателей
const int M1_SPEED_PIN = 9;
const int M2_SPEED_PIN = 10;

// Расстояние от припятствия, на котором робот будет останавливаться
const int STOP_DISTANCE = 15;

// Создание объекта ИК-сенсора GP2Y0A21YK0F подключенныого к порту A0
SharpIR sensor(GP2Y0A21YK0F, A0);

void setup()
{
  // Установка портов в режим вывода
  pinMode(M1_ENABLE_PIN, OUTPUT);
  pinMode(M2_ENABLE_PIN, OUTPUT);
  
  pinMode (M1_DIR_PIN, OUTPUT);
  pinMode (M2_DIR_PIN, OUTPUT);
  
  pinMode (M1_SPEED_PIN, OUTPUT);
  pinMode (M2_SPEED_PIN, OUTPUT);  

  // Включение драйверов двигателя
  digitalWrite(M1_ENABLE_PIN, HIGH);
  digitalWrite(M2_ENABLE_PIN, HIGH);

  // Установка направления движения
  digitalWrite(M1_DIR_PIN, HIGH);
  digitalWrite(M2_DIR_PIN, HIGH);

  // Начало движения
  analogWrite(M1_SPEED_PIN, 50);
  analogWrite(M2_SPEED_PIN, 50);
  
  // DEBUG
  Serial.begin(9600);
}

void loop()
{    
  // Получение расстояния с датчика 
  int distance = sensor.getDistance();
  
  // DEBUG
  Serial.println(distance); // Print the value to the serial monitor

  // Если расстояние меньше заданного, то...
  if (distance < STOP_DISTANCE)
  {        
    // Прекращение движения
    analogWrite(M1_SPEED_PIN, 0);
    analogWrite(M2_SPEED_PIN, 0);
    delay(1000); // Задержка 1с        
    
    // Переключение направления вращения одного из колёс — робот будет вращаться на месте
    digitalWrite(M2_DIR_PIN, LOW);
    // Начало вращения
    analogWrite(M1_SPEED_PIN, 50);
    analogWrite(M2_SPEED_PIN, 50);
    delay(850); // Робот будет вращаться минимум 850мс
    // Вращение колёс в разные стороны по скоростью 50 в течении 850 миллисекнуд это поворот приблизительно на 90°
    
    // Вращение робота происходит до тех пор, пока перед ним не пропадёт препятствие
    while(sensor.getDistance() < STOP_DISTANCE)
    {}
    
    // Прекращение вращения
    analogWrite(M1_SPEED_PIN, 0);
    analogWrite(M2_SPEED_PIN, 0);    
    delay(500);

    // Переключение направления вращения мотора в исходное состояние
    digitalWrite(M2_DIR_PIN, HIGH);
    // Начало движения
    analogWrite(M1_SPEED_PIN, 50);
    analogWrite(M2_SPEED_PIN, 50);    
  }  
}
