/*
Плата Arduino Mega на чипе ATMega 2560.

Шаблон для файла aplha.csv
1;100;160;14;-21
2;105;170;15;-22
-\\-\\-
25;120;200;18;-25
  !!!Строк должно быть строго 25, каждая строка заканчиваться без символов.
*/

//=============
//Библиотки
//=============
#include "EEPROM.h"
#include "SPI.h" //Библиотека SPI
#include "SD.h" //Библиотека SD
#include "GyverTM1637.h"   // Библиотека индикатора
#include "StringSplitter.h" //Библиотека среза строк

//=============
//Константы
//=============
#define BTN A0   // Пин включения флага
#define BTN2 A15   // Пин блока резисторов(ключей) 5v - GDN - A15
#define chipSelect 8   // Пин CS флэшки
#define CLK1 23 // Пин CLK
#define DIO1 22 // Пин DIO
#define CLK2 25 // Пин CLK2
#define DIO2 24 // Пин DIO2
#define CLK3 27 // Пин CLK3
#define DIO3 26 // Пин DIO3
#define CLK4 29 // Пин CLK4
#define DIO4 28 // Пин DIO4
#define CLK5 31 // Пин CLK5
#define DIO5 30 // Пин DIO5
#define light 5 // Яркость дисплеев
Sd2Card card;
SdVolume volume;
SdFile root;
const byte ERR1_1[4]= {_E, _r, _empty, _1} ; 
const byte ERR2[4]= {_E, _r, _empty, _2} ;
const byte ERR3_1[4]= {_S, _d, _E, _r} ;
const byte ERR3_2[4]= {_F, _i, _L, _E} ; 
const byte ERR3_3[4]= {_N, _U, _L, _L} ; 
GyverTM1637 disp1(CLK1, DIO1); GyverTM1637 disp2(CLK2, DIO2); GyverTM1637 disp3(CLK3, DIO3);GyverTM1637 disp4(CLK4, DIO4); GyverTM1637 disp5(CLK5, DIO5);

//=============
//Переменные
//=============
String (string);
String Data[25];
int lines[25][5];
String(buf);
boolean btnState, btnFlag; // Состояние флажкасреднего
int bb;  //Отдадка
int a2d_data = 0;

//=============
//Установки
//=============
void setup() {
  Serial.begin(9600);//Запуск последовательного порта
  pinMode(BTN, INPUT_PULLUP); //Значение на пин ключа 1 или 0
  pinMode(BTN2, INPUT); //Значение на чтение числа ключа
  pinMode(8, OUTPUT); 
  digitalWrite(8, HIGH);  //Инициализация пина флэшки
 disp1.clear(); disp2.clear(); disp3.clear(); disp4.clear(); disp5.clear(); // Очистка индикатора
 disp1.brightness(light); disp2.brightness(light); disp3.brightness(light); disp4.brightness(light); disp5.brightness(light); // Яркость, 0 - 7 (минимум - максимум)
  if (!SD.begin(chipSelect)) {
    disp2.displayByte(_S, _D,  _E, _r);
    disp4.displayByte(_N, _O,  _empty, _empty);
    Serial.println(F("Карты нет или не верный формат."));
    while (1);}
  Serial.println(F("Карта активна."));
  SDcard();
  ArrayData();
  ArrayString();
  }

//=============
//Основной цикл
//=============
void loop() {
  btnState = !digitalRead(BTN);  // Читаем состояние ключа, 1=зажат, 0=нет.
  if (btnState && !btnFlag) {    // Если нажат и был отпущен (btnFlag 0)
    btnFlag = true;              // Запомнили что нажата
    Serial.println(F("Позиция "));
    a2d_data = analogRead(BTN2);
    byte R2= map(a2d_data, 5, 125, 1, 25);
    bb = Resist2(R2);
    Serial.print(F("Значение с блока резисторов(для настройки Resist2): "));
    Serial.println(R2);
    Serial.print(F("Номер секции блока: "));
    Serial.println(bb);
    Indicator(bb);
    Serial.println( );}
  else if (!btnState && btnFlag) {    // Если отпущен и была нажат (btnFlag 1)
    btnFlag = false;             // Запомнили что отпущена
    Serial.println(F("Отпустили "));
    disp1.displayInt(0000);
    Serial.println( );}
    }

//=============
//Поиск позиции энкодера
//=============
  byte Resist2(byte R2) {
  if (R2 >= 8 && R2 <= 13) R2 = 1;
  else if (R2 >= 16 && R2 <= 20) R2 = 2;
  else if (R2 >= 33 && R2 <= 38) R2 = 3;
  else if (R2 >= 40 && R2 <= 44) R2 = 4;
  else if (R2 >= 47 && R2 <= 51) R2 = 5;
  else if (R2 >= 54 && R2 <= 60) R2 = 6;
  else if (R2 >= 61 && R2 <= 70) R2 = 7;
  else if (R2 >= 71 && R2 <= 80) R2 = 8; 
  else if (R2 >= 81 && R2 <= 92) R2 = 9;
  else if (R2 >= 93 && R2 <= 106) R2 = 10;
  else if (R2 >= 107 && R2 <= 111) R2 = 11;
  else if (R2 >= 112 && R2 <= 118) R2 = 12;
  else if (R2 >= 119 && R2 <= 125) R2 = 13;
  else if (R2 >= 126 && R2 <= 130) R2 = 14;
  else if (R2 >= 131 && R2 <= 134) R2 = 15;
  else if (R2 >= 135 && R2 <= 139) R2 = 16;
  else if (R2 >= 140 && R2 <= 143) R2 = 17;
  else if (R2 >= 144 && R2 <= 147) R2 = 18;
  else if (R2 >= 148 && R2 <= 150) R2 = 19;
  else if (R2 >= 152 && R2 <= 155) R2 = 20;
  else if (R2 >= 156 && R2 <= 158) R2 = 21;
  else if (R2 >= 159 && R2 <= 159) R2 = 22;
  else if (R2 >= 160 && R2 <= 163) R2 = 23; 
  else if (R2 >= 164 && R2 <= 166) R2 = 24;
  else if (R2 >= 167 && R2 <= 170) R2 = 25;
  else if (R2 >= 171) R2 = 254; 
  return R2; 
  }

//=============
//Работа с индикаторами
//=============
void Indicator(int Ind) {
    switch (Ind) {
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
    case 10:
    case 11:
    case 12:
    case 13:
    case 14:
    case 15:
    case 16:
    case 17:
    case 18:
    case 19:
    case 20:
    case 21:
    case 22:
    case 23:
    case 24:
    case 25:
      disps(Ind);
      break;
    case 254:
      dispsErr1();
      break;
    default:
      dispsErr2();
    break;}
    }

void disps(int BB){
  if (BB != false){
      disp1.displayInt(lines[BB-1][0]);disp2.displayInt(lines[BB-1][1]);disp3.displayInt(lines[BB-1][2]);disp4.displayInt(lines[BB-1][3]);disp5.displayInt(lines[BB-1][4]);
      Serial.println(F("Функция блока дисплеев - РАБОТАЕТ"));}
  else Serial.println(F("Функция блока дисплеев - НЕРАБОТАЕТ"));
  }
  
void dispsErr1(){
      disp1.displayByte(_empty, _0, _0, _empty);disp2.clear();disp3.displayByte(ERR1_1);disp4.clear();disp5.clear();
      Serial.println(F("Проверь цепь"));
      }
      
void dispsErr2(){
      disp1.displayByte(_empty, _0, _0, _empty);disp2.clear();disp3.displayByte(ERR2);disp4.clear();disp5.clear();
      Serial.println(F("Проверь блок контактов. Подстрой резисторы."));
      }

//=============
//Работа с флэшкой
//=============
int SDcard() {
  File logFile = SD.open("alpha.csv", FILE_READ);
 if (logFile) {
    while (logFile.available()) {
      string+=(char)logFile.read();}
    logFile.close();
  }
  else {
        disp1.displayByte(ERR3_1); disp2.displayByte(ERR3_2);disp3.displayByte(ERR3_3); 
    Serial.println(F("Файла нет."));}
    }

//=============
//Разбор csv файла
//=============
//Перебор файла
int ArrayData() {
   StringSplitter *splitter = new StringSplitter(string, '\n', 27);  // Срез строк по делителю (строка, делитель, ограничение/кол-во)
   int itemCount = splitter->getItemCount();
  for(int i = 0; i < itemCount; i++){
    String item = splitter->getItemAtIndex(i);
        Data[i] = String(item)+'\0';}
        }
//Перебор строки
int ArrayString() {
  int j = 0;
while(j<25){
  int i = 0;
  int u = 0;
 while (i<(Data[j].length())){
  char m = Data[j].charAt(i);
   //Serial.print("M_");Serial.println(m);
  if (m == ';' || m == '\0'){ 
    lines[j][u]=buf.toInt();
    buf="";
    u++;}
  else {
  buf +=m;} 
  i++;}
  j++;}
  }
