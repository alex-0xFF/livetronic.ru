//#include <SoftwareSerial.h>

int LED1 = 6;
int LED2 = 11;

int DIR1 = 2;  // Цифровые порты, отвечающие за направление движения
int DIR2 = 4;
int EN1 = 3;  // ШИМ порты, отвечающие за скорость вращения мотора
int EN2 = 5;

int M_PWR = 170;

int commandByte;

//SoftwareSerial btSerial(0, 1); // RX, TX

void setup(){                
    pinMode(DIR1, OUTPUT);
    pinMode(DIR2, OUTPUT);
    pinMode(EN1, OUTPUT);
    pinMode(EN2, OUTPUT);        

    for (int i=0; i<4; i++){
      delay(100);
      digitalWrite(LED1, 1);
      digitalWrite(LED2, 1);
      delay(100);
      digitalWrite(LED1, 0);
      digitalWrite(LED2, 0);      
    }
    
    Serial.begin(9600);
}

void loop(){        
    if (Serial.available() > 0) {
      commandByte = Serial.read();
      
      if (commandByte == 0) {                        
          digitalWrite(EN1, 0);
          digitalWrite(EN2, 0);          
      } else {
          switch (commandByte) {        
          case 1:                              
              digitalWrite(DIR1, 1);
              digitalWrite(DIR2, 1);
              digitalWrite(EN1, 1);
              digitalWrite(EN2, 1);
              break;
          case 2:                              
              digitalWrite(DIR1, 0);
              digitalWrite(DIR2, 0);
              digitalWrite(EN1, 1);
              digitalWrite(EN2, 1);
              break;
          case 3:                              
              digitalWrite(DIR1, 1);
              digitalWrite(DIR2, 0);
              analogWrite(EN1, M_PWR);
              analogWrite(EN2, M_PWR);
              break;    
          case 4:                               
              digitalWrite(DIR1, 0);
              digitalWrite(DIR2, 1);
              analogWrite(EN1, M_PWR);
              analogWrite(EN2, M_PWR);
            break;
          }
      }           
    }    
}

