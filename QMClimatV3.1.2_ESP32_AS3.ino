/* 
Версия ядра esp32 - 3.0.7  
Версия библиотеки ArduinoJson - 7
*/

/*---  Измененения в данной версии  ---*/
/* 
---  Только для ESP32 (из-зи большого количества необходимых пинов входов/выходов)
---  WiFi, информация о контроллере и веб перенесены на второе ядро
---  Другая обработка веб страници
---  Отображаемые датцики выбираются из веба. 
---  Добавлена подсветка приборов с индикацией превышения показаний. (адресные диоды)
---  Сохранение только измененного пункта меню (не перезаписывается весь блок, а только измененный параметр)
---  Изменена работа Alarm Sound
---  Изменена работа ARROW
---  Добавлен CO сенсор (MQ-7)
*/

#include "config.h"

bool restartFlag = false;

// Инициализация LittleFS
void initFS() {
  if (!LittleFS.begin()) {
    Serial.println("LittleFS mount error");
  } else {
    Serial.println("LittleFS successfully installed");
  }
}

// Инициализация Wi-Fi соединения
bool initWiFi() {
  if (ssid == "" || pass == "") {
    Serial.println("SSID or PASS not set");
    return false;
  }

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid.c_str(), pass.c_str());

  Serial.println("Connection to WiFi...");
  unsigned long startTime = millis();

  while (WiFi.status() != WL_CONNECTED && millis() - startTime < WIFI_CONNECT_TIMEOUT) {
    delay(500);
    Serial.print(".");
  }
  if (WiFi.status() == WL_CONNECTED) {
    Serial.print("\nIP адрес: ");
    Serial.println(WiFi.localIP());
    return true;
  } else {
    Serial.println("\n Failed to connect to WiFi");
    return false;
  }
}

/********************* INFO *********************/
// Температура ядра
#ifdef __cplusplus
extern "C" {
#endif
uint8_t temprature_sens_read();
#ifdef __cplusplus
}
#endif
// uint8_t temprature_sens_read();

// Объявление переменных
uint32_t chipId = 0;
uint8_t  chipCores = 0;
uint32_t sketchSize = 0;
uint32_t freeSketchSpace = 0;
uint32_t cpuFreqMHz = 0;
uint32_t flashSize = 0;
uint32_t freeHeap = 0;
const char *chipModel = 0;
String macAddr;

void getChip() {
  chipId = 0;
  for (int i = 0; i < 40; i += 8) {
    chipId |= ((ESP.getEfuseMac() >> i) & 0xff) << i;
  }
  chipModel = ESP.getChipModel();             // Модель МК
  chipCores = ESP.getChipCores();             // Количество ядер МК
  sketchSize = ESP.getSketchSize();           // Размер скетча
  freeSketchSpace = ESP.getFreeSketchSpace(); // Свободно памяти в области скетча
  cpuFreqMHz = getCpuFrequencyMhz();          // Функция для получения частоту чипа
  flashSize = ESP.getFlashChipSize();         // Функция для получения обьема flash памяти
  freeHeap = ESP.getFreeHeap();               // Функция для получения свободного обьема оперативной памяти
  // macAddr = ESP.getEfuseMac();
}



/********************* TaskCreatePinnedToCore 2 *********************/
void WiFiWebServerTask(void *pvParameters) {
  Serial.print("Task1 running on core ");
  Serial.println(xPortGetCoreID());

  getChip();

  loadWiFiConfig();
  bool wifiConnected = initWiFi();

  // Общие маршруты для обеих ситуаций
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(LittleFS, "/index.html", "text/html");  //, processor);
  });

  server.serveStatic("/", LittleFS, "/");
  server_httpinit();

  if (wifiConnected) {
    Serial.println("WiFi connected");
    fetchWeather();
  } else {
    // Настройка точки доступа (AP)
    Serial.println("Setting an Access Point");

    WiFi.softAP("QMClimat", NULL);

    IPAddress IP = WiFi.softAPIP();
    Serial.print("IP AP: ");
    Serial.println(IP);

    // Страница настроек Wi-Fi
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
      request->send(LittleFS, "/wifi_settings.html", "text/html");
    });


    // Обработка POST-запроса с JSON данными для сохранения SSID и пароля
    server.on("/", HTTP_POST, [](AsyncWebServerRequest *request) {
      if (request->hasParam("json", true)) {
        AsyncWebParameter *p = request->getParam("json", true);
        String jsonStr = p->value();
        //DynamicJsonDocument doc(256);
        JsonDocument doc;
        DeserializationError error = deserializeJson(doc, jsonStr);
        if (error) {
          Serial.print(F("JSON parsing error: "));
          Serial.println(error.c_str());
          request->send(400, "application/json", "{\"error\":\"Invalid JSON\"}");
          return;
        }
        if (doc["ssid"].is<JsonVariant>()) {
          ssid = String((const char *)doc["ssid"]);
          saveWiFiConfig();
          // Serial.print("SSID обновлен: ");
          // Serial.println(ssid);
        }

        if (doc["pass"].is<JsonVariant>()) {
          pass = String((const char *)doc["pass"]);
          saveWiFiConfig();
          // Serial.print("Пароль обновлен: ");
          // Serial.println(pass);
        }
        restartFlag = true;
        request->send(200, "application/json", "{\"status\":\"ok\"}");
      } else {
        request->send(400, "application/json", "{\"error\":\"Missing json parameter\"}");
      }
    });
  }

  server.begin();


  while (true) {
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}


