#include <SharpIR.h>

// Цифровые порты, отвечающие за направление движения
int M1_DIR_PIN = 2;
int M2_DIR_PIN = 4;

// ШИМ порты, отвечающие за скорость вращения мотора
int M1_SPEED_PIN = 3; //EN1
int M2_SPEED_PIN = 5; //EN2

// Расстояние от припятствия, на котором робот будет останавливаться
const int STOP_DISTANCE = 15;

const int SPEED = 50;

// Создание объекта ИК-сенсора GP2Y0A21YK0F подключенныого к порту A0
SharpIR sensor(GP2Y0A21YK0F, A2);

void setup()
{
  // Установка портов в режим вывода
  pinMode (M1_DIR_PIN, OUTPUT);
  pinMode (M2_DIR_PIN, OUTPUT);
  
  pinMode (M1_SPEED_PIN, OUTPUT);
  pinMode (M2_SPEED_PIN, OUTPUT);  

  // Установка направления движения
  digitalWrite(M1_DIR_PIN, HIGH);
  digitalWrite(M2_DIR_PIN, HIGH);

  // Начало движения
  analogWrite(M1_SPEED_PIN, SPEED);
  analogWrite(M2_SPEED_PIN, SPEED);
  
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
    analogWrite(M1_SPEED_PIN, SPEED);
    analogWrite(M2_SPEED_PIN, SPEED);
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
    analogWrite(M1_SPEED_PIN, SPEED);
    analogWrite(M2_SPEED_PIN, SPEED);    
  }  
}
