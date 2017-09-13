/*******************************************************************************
Программа для настройки и автономного движения рук картонного андроида

Версия платы Livetronic: Mega

Дата редакции: 28-03-2017

Разработчик: Артур Голубцов
*******************************************************************************/

#include <Servo.h>

#define MIN 60      // Минимальный угол поворота
#define MAX 120     // Максимальный угол поворота
#define N 12        // Количество серв для проверки

Servo servo[12];    // Массив с объектами сервоприводов
int pos = 0;        // Пересенная для хранения угла поворота

void setup() {
  for (int i = 0; i < N; i++) {         // Подключаем сервоприводы к портам подряд, начиная со второго
    servo[i].attach(i+2);
  }
  pos = 90;
}

void loop() {  
  
  // Выставляем сервоприводы в начальое положение и ждём 3 секунды  
         
  for (int i = 0; i < N; i++)               // Выставляем все сервоприводы в среднее положение (угол 90 градусов)
    servo[i].write(pos);                    // Команда для поворота вала сервопривода в определённый угол (в данном случае - 90 градусов)
  delay(3000);                              // Ждём 3 секунды

  // Уходим в бесконечный цикл и вращаем руками робота до опупения
  
  while(true) {                            
    for(pos = MIN; pos <= MAX; pos += 1)    // Вращаем вал сервопривода от угла MIN до угла MAX с шагом 1 градус
    {
      // Первую половину (на портах от 2 до 2+N/2) сервоприводов вращаем влево
      for (int i = 0; 2*i < N; i++)         // Выставляем все сервоприводы в угол 'pos'  
        servo[i].write(pos); 
      // Вторую половину (на портах от 2+N/2 до 2+N)сервоприводов вращаем вправо
      for (int i = N/2; i < N; i++)         // Выставляем все сервоприводы в угол 'pos'  
        servo[i].write(MAX + MIN - pos);           
      delay(15);                            // Ждём 15мс для того, чтобы сервопривод успел занять требуемое положение
    } 
    
    for(pos = MAX; pos>=MIN; pos-=1)        // Вращаем вал сервопривода от угла MAX до угла MIN с шагом 1 градус
    { 
      // Первую половину (на портах от 2 до 2+N/2) сервоприводов вращаем вправо
      for (int i = 0; 2*i < N; i++)         // Выставляем все сервоприводы в угол 'pos'  
        servo[i].write(pos); 
      // Вторую половину (на портах от 2+N/2 до 2+N)сервоприводов вращаем влево
      for (int i = N/2; i < N; i++)         // Выставляем все сервоприводы в угол 'pos'  
        servo[i].write(MAX + MIN - pos);                
      delay(15);                            // Ждём 15мс для того, чтобы сервопривод успел занять требуемое положение
    } 
  }
}