void setup() {
  Serial.begin(9600);

  // Создаём задачу для Wi-Fi и веб-сервера на ядре 1
  xTaskCreatePinnedToCore(
    WiFiWebServerTask,   /* Функция задачи. */
    "WiFiWebServerTask", /* Имя задачи. */
    10000,               /* Размер стека */
    NULL,                /* Параметры задачи */
    1,                   /* Приоритет */
    NULL,                /* Выполняемая операция */
    0                    /* Номер ядра, на котором она должна выполняться */
  );


  // getChip();
  initFS();
  loadConfig();  //Serial.println(F("FileConfig"));

  pinMode(HOOD_PIN, OUTPUT);
  digitalWrite(HOOD_PIN, LOW);
  pinMode(BUZ_PIN, OUTPUT);
  digitalWrite(BUZ_PIN, LOW);
  //   pinMode(BUTTON_PIN, INPUT_PULLUP); // Используем внутренний подтягивающий резистор

  timeSync();  //Serial.println(F("Start Time"));

init_sensor();

if (PMS5003 == true ) Serial.println("Sensor started");

#if USE_IND == true
  Test();
#endif

  pixels.begin();
  pixels.show();
  updateDiodes();

  // server.begin();

  /////////////////////////////////////////////////
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // адрес дисплея 0x3C
    Serial.println(F("SSD1306 не найден"));
    for (;;); // бесконечный цикл, если дисплей не найден
  }
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

//  testanimate(logo_bmp, LOGO_WIDTH, LOGO_HEIGHT); // Animate bitmaps

/////////////////////////////////////////////////////
}

void loop() {
if(BME280 == true) readBME();

if(PMS5003 == true) pms5003();

if(SGP30 == true) sgp30();

if(Z19B == true) mhz19b();

if(MQ7i == true) mq7();

if(MQ135i == true) mq135();


#if USE_IND == true
  arrows();
#endif

  alarm();


  // pixels.show();
  updateDiodes();

 display.clearDisplay(); // Вывод данных на дисплей


  //////////////////// TIME /////////////////////
  if ((millis() < timeCount)) {  //If use device more than 50 days
    timeCount = millis();
    lastNtpTime = millis();
    //    ReconnectTime = millis();
  }
  if (statusUpdateNtpTime && (millis() - lastNtpTime >= TIME_UPDATE)) {  //Update time from NTP
    lastNtpTime = millis();
    //Serial.println("Update time");
    timeSync();
  }
  if (!statusUpdateNtpTime && (millis() - lastNtpTime >= NTP_UPDATE)) {  //When updating from NTP in the absence of a server response, we try to synchronize again
    lastNtpTime = millis();
    statusUpdateNtpTime = 1;
    //Serial.println("Try sync NTP");
    timeSync();
  }

  //////////////////// Weather /////////////////////
  if (needFetchWeather) {
    needFetchWeather = false;
    fetchWeather();
    //    fetchWeatherTomorrow();
  }


  if (restartFlag) {
    delay(1000);
    ESP.restart();
  }

  //  if(parseData()) {
    displayWeatherCycle();
  // }
}



//////////////////////////////////////////////////////////////////////////
void displayWeatherCycle() {
  parseData();
  static const int totalDisplays = 6; // всего 8 отображений
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    // Обновляем время следующего переключения
    previousMillis = currentMillis;

    // Очистка дисплея
    display.clearDisplay();
    // Вызов текущей функции
    switch (currentIndex) {
      case 0: displayDescription(); break;
      case 1: displayTemperature(); break;
      case 2: displayHumidity(); break;
      case 3: displayWind(); break;
      case 4: displayBme280(); break;
      case 5: displayPMS(); break;
    }
    display.display();
    currentIndex = (currentIndex + 1) % totalDisplays;
  }
}


void displayDescription() {
  display.setCursor(0, 12);
  display.println(weather.descript);
}

void displayTemperature() {
  char buffer[20];
  snprintf(buffer, sizeof(buffer), "Temp: %.1f C", weather.temp);
  display.setCursor(0, 10);
  display.println(buffer);
}

void displayHumidity() {
  char buffer[20];
  snprintf(buffer, sizeof(buffer), "Hamibity: %d%%", weather.humidity);
  display.setCursor(0, 8);
  display.println(buffer);
}

void displayWind() {
  char buffer[30];
  snprintf(buffer, sizeof(buffer), "Wind: %.1f m/c", weather.wind_speed);
  display.setCursor(0, 0);
  display.println(buffer);
}

void displayBme280() {
  display.setCursor(0, 1);
  display.println("Temp: " + String(temp) + " C");
  display.setCursor(0, 11);
  display.println("Ham:  " + String(hum) + " %");
  display.setCursor(0, 22);
  display.println("Pres: " + String(pres) + " mmHg");
}
void displayPMS() {
  // display.setCursor(0, 1);
  // display.println("ppm1:   " + String(val1) + " ug/m3");
  // display.setCursor(0, 11);
  // display.println("ppm2.5: " + String(val2) + " ug/m3");
  // display.setCursor(0, 22);
  // display.println("ppm10:  " + String(val3) + " ug/m3");
}
///////////////////////////////////////////////////////////////////////////////