#include <EEPROM.h>                                          //библиотека работы с постоянной памятью для записи настроек.
#include <Adafruit_GFX.h>                                    // библиотека работы со шрифтами
#include <Adafruit_PCD8544.h>                                //библиотека работы с жк экраном от nokia3310
#include "DHT.h"                                             //библиотека работы с датчиком DHT11

  int led = 8;                                               //переменная с номером пина реле
  int ldr1 = 0;                                              //аналоговый пин фоторезистора
  int ldr2 = 1;                                              //второй аналоговый пин фоторезистора для исключения ложных срабатываний
  int level1 = 0;                                            //уровень освещенности с первого фоторезистора
  int level2 = 0;                                            //уровень освещенности со второго фоторезистора
  const int buttonPin1 = 9;                                  //цифровой пин первой кнопки "+"
  const int buttonPin2 = 10;                                 //цифровой пин второй кнопки "-"
  int buttonState1 = 0;                                      //изначальное состояние кнопки
  int buttonState2 = 0;                                      //изначальное состояние кнопки

#define DHTPIN 2                                             // номер пина, к которому подсоединен датчик
  DHT dht(DHTPIN, DHT11);                                    //
  // pin 7 - Serial clock out (SCLK)
  // pin 6 - Serial data out (DIN)
  // pin 5 - Data/Command select (D/C)
  // pin 4 - LCD chip select (CS)
  // pin 3 - LCD reset (RST)

  Adafruit_PCD8544 display = Adafruit_PCD8544(7, 6, 5, 4, 3);//
#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2
#define LOGO16_GLCD_HEIGHT 16
#define LOGO16_GLCD_WIDTH  16

  const static unsigned char PROGMEM logo16_glcd_bmp[] =     //массив цветочка (бантики)
  { B00000000, B11000000,
    B00000001, B11000000,
    B00000001, B11000000,
    B00000011, B11100000,
    B11110011, B11100000,
    B11111110, B11111000,
    B01111110, B11111111,
    B00110011, B10011111,
    B00011111, B11111100,
    B00001101, B01110000,
    B00011011, B10100000,
    B00111111, B11100000,
    B00111111, B11110000,
    B01111100, B11110000,
    B01110000, B01110000,
    B00000000, B00110000 };

  int delay_sleep = 1000;                                    //время задержки в системе (обновления экрана и измерений)
  int prom = 14;                                             //длительность периода работы лампы (пока тупо рисуем 14 часов на экране. потом будем измерять)
  int porog = 800;                                           //пороговое значение уровня освещенности, когда включается освещение
                                                             //в будущем читать из EEPROM (чтобы настройки запоминались)
void setup()                                                 //процедура первичной инициализации
{
  Serial.begin(9600);                                        //инициализируем сериал на скорости 9600
  dht.begin();                                               //инициализируем датчик влажности и температуры
  display.begin();                                           //инициализируем экран
  display.setContrast(60);                                   //указываем контраст экрана. 60 вроде как самое то
  display.display();                                         // show splashscreen -- используется несколько раз. кажется для отображения заготовленных изменнений. проверить. если что удалить.
  delay(delay_sleep);                                        // задержка. кажется нужна. связано с ком.выше, чтобы успел отобразиться (?)
  display.clearDisplay();                                    // clears the screen and buffer
  display.drawPixel(10, 10, BLACK);                          //draw a single pixel. было, тупо тупая фишка. можно будет удалить.
  display.display();
  delay(delay_sleep);                                        //
  display.clearDisplay();
  display.setTextSize(1);                                    //выбираем размер шрифта 1 (мелкий, кажется самый)
  display.setTextColor(BLACK);                               //выбираем цвет шрифта (??)

  pinMode(buttonPin1, INPUT);                                // настроить пин первой кнопки на ввод данных 
  pinMode(buttonPin2, INPUT);                                // настроить пин второй кнопки на ввод данных
  pinMode(led, OUTPUT);                                      //указываем, что пин реле - выход. в последующем переименовать это в понятное реле.
    
}

void loop() //процедура loop
{
  buttonState1 = digitalRead(buttonPin1);
  buttonState2 = digitalRead(buttonPin2);
   if (buttonState1 == HIGH) {
    // turn LED on:
    porog = porog + 10;
    Serial.println(" + ");
  } else {
    // turn LED off:
//    digitalWrite(ledPin, LOW);
    Serial.println(" 0");
  }

   if (buttonState2 == HIGH) {
    // turn LED on:
    porog = porog - 10;
    Serial.println(" - ");
  } else {
    // turn LED off:
//    digitalWrite(ledPin, LOW);
    Serial.println(" 0");
  }

  
  level1 = analogRead(ldr1);
  level2 = analogRead(ldr2);
  float h = dht.readHumidity();    //Считываем влажность
  float t = dht.readTemperature();  // Считываем температуру
  if (isnan(h) || isnan(t)) {  // Проверка удачно прошло ли считывание.
  Serial.println(" ERROR t or h ");
  display.print (" ERROR t or h ");
  //return();
}
  display.clearDisplay();
  display.setCursor(0,0);
  display.setTextSize(0);  
  display.print("sens ");  
  display.print(level1);
  display.print("/");
  display.println(level2);
  display.setTextColor(BLACK);
display.setTextSize(2);
display.print(int(h));
display.println("% H");
display.setTextSize(2);
display.print(int(t));
display.println("  C");
display.setTextSize(1);
//display.print("light");
display.print(porog);
  if ((level1 < porog)&(level2 < porog))
    {
    digitalWrite(led, HIGH);
    Serial.println(" ON");
    display.print(" ON (");
    display.print(prom);    
    display.print("h)");  
    }
  else 
    {
    digitalWrite(led, LOW); //иначе выключаем
    Serial.println(" OFF");
    display.print(" OFF");
    }
  display.display();    
  delay(1000);
}

