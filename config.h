#ifndef CONFIG_H
#define CONFIG_H
/*******************************
 * Название и версия проекта
 *******************************/
const String nName = "QMClimat";
const String nVersion = "_v3.1.2";

/*******************************
 * Опции использования сенсоров и индикаторов
 *******************************/
#define USE_IND true        //true - использовать Стрелочные индикаторы. false - Без cтрелочных индикаторов.

//Калибровка датчиков серии MQ
#define USE_MQ_CAlibration false        //true - Выполнять калибровку на чистом воздухе. false - Без калибровки.
/*******************************
 * Подключение необходимых библиотек
 *******************************/
#include <WiFi.h>
#include <AsyncTCP.h>
#include <HTTPClient.h>
#include <Arduino.h>
#include <ESPAsyncWebServer.h>
#include <LittleFS.h>
#include <ArduinoJson.h>
#include <Update.h> // Для OTA обновлений

/*******************************
 * Настройки WiFi-сервера
 *******************************/
AsyncWebServer server(80);
AsyncWebServerRequest* pendingRequest = nullptr;

// Переменные для хранения данных WiFi
String ssid;
String pass;

/*******************************
 * Настройка светодиодов (Neopixel)
 *******************************/
#include <Adafruit_NeoPixel.h>
#define NUM_LEDS 4
#define LED_PIN 27
Adafruit_NeoPixel pixels(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

/*******************************
 * BME280 - датчик температуры и влажности (опционально)
 *******************************/
  #include <Wire.h>
  #include <Adafruit_BME280.h>
  Adafruit_BME280 bme;  // BME280 connect I2C
  #define SEALEVELPRESSURE_HPA (1013.25)//Pa
 //unsigned long bmedelayTime;
  String temp;
  String hum;
  String pres;
  String alt;

  unsigned long ReedTimeBME = 0;


/*******************************
 * SGP30 - датчик VOC и eCO2 (опционально)
 *******************************/
//  #include <Wire.h>
  #include <Adafruit_SGP30.h>
  Adafruit_SGP30 sgp;
  unsigned long ReedTimeTVOC = 0;

  int counter = 0;

  // Формула компенсации датчика TVOC от температуры и влажности
  uint32_t getAbsoluteHumidity(float tempSGP, float humSGP) {
  // approximation formula from Sensirion SGP30 Driver Integration chapter 3.15
  const float absoluteHumidity = 216.7f * ((humSGP / 100.0f) * 6.112f * exp((17.62f * tempSGP) / (243.12f + tempSGP)) / (273.15f + tempSGP));  // [g/m^3]
  const uint32_t absoluteHumidityScaled = static_cast<uint32_t>(1000.0f * absoluteHumidity);                                                   // [mg/m^3]
  return absoluteHumidityScaled;
  }
  String eco2;
  String tvoc;
  String h2;
  String eth;
  int Absolutehumidity;


/*******************************
 * PMS5003 - датчик PM (опционально)
 *******************************/
  #include <PMS.h>
  //    #include <WebSerial.h>

  String val1;              //define PM1.0 value of the air detector module
  String val2;              //define PM2.5 value of the air detector module
  String val3;              //define PM10 value of the air detector module
  #define DEBUG_OUT Serial  // GPIO1, GPIO3 (TX/RX pin on ESP-12E Development Board)

  //PMS_READ_INTERVAL (4:30 min) and PMS_READ_DELAY (30 sec) CAN'T BE EQUAL! Values are also used to detect sensor state.
  static const uint32_t PMS_READ_INTERVAL = 270000;
  static const uint32_t PMS_READ_DELAY = 30000;
  uint32_t timerInterval = PMS_READ_DELAY;  // Default sensor state.

  PMS pms(Serial);
  PMS::DATA data;


/*******************************
 * MHZ-19B - датчик CO2 (опционально)
 *******************************/
  #include <MHZ.h>
    #define CO2_IN 15
    #define CO2power 13 
  MHZ co2(CO2_IN, MHZ19B);
  String Co2;

const uint32_t cycleInterval = 180000;   // время между включениями датчикв
//const uint8_t numReadings = 10;         // количество замеров
const uint32_t warmUpTime = 60000;      // время прогрева
const uint32_t readDelay = 1000;        // время между замерами

unsigned long lastCycleTime = 0;


/*******************************
 * MQ135 - датчик газа (опционально)
 *******************************/
#include <MQUnifiedsensor.h>
unsigned long ReedTimeGas = 0;

#define         Board                   ("ESP-32") // Wemos ESP-32 or other board, whatever have ESP32 core.
#define         Pin                     (35)  //IO25 for your ESP32 WeMos Board, pinout here: https://i.pinimg.com/originals/66/9a/61/669a618d9435c702f4b67e12c40a11b8.jpg
#define         Type                    ("MQ-135") //MQ3 or other MQ Sensor, if change this verify your a and b values.
#define         Voltage_Resolution      (3.3) // 3V3 <- IMPORTANT. Source: https://randomnerdtutorials.com/esp32-adc-analog-read-arduino-ide/
#define         ADC_Bit_Resolution      (12) // ESP-32 bit resolution. Source: https://randomnerdtutorials.com/esp32-adc-analog-read-arduino-ide/
#define         RatioMQ135CleanAir      (3.6)//RS / R0 = 3.6 ppm  
#define         RL_MQ135                (1.0) //Наминал нагрузочного (RL) резистора на плате модуля, рекомендвется 20кОм(10-47кОм)
#define         R0_MQ135                (29.66)  //Установить значение R0 и раскоментировать строку после выполнения колибровки на чистом воздухе

MQUnifiedsensor MQ135(Board, Voltage_Resolution, ADC_Bit_Resolution, Pin, Type);

  String Co;
  String CO2;


/*******************************
 * MQ-7 - датчик CO
 *******************************/
//  #include <MQUnifiedsensor.h>
unsigned long ReedTimeSmoke = 0;

#define         Board                   ("ESP-32") // Wemos ESP-32 or other board, whatever have ESP32 core.
#define         CO_IN                   (35)  //IO25 for your ESP32 WeMos Board, pinout here: https://i.pinimg.com/originals/66/9a/61/669a618d9435c702f4b67e12c40a11b8.jpg
#define         Type                    ("MQ-7") //MQ7 or other MQ Sensor, if change this verify your a and b values.
#define         Voltage_Resolution      (3.3) // 3V3 <- IMPORTANT.
#define         ADC_Bit_Resolution      (12) // ESP-32 bit resolution.
#define         RatioMQ7CleanAir        (27.5) //RS / R0 = 27.5 ppm  
#define         RL_MQ7                  (2.0) //Наминал нагрузочного резистора (RL) на плате модуля, рекомендвется 4.7кОм
#define         R0_MQ7                  (0.43)  //Установить полученнон значение R0 и раскоментировать строку после выполнения колибровки на чистом воздухе

MQUnifiedsensor MQ7(Board, Voltage_Resolution, ADC_Bit_Resolution, CO_IN, Type);

unsigned long oldTime = 0;

String CO;

/*******************************
 * Погода (OpenWeatherMap)
 *******************************/
volatile bool needFetchWeather = false;
const String Wserver  = "api.openweathermap.org";
String Location = "Krasnoyarsk";
String API_Key = "a0e2db5f9e68ec8ef5310cf8e2f4c884";

struct WeatherData {
  String descript;
  String name;           // Название города
  float temp;            // Цельсий
  float feels_like;      // Ощущается как
  float pressure_hPa;    // Гектопаскали
  float pressure_mmHg;   // мм рт. ст.
  byte humidity;         // %
  int clouds_all;        // %
  float wind_speed;      // м/с
  float wind_gust;       // м/с (может отсутствовать)
  int wind_deg;          // градусы
};

WeatherData weather;          // текущая погода
//WeatherData weatherTomorrow;  // погода на завтра
//String forecastJson = "{}";


/*******************************
 * Время и таймзона
 *******************************/
#include <TimeLib.h>
#include <Timezone.h>
int8_t timezone = 7;                               // Time zone GTM
//bool isDayLightSaving = true;                      //Summer time use
String NtpServerName = "us.pool.ntp.org";         //time.nist.gov"
bool statusUpdateNtpTime = 1;                      //
const unsigned long TIME_UPDATE = 1000 * 60 * 10;  //Time update interval
const unsigned long NTP_UPDATE = 1000 * 30;        //Интервал обновления с NTP при отсутствии ответа сервера
unsigned long lastNtpTime = 0;

// Central European Time (Frankfurt, Paris)
TimeChangeRule CEST = { "CEST", Last, Sun, Mar, 2, 120 };  // Central European Summer Time
TimeChangeRule CET = { "CET ", Last, Sun, Oct, 3, 60 };    // Central European Standard Time
Timezone CE(CEST, CET);
TimeChangeRule* tcr;

/*******************************
 * Тревоги и уровни срабатывания
 *******************************/
  float lowlevltemp = 29;         // уровень срабатывания тревоги 1
  // float lowlevleCO2 = 600;        // уровень срабатывания тревоги 1
  float lowlevlCO2 = 900;         // уровень срабатывания тревоги 1 
  float lowlevlTVOC = 60;         // уровень срабатывания тревоги 1
  float lowlevlPM25 = 26;         // уровень срабатывания тревоги 1
  float lowlevlCO = 30;           // уровень срабатывания тревоги 1 

  float midlevltemp = 31;           // уровень срабатывания тревоги 2
  // float midlevleCO2 = 900;          // уровень срабатывания тревоги 2
  float midlevlCO2 = 1001;         // уровень срабатывания тревоги 2
  float midlevlTVOC = 101;         // уровень срабатывания тревоги 2
  float midlevlPM25 = 46;          // уровень срабатывания тревоги 2
  float midlevlCO = 200;          // уровень срабатывания тревоги 2

  float highlevltemp = 33;           // уровень срабатывания тревоги 3
  // float highlevleCO2 = 1600;          // уровень срабатывания тревоги 3
  float highlevlCO2 = 2000;         // уровень срабатывания тревоги 3
  float highlevlTVOC = 200;         // уровень срабатывания тревоги 3
  float highlevlPM25 = 66;          // уровень срабатывания тревоги 3
  float highlevlCO = 400;           // уровень срабатывания тревоги 3

  #define HOOD_PIN 0
  #define BUZ_PIN 2

//Hood
bool alarmLogged = false; // состояние тревоги

//BUZ
int currentLevel = 0; // например, установить в нужное значение

/*******************************
 * Индикаторы (опционально)
 *******************************/
#if USE_IND == true
int out = 0;
const int pins[] = { 32, 33, 25, 26 };  //Пины подключения стрелочных индикаторов

int AnalogArrow0, AnalogArrow1, AnalogArrow2, AnalogArrow3;
int pinArrow0, pinArrow1, pinArrow2, pinArrow3;
#endif

/*******************************
* временная функция Отображение параметров на дисплее
 *******************************/
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // ширина дисплея
#define SCREEN_HEIGHT 32 // высота дисплея
#define OLED_RESET -1   // сброс не используется, потому что он подключен к RESET пина
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

unsigned long previousMillis = 0;
const unsigned long interval = 5000; // интервал в миллисекундах (3 сек)
int currentIndex = 0;

/*******************************
 * Модификация прошивки и версия
 *******************************/
#if USE_IND == true
const String urts = "_Arrow";
#else
const String urts = "";
#endif

String Version = nName + nVersion + urts;

/*******************************
* Настройки по умолчанию
 *******************************/
bool ASound = false;  // Сирена по тревоге включена
bool AHood = false;   // Вытяжка по тревоге выключена

uint8_t asound = 0;  //(Temp, CO2, PM2.5, TVOC)
uint8_t ahood = 0;   //(CO2, PM2.5, TVOC)

#if USE_IND == true
bool ARROW0 = true;       // Индикатор 0
bool ARROW1 = false;      // Индикатор 1
bool ARROW2 = false;      // Индикатор 2
bool ARROW3 = false;      // Индикатор 3

uint8_t arrow0 = 1;       //(Temp, Hamm, Pres, CO2, CO, PM2.5, TVOC)
uint8_t arrow1 = 3;       //(Temp, Hamm, Pres, CO2, CO, PM2.5, TVOC)
uint8_t arrow2 = 4;       //(Temp, Hamm, Pres, CO2, CO, PM2.5, TVOC)
uint8_t arrow3 = 5;       //(Temp, Hamm, Pres, CO2, CO, PM2.5, TVOC)
#endif

//Лед подсветка
bool AddrLED = true;      // Включена подсветка
uint8_t Brightness = 100; // Значение яркости по умолчанию (0-255)
uint8_t ColorLED = 1;     // Цвет светодиодов: 0-green, 1-yellow, 2-blue, 3-white

//Активные датчики
bool BME280 = true;     // Включить BME280
bool SGP30 = false;      // Включить SGP30
bool PMS5003 = false;   // Включить PMS5003
bool Z19B = false;      // Включить Z19B
bool MQ7i = false;      // Включить MQ-7
bool MQ135i = false;    // Включить MQ-135

//Wi-Fi setting
const unsigned long WIFI_CONNECT_TIMEOUT = 5000;    // Таймаут для WiFi подключения (мс)
const long TIMER_INTERVAL = 5000;                    // Интервал таймера (мс)

const char* PARAM_SSID = "ssid";
const char* PARAM_PASS = "pass";

// //
String jsonConfig = "{}";
unsigned long timeCount = 0;  //Time counter for All

/*******************************
* Объявление прототипов функций
 *******************************/
void WIFI_init();
void initFS(void);
void server_httpinit(void);
void init_sensor();
boolean reconnect();
bool loadConfig();

void Test();
void arrows();
void alarm();
#endif           // CONFIG_H
