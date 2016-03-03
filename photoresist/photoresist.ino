#include <EEPROM.h>                                          //библиотека работы с постоянной памятью для записи настроек.
#include <Adafruit_GFX.h>                                    //библиотека работы с примитивами и текстом
#include <Adafruit_PCD8544.h>                                //библиотека работы с жк экраном от nokia3310
#include "DHT.h"                                             //библиотека работы с датчиком DHT11

//  const int relayLight = 8;                                //переменная с номером пина реле
  const byte relayLight = 8;                                 //переменная с номером пина реле
  const byte ldr1 = 0;                                       //аналоговый пин фоторезистора
  const byte ldr2 = 1;                                       //второй аналоговый пин фоторезистора для исключения ложных срабатываний
  int level1 = 0;                                            //уровень освещенности с первого фоторезистора
  int level2 = 0;                                            //уровень освещенности со второго фоторезистора
//  const int buttonPlus = 9;                                  //цифровой пин первой кнопки "+"
//  const int buttonMinus = 10;                                //цифровой пин второй кнопки "-"
//  const int buttonLight = 11;              //цифровой пин кнопки включения освещения экрана
  const byte buttonPlus = 9;                                  //цифровой пин первой кнопки "+"
  const byte buttonMinus = 10;                                //цифровой пин второй кнопки "-"
  const byte buttonLight = 11;              //цифровой пин кнопки включения освещения экрана

  const int delayButtonRequest = 100;      //длительность периодичности опроса кнопок
  //const int delayScreenLight = delayButtonRequest * 10;       //длительность подсветки. на будущее для сохранения настроек в памяти
  //const int delaySensorsWait = delayButtonRequest * 20;       //длительность периодичности опроса датчиков
//  const int screenLightPin = 12;           //цифровой пин подсветки (на выход)
  const byte screenLightPin = 12;           //цифровой пин подсветки (на выход)
  byte buttonPlusState = 0;                                   //состояние кнопки "плюс"
  byte buttonMinusState = 0;                                  //изначальное состояние кнопки
  const int step_level = 1;                                 //переменная шага изменения уровня освещенности
  const int serialSpeed = 9600;                              //скорость подключения по сериалу
  const byte contrast = 60;                                   //указываем контраст экрана. 60 вроде как самое то
  String strStateRelay = "empty str";
  unsigned long currentTime;               //------текущее время
  unsigned long loopTime;                  //------время для обновления

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
//  int porog = 800;                                           //пороговое значение уровня освещенности, когда включается освещение
  uint8_t porog = 80;
                                                             //в будущем читать из EEPROM (чтобы настройки запоминались)
void setup()                                                 //процедура первичной инициализации
{
  porog = EEPROM.read(0);
  currentTime = millis();                     //считываем время, прошедшее с момента запуска программы
  loopTime = currentTime;                     //выравниваем показания
  Serial.begin(serialSpeed);                                 //инициализируем сериал на скорости 9600
  dht.begin();                                               //инициализируем датчик влажности и температуры
  display.begin();                                           //инициализируем экран
  display.setContrast(contrast);                             //указываем контраст экрана. 60 вроде как самое то
  display.display();                                         //show splashscreen -- используется несколько раз. кажется для отображения заготовленных изменнений. проверить. если что удалить.
  delay(delay_sleep);                                        //задержка. кажется нужна. связано с ком.выше, чтобы успел отобразиться (?)
  display.clearDisplay();                                    //clears the screen and buffer
  display.drawPixel(10, 10, BLACK);                          //draw a single pixel. было, тупо тупая фишка. можно будет удалить.
  display.display();
  delay(delay_sleep);                                        //
  display.clearDisplay();
  display.setTextSize(1);                                    //выбираем размер шрифта 1 (мелкий, кажется самый)
  display.setTextColor(BLACK);                               //выбираем цвет шрифта (??)

  pinMode(buttonPlus, INPUT);                                //настроить пин первой кнопки на ввод данных 
  pinMode(buttonMinus, INPUT);                               //настроить пин второй кнопки на ввод данных
  pinMode(buttonLight, INPUT);                 //настроить пин кнопки подсветки на ввод данных
  pinMode(relayLight, OUTPUT);                               //указываем, что пин реле - выход. в последующем переименовать это в понятное реле.
  pinMode(screenLightPin, OUTPUT);             //настроить пин подсветки на вывод, им запускать транзистор подсветки
}

