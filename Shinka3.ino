
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>
#include <Rtc_Pcf8563.h>
#include <LiquidCrystal.h>


#define NUM_KEYS 16
Adafruit_BMP280 bme; // I2C
Rtc_Pcf8563 rtc;
LiquidCrystal lcd(4 , 5 , 6 , 7 , 8 , 9);


int  adc_key_val[NUM_KEYS] = {50, 130, 200, 300, 350, 390, 430, 475, 510, 535, 560, 585, 610, 628, 640, 700};
int s1 = A2;   // Выбираем пин для подключения датчика освещенности "Фоторезистора"
int s2;  // Переменная, которая будет хранить информацию, получаемую с датчика
int sensorPin = 2;          // Set up a PIR sensor pin

int val = LOW;      //  pir input value
int in1 = 10;              // Указываем, что вывод реле In1, подключен к реле цифровому выводу 5
int in2 = 11;              // Указываем, что вывод реле In2, подключен к реле циф
int p; //в мм рт ст
byte count =0;
byte countm = 0;
long int ms =0;
int save1 = 0;
int save2 = 0;
int save3 = 0;

int t; // Переменная в которую записываются текущий показатель часов

unsigned long previousmillis = 0;
float duration = 0;

const int numReadings = 10;
int readings[numReadings];      // данные, считанные с входного аналогового контакта
int index = 0;                  // индекс для значения, которое считывается в данный момент
int total = 0;                  // суммарное значение
int average = 0;                // среднее значение



void setup()
{
  Serial.begin(9600);
  bme.begin();
  lcd.begin(16, 2);
  pinMode(12, OUTPUT);
  pinMode(sensorPin, INPUT);
  pinMode(in1, OUTPUT);    // Установим вывод 5 как выход
  pinMode(in2, OUTPUT);    // Установим вывод 6 как выход




  //clear out all the registers
  //rtc.initClock();
  //set a time to start with.
  //day, weekday, month, century, year
  //rtc.setDate(5, 6, 12, 0, 21);
  //hr, min, sec
  //rtc.setTime(7, 0, 40);
}