void testdrawbitmap(const uint8_t *bitmap, uint8_t w, uint8_t h) {
  uint8_t icons[NUMFLAKES][3];
  srandom(666);     // whatever seed
 
  // initialize
  for (uint8_t f=0; f< NUMFLAKES; f++) {
    icons[f][XPOS] = random() % display.width();
    icons[f][YPOS] = 0;
    icons[f][DELTAY] = random() % 5 + 1;
    
    Serial.print("x: ");
    Serial.print(icons[f][XPOS], DEC);
    Serial.print(" y: ");
    Serial.print(icons[f][YPOS], DEC);
    Serial.print(" dy: ");
    Serial.println(icons[f][DELTAY], DEC);
  }

  while (1) {
    // draw each icon
    for (uint8_t f=0; f< NUMFLAKES; f++) {
      display.drawBitmap(icons[f][XPOS], icons[f][YPOS], logo16_glcd_bmp, w, h, BLACK);
    }
    display.display();
    delay(200);
    
    // then erase it + move it
    for (uint8_t f=0; f< NUMFLAKES; f++) {
      display.drawBitmap(icons[f][XPOS], icons[f][YPOS],  logo16_glcd_bmp, w, h, WHITE);
      // move it
      icons[f][YPOS] += icons[f][DELTAY];
      // if its gone, reinit
      if (icons[f][YPOS] > display.height()) {
  icons[f][XPOS] = random() % display.width();
  icons[f][YPOS] = 0;
  icons[f][DELTAY] = random() % 5 + 1;
      }
    }
   }
}


void testdrawchar(void) {
  display.setTextSize(1);
  display.setTextColor(BLACK);
  display.setCursor(0,0);

  for (uint8_t i=0; i < 168; i++) {
    if (i == '\n') continue;
    display.write(i);
    //if ((i > 0) && (i % 14 == 0))
      //display.println();
  }    
  display.display();
}

void testdrawcircle(void) {
  for (int16_t i=0; i<display.height(); i+=2) {
    display.drawCircle(display.width()/2, display.height()/2, i, BLACK);
    display.display();
  }
}

void testfillrect(void) {
  uint8_t color = 1;
  for (int16_t i=0; i<display.height()/2; i+=3) {
    // alternate colors
    display.fillRect(i, i, display.width()-i*2, display.height()-i*2, color%2);
    display.display();
    color++;
  }
}

void testdrawtriangle(void) {
  for (int16_t i=0; i<min(display.width(),display.height())/2; i+=5) {
    display.drawTriangle(display.width()/2, display.height()/2-i,
                     display.width()/2-i, display.height()/2+i,
                     display.width()/2+i, display.height()/2+i, BLACK);
    display.display();
  }
}

void testfilltriangle(void) {
  uint8_t color = BLACK;
  for (int16_t i=min(display.width(),display.height())/2; i>0; i-=5) {
    display.fillTriangle(display.width()/2, display.height()/2-i,
                     display.width()/2-i, display.height()/2+i,
                     display.width()/2+i, display.height()/2+i, color);
    if (color == WHITE) color = BLACK;
    else color = WHITE;
    display.display();
  }
}

void testdrawroundrect(void) {
  for (int16_t i=0; i<display.height()/2-2; i+=2) {
    display.drawRoundRect(i, i, display.width()-2*i, display.height()-2*i, display.height()/4, BLACK);
    display.display();
  }
}

void testfillroundrect(void) {
  uint8_t color = BLACK;
  for (int16_t i=0; i<display.height()/2-2; i+=2) {
    display.fillRoundRect(i, i, display.width()-2*i, display.height()-2*i, display.height()/4, color);
    if (color == WHITE) color = BLACK;
    else color = WHITE;
    display.display();
  }
}
   
void testdrawrect(void) {
  for (int16_t i=0; i<display.height()/2; i+=2) {
    display.drawRect(i, i, display.width()-2*i, display.height()-2*i, BLACK);
    display.display();
  }
}

void testdrawline() {  
  for (int16_t i=0; i<display.width(); i+=4) {
    display.drawLine(0, 0, i, display.height()-1, BLACK);
    display.display();
  }
  for (int16_t i=0; i<display.height(); i+=4) {
    display.drawLine(0, 0, display.width()-1, i, BLACK);
    display.display();
  }
  delay(250);
  
  display.clearDisplay();
  for (int16_t i=0; i<display.width(); i+=4) {
    display.drawLine(0, display.height()-1, i, 0, BLACK);
    display.display();
  }
  for (int8_t i=display.height()-1; i>=0; i-=4) {
    display.drawLine(0, display.height()-1, display.width()-1, i, BLACK);
    display.display();
  }
  delay(250);
  
  display.clearDisplay();
  for (int16_t i=display.width()-1; i>=0; i-=4) {
    display.drawLine(display.width()-1, display.height()-1, i, 0, BLACK);
    display.display();
  }
  for (int16_t i=display.height()-1; i>=0; i-=4) {
    display.drawLine(display.width()-1, display.height()-1, 0, i, BLACK);
    display.display();
  }
  delay(250);

  display.clearDisplay();
  for (int16_t i=0; i<display.height(); i+=4) {
    display.drawLine(display.width()-1, 0, 0, i, BLACK);
    display.display();
  }
  for (int16_t i=0; i<display.width(); i+=4) {
    display.drawLine(display.width()-1, 0, i, display.height()-1, BLACK); 
    display.display();
  }
  delay(250);
}