void loop()                                   //процедура бесконечного цикла
{
  float h = 0;                                //объявляем переменную типа float влажности
  float t = 0;                                //объявляем переменную типа float температуры
  int prom = 14;                                             //длительность периода работы лампы (пока тупо рисуем 14 часов на экране. потом будем измерять)
  bool b_updateScreen = false;                //флаг обновления содержимого экрана. если true, то обновить экран
  bool b_updateSensorValue = false;           //флаг обновления значений сенсоров влажности и температуры, если true, то запросить новые значения
  bool b_updateButtons = false;               //флаг обновления значений кнопок
  int i_prom = 0;                             //счетчик повторов для опроса датчиков

  currentTime = millis();                               // считываем заново время, прошедшее с момента запуска программы
  if(currentTime >= (loopTime + delayButtonRequest))
  {   // сравниваем текущий таймер с переменной loopTime + 0,1 секунда для задержки в 0,1 секунду
    i_prom++;
    if(i_prom = 10)
    {                                    //будет выполняться в 20 раза реже delayScreenLight (2 секунды) чтобы датчики успевали отработать
      b_updateScreen = true;                            //ставим флаг обновить экран раз в секунду
      b_updateButtons = true;                           //ставим флаг "время обновить значение кнопок"
    }
    if(i_prom = 20)
    {                                    //будет выполняться в 20 раза реже delayScreenLight (2 секунды) чтобы датчики успевали отработать
      b_updateSensorValue = true;                       //ставим флаг обновить значения сенсоров в true
      i_prom = 0;                                       //зануляем счетчик для следующего цикла
    }
    loopTime = currentTime;                             //в loopTime записываем новое текущее значение
  }
  
// определяем нажатие кнопок и изменение порогового уровня освещенности
// переопределить поведение. кажется все окей.
  if(b_updateButtons)
  {                      //время выяснить значение кнопок
    buttonPlusState = digitalRead(buttonPlus);                //считать в переменную состояние кнопки1 на увеличение порога (есть проблема, что надо держать долго кнопку)
    buttonMinusState = digitalRead(buttonMinus);              //считать в переменную состояние кнопки2 на уменьшение порога (есть проблема, что надо держать долго кнопку)
    if (buttonPlusState == HIGH) 
    {                            //если нажата кнопка1 ("+"), то увеличить
      porog = porog + step_level;                              //пороговое значение на step_level
      EEPROM.write(0, porog);
      Serial.println(" + ");                                   //отладочная
    } 
    else 
    {
      Serial.println(" 0");                                    //конструкцию с двумя if сократить!
      if (buttonMinusState == HIGH) 
      {                          //если нажата кнопка2 ("-"), то уменьшить
        porog = porog - step_level;                            //пороговое значение на step_level
        EEPROM.write(0, porog);
        Serial.println(" - ");                                 //отладочная
      }
    }
    b_updateButtons = false;                                   //сбрасываем флаг для следующего цикла
  }

  if(b_updateSensorValue)
  {                         //время обновить значения сенсоров
    level1 = int(analogRead(ldr1)/10);                                 // выясняем уровень освещенности на фоторезисторе1  
    level2 = int(analogRead(ldr2)/10);                                 // выясняем уровень освещенности на фоторезисторе2
    h = dht.readHumidity();                                    //Считываем влажность с датчика DHT11
    t = dht.readTemperature();                                 //Считываем температуру с датчика DHT11
    if (isnan(h) || isnan(t)) 
    {                                // Проверка удачно прошло ли считывание.
      Serial.println(" ERROR t or h ");                        // в сериал
      display.print (" ERROR t or h ");                        // на дисплей
    }
    b_updateSensorValue = false;                        //сбрасываем флаг обновления данных с сенсоров
  }

  if(b_updateScreen)
  {                             //время обновить экран
    display.clearDisplay();
    display.setCursor(0,0);                                    //ставим курсор на начальную позицию
    display.setTextSize(0);                                    //задаем размер шрифта 0 (кажется не работает)
    display.print("sens ");                                    //форматируем вывод. выводим текст на экран
    display.print(level1);
    display.print("/");
    display.println(level2);
    display.setTextColor(BLACK);
    display.setTextSize(2);                                   //выводим большими буквами
    display.print(int(h));
    display.println("% H");
    display.setTextSize(2);
    display.print(int(t));
    display.print(char(0));        //печатаем знак градуса (проверить!)
    display.println("  C");
    display.setTextSize(1);
    display.print(porog);
    display.print(strStateRelay);    // добавить печать строки со значением статуса реле
    display.display();    
//    delay(delay_sleep);
    b_updateScreen = false;          //сбрасываем флаг обновления дисплея
  }
  
  if ((level1 < porog)&(level2 < porog))
  {                   //сама логика включения реле. если оба датчика показывают уровень освещенности ниже порогового
    digitalWrite(relayLight, HIGH);
// добавить строку и в нее записывать статус реле. а строку в дисплей печатать во время обновления    
    strStateRelay = String(" ON (" + String(prom) + "h)");
//    display.print(" ON (");
//    display.print(prom);    
//    display.print("h)");  
  }
  else 
  {
    digitalWrite(relayLight, LOW);                          //иначе выключаем реле освещения
    strStateRelay = String(" OFF");
//    display.print(" OFF");
  }
}

void testdrawbitmap(const uint8_t *bitmap, uint8_t w, uint8_t h) 
{    //в будущем можно сделать экранную заставку, если температура не трогалась более 5 минут
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