void loop()
{
  t = rtc.getHour();
  float p = bme.readPressure();
  p = p * 0.00750062;
  val = digitalRead(sensorPin);  // Считываем информацию с датчика pir read input value
  s2 = analogRead(s1);// Присваиваем значение переменой s2


  total = total - readings[index]; // берем последнее значение...
  readings[index] = s2; // ...которое было считано от сенсора:
  total = total + readings[index];  // добавляем его к общей сумме:
  index = index + 1;      // продвигаемся к следующему значению в массиве:

  if (index >= numReadings)  // если мы в конце массива...
    index = 0;          // ...возвращаемся к началу:

  average = total / numReadings; // вычисляем среднее значение:
  Serial.println(average);  // выводим его на компьютер цифрами в кодировке ASCII
  delay(1);        // делаем задержку между считываниями – для стабильности программы


  if (average > 400 && t > 6 || t < 2 ) { //Устанавливаем пороговое значение, при котором лампочка будет включаться

    digitalWrite(in2, LOW);
  
    
  }
  else
    digitalWrite(in2, HIGH); // Если показания датчика больше порогового значения, то лампочка не включится или отключится

  

  if (val == HIGH && s2 > 400) {            // check if the input is HIGH
    digitalWrite(in1, LOW);  // turn Relay ON
    Serial.println("Motion detected!");
    lcd.setCursor(8, 0);
    lcd.print("M");
  }

  else {
    digitalWrite(in1, HIGH); // turn Relay OFF
    Serial.println("Motion ended!");
    lcd.setCursor(8, 0);
    lcd.print("-");


  }



  delay (1000);




    if (count == 59) {
      countm++;
      count = 0;
    }
    if ((millis() - ms) > 1000) {
      count++;
      ms = millis();
    }
if (countm == 99) 
{countm = 0;
}


  Serial.println(s2); // Выводим информацию на "Монитор порта" с датчика освещенности
  Serial.print(F("Temperature = "));
  Serial.print(bme.readTemperature());
  Serial.println(" *C");
  Serial.print(F("Pressure = "));
  Serial.print(bme.readPressure());
  Serial.println(" Pa");
  Serial.print(F("Approx altitude = "));
  Serial.print(bme.readAltitude(1013.25)); // this should be adjusted to your local forcase
  Serial.println(" m");
  Serial.println();


  lcd.setCursor(0, 0);
  lcd.print(rtc.getHour());
  lcd.print(":");
  lcd.print(rtc.getMinute());
  lcd.setCursor(6, 0);
  lcd.print(rtc.getDay());
  lcd.print("/");
  lcd.print(rtc.getMonth());
  lcd.print(" ");
  lcd.print(bme.readTemperature()-5, 1);
  lcd.setCursor(11, 1);
  lcd.print(average);
  lcd.setCursor(0, 1);
  
     lcd.print(countm);
     lcd.print(":");
    lcd.print(count);
   lcd.setCursor(7, 1);
    lcd.print(save1);


  










  int key = get_key(A0);
  if (key) Serial.println(key, DEC);
  Serial.println(key);

  if (key == 4) {
    count = 0;
    countm = 0;
    lcd.clear();
     lcd.print(" RESETING TIMER");
     
      
tone(12, 392, 350);
delay(350);
tone(12, 392, 350);
delay(350);
tone(12, 392, 350);
delay(350);
tone(12, 311, 250);
delay(250);
tone(12, 466, 100);
delay(100);
tone(12, 392, 350);
delay(350);
tone(12, 311, 250);
delay(250);
tone(12, 466, 100);
delay(100);
tone(12, 392, 700);
delay(700);

tone(12, 587, 350);
delay(350);
tone(12, 587, 350);
delay(350);
tone(12, 587, 350);
delay(350);
tone(12, 622, 250);
delay(250);
tone(12, 466, 100);
delay(100);
tone(12, 369, 350);
delay(350);
tone(12, 311, 250);
delay(250);
tone(12, 466, 100);
delay(100);
tone(12, 392, 700);
delay(700);

tone(12, 784, 350);
delay(350);
tone(12, 392, 250);
delay(250);
tone(12, 392, 100);
delay(100);
tone(12, 784, 350);
delay(350);
tone(12, 739, 250);
delay(250);
tone(12, 698, 100);
delay(100);
tone(12, 659, 100);
delay(100);
tone(12, 622, 100);
delay(100);
tone(12, 659, 450);
delay(450);

tone(12, 415, 150);
delay(150);
tone(12, 554, 350);
delay(350);
tone(12, 523, 250);
delay(250);
tone(12, 493, 100);
delay(100);
tone(12, 466, 100);
delay(100);
tone(12, 440, 100);
delay(100);
tone(12, 466, 450);
delay(450);

tone(12, 311, 150);
delay(150);
tone(12, 369, 350);
delay(350);
tone(12, 311, 250);
delay(250);
tone(12, 466, 100);
delay(100);
tone(12, 392, 750);
delay(750);
lcd.clear();
    
       }
       
        if (key == 1) {
    save1 =count + countm*60;
     lcd.clear();
  lcd.print(save1);
  lcd.print(" SAVING 1");
  delay(2000);
 lcd.clear(); 
       }
       
       
        if (key == 2) {
    save2 =count + countm*60;
     lcd.clear();
  lcd.print(save2);
  lcd.print(" SAVING 2");
  delay(2000); 
  lcd.clear();
       }
       
        if (key == 3) {
    save3 =count + countm*60;
     lcd.clear();
  lcd.print(save3);
    lcd.print(" SAVING 3");
  delay(2000); 
  lcd.clear();
       }
       
        
       
        if (key == 5) {
   
          lcd.clear();
          lcd.setCursor(7, 1);
  lcd.print(save1);
   
  delay(2000); 

       }
       
        if (key == 6) {
   
          lcd.clear();
          lcd.setCursor(7, 1);
  lcd.print(save2);
  
   
  delay(2000); 
  
       }
       
        if (key == 7) {
   
          lcd.clear();
          lcd.setCursor(7, 1);
  lcd.print(save3);
   
     delay(2000); 
     
       }
       
       
     if (key == 8) {  
tone(12, 392, 350);
delay(350);
tone(12, 392, 350);
delay(350);
tone(12, 392, 350);
delay(350);
tone(12, 311, 250);
delay(250);
tone(12, 466, 100);
delay(100);
tone(12, 392, 350);
delay(350);
tone(12, 311, 250);
delay(250);
tone(12, 466, 100);
delay(100);
tone(12, 392, 700);
delay(700);

tone(12, 587, 350);
delay(350);
tone(12, 587, 350);
delay(350);
tone(12, 587, 350);
delay(350);
tone(12, 622, 250);
delay(250);
tone(12, 466, 100);
delay(100);
tone(12, 369, 350);
delay(350);
tone(12, 311, 250);
delay(250);
tone(12, 466, 100);
delay(100);
tone(12, 392, 700);
delay(700);

tone(12, 784, 350);
delay(350);
tone(12, 392, 250);
delay(250);
tone(12, 392, 100);
delay(100);
tone(12, 784, 350);
delay(350);
tone(12, 739, 250);
delay(250);
tone(12, 698, 100);
delay(100);
tone(12, 659, 100);
delay(100);
tone(12, 622, 100);
delay(100);
tone(12, 659, 450);
delay(450);

tone(12, 415, 150);
delay(150);
tone(12, 554, 350);
delay(350);
tone(12, 523, 250);
delay(250);
tone(12, 493, 100);
delay(100);
tone(12, 466, 100);
delay(100);
tone(12, 440, 100);
delay(100);
tone(12, 466, 450);
delay(450);

tone(12, 311, 150);
delay(150);
tone(12, 369, 350);
delay(350);
tone(12, 311, 250);
delay(250);
tone(12, 466, 100);
delay(100);
tone(12, 392, 750);
delay(750);
       
     }
       
        if (key == 16) {
          lcd.clear();
          lcd.print(" TEBE PIZDEC");
    
  digitalWrite(in1, HIGH);
   digitalWrite(in2, HIGH);
  delay(18000); 
  lcd.clear();
       }
       
       
       
        if (key == 13) { 
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(bme.readPressure());
  lcd.print(" V OCHKO SEBE");
  lcd.setCursor(0, 1);
  lcd.print("   PONAZHIMAY ;)");
   delay(5000); 
   lcd.clear();
        }   
       
       
  delay(500);

}



int get_key(int key_pin)
{
  int input = analogRead(key_pin); //Serial.println(input, DEC);
  for (int k = 0; k < NUM_KEYS; k++) if (input < adc_key_val[k])  return k + 1;
  return 0;
}





