void server_httpinit() {
  /////////////////// Route for root / web page //////////////////////
  //server.on("/save_wifi", HTTP_POST, handle_SaveWifi);
  server.on("/scan_networks", HTTP_GET, handle_ScanNetworks);  // Обработчик маршрута scan_networks
  // server.on("/led", HTTP_GET, handleLed_Control);
  // server.on("/status", HTTP_GET, handle_Status);
  server.on("/info", HTTP_GET, handle_Info);
  ////////// Обработка обновления OTA //////////
  server.on(
    "/update", HTTP_POST, [](AsyncWebServerRequest *request) {}, handle_FileUpdate);
  ////////// Обработка обновления FS //////////
  server.on(
    "/edit", HTTP_POST, [](AsyncWebServerRequest *request) {}, handle_FileUpload);  // Обработка загрузки файла
  server.on("/edit", HTTP_DELETE, handle_Delete);                                   // Обработка удаления файла
  server.on("/list", HTTP_GET, handle_List);
  ////////// Time //////////
  server.on("/Time", HTTP_GET, handle_Time);             // Sync Time
  server.on("/TimeZone", HTTP_GET, handle_time_zone);    // Set time zone
                                                         //  server.on("/TimeSet", HTTP_GET, handle_time_set);
  server.on("/Ntpserver", HTTP_GET, handle_ntp_server);  // Set sync NTP
  ////////// Weather //////////
  server.on("/updateWeather", HTTP_GET, handle_UpdateWeather);
  server.on("/weather", HTTP_GET, handle_Weather);
  server.on("/setweather", HTTP_POST, handle_SetWeather);
  //server.on("/weatherTomorrow", HTTP_GET, handleWeatherTomorrow);
  ////////// configs //////////
  server.on("/configs.json", HTTP_GET, handle_ConfigJSON);  // create configs.json for sending to WEB interface
  server.on("/sensors", HTTP_GET, handle_Sensors);
  server.on("/restart", handle_Restart);  // reset ESP
  // server.on("/resetconfig", HTTP_GET, handle_resetConfig);
  server.on("/alarmsetup", HTTP_GET, handle_AlarmSetup);
  server.on("/alarmlevl", HTTP_GET, handle_AlarmLevl);
  server.on("/alarmstatus", HTTP_GET, handle_AlarmStatus);
  server.on("/sensorsetup", HTTP_GET, handle_SensorSetup);
#if USE_IND == true
  server.on("/arrowsetup", HTTP_GET, handle_ArrowSetup);
  server.on("/testON", HTTP_GET, handle_TestON);
  server.on("/testOFF", HTTP_GET, handle_TestOFF);
#endif
  server.on("/setColor", HTTP_GET, handle_SetColor);
}

////////////////////////////// Config /////////////////////////////
void handle_ConfigJSON(AsyncWebServerRequest *request) {
  // time_t tn = now();
  String root = "{}";

  JsonDocument jsonDoc;
  DeserializationError error = deserializeJson(jsonDoc, root);
  if (error) {
    Serial.print(F("deserializeJson() failed with code "));
    Serial.println(error.c_str());
    return;
  }
  JsonObject json = jsonDoc.as<JsonObject>();
  // Заполняем настроек

  ////////// Time /////////
  json["ntpserver"] = NtpServerName;
  json["timezone"] = timezone;
  json["time"] = GetTime(true);
  //json["date"] = GetDate();

  ////////// Weather /////////
  json["location"] = Location;
  json["apikey"] = API_Key;

  ////////// Alarm /////////
  json["ASound"] = (ASound ? "checked" : "");
  jsonDoc["asound"] = asound;
  json["AHood"] = (AHood ? "checked" : "");
  jsonDoc["ahood"] = ahood;

  ////////// Arrow /////////
#if USE_IND == true
  json["ARROW0"] = (ARROW0 ? "checked" : "");
  jsonDoc["arrow0"] = arrow0;
  json["ARROW1"] = (ARROW1 ? "checked" : "");
  jsonDoc["arrow1"] = arrow1;
  json["ARROW2"] = (ARROW2 ? "checked" : "");
  jsonDoc["arrow2"] = arrow2;
  json["ARROW3"] = (ARROW3 ? "checked" : "");
  jsonDoc["arrow3"] = arrow3;
#endif

  json["AddrLED"] = (AddrLED ? "checked" : "");
  jsonDoc["ColorLED"] = ColorLED;
  jsonDoc["Brightness"] = Brightness;

  ////////// Sensors /////////
  json["BME280"] = (BME280 ? "checked" : "");
  json["SGP30"] = (SGP30 ? "checked" : "");
  json["PMS5003"] = (PMS5003 ? "checked" : "");
  json["Z19B"] = (Z19B ? "checked" : "");
  json["MQ7i"] = (MQ7i ? "checked" : "");
  json["MQ135i"] = (MQ135i ? "checked" : "");

  json["lowlevltemp"] = lowlevltemp;
  // json["lowlevleCO2"] = lowlevleCO2;
  json["lowlevlCO2"] = lowlevlCO2;
  json["lowlevlCO"] = lowlevlCO;
  json["lowlevlTVOC"] = lowlevlTVOC;
  json["lowlevlPM25"] = lowlevlPM25;

  json["midlevltemp"] = midlevltemp;
  // json["midlevleCO2"] = midlevleCO2;
  json["midlevlCO2"] = midlevlCO2;
  json["midlevlCO"] = midlevlCO;
  json["midlevlTVOC"] = midlevlTVOC;
  json["midlevlPM25"] = midlevlPM25;

  json["highlevltemp"] = highlevltemp;
  // json["highlevleCO2"] = highlevleCO2;
  json["highlevlCO2"] = highlevlCO2;
  json["highlevlCO"] = highlevlCO;
  json["highlevlTVOC"] = highlevlTVOC;
  json["highlevlPM25"] = highlevlPM25;

  root = "";
  serializeJson(json, root);
  request->send(200, "text/json", root);
}


///////////////////////////// ScanNetworks /////////////////////////////
void handle_ScanNetworks(AsyncWebServerRequest *request) {

  int n = WiFi.scanNetworks();  // синхронное сканирование
  // Обработка после завершения сканирования
  //DynamicJsonDocument jsonDoc(1024);
  JsonDocument jsonDoc;
  //JsonArray networksArray = jsonDoc.createNestedArray("networks");
  JsonArray networksArray = jsonDoc["networks"].to<JsonArray>();
  for (int i = 0; i < n; ++i) {
    // JsonObject obj = networksArray.createNestedObject();
    JsonObject obj = networksArray.add<JsonObject>();
    obj["ssid"] = WiFi.SSID(i);
    obj["rssi"] = WiFi.RSSI(i);
    obj["channel"] = WiFi.channel(i);
    obj["bssid"] = WiFi.BSSIDstr(i);
    obj["auth"] = WiFi.encryptionType(i);
  }
  String response;
  serializeJson(jsonDoc, response);
  request->send(200, "application/json", response);
}


////////////////////////////// Info /////////////////////////////
void handle_Info(AsyncWebServerRequest *request) {

  // Собираем все данные в JSON объект
  JsonDocument json;

  // Добавляем данные в JSON
  json["Version"] = Version;

  json["chipId"] = chipId;        // Чип ID как число
  json["chipModel"] = chipModel;  //
  json["chipCores"] = chipCores;
  json["sketchSize"] = sketchSize;  //Размер скетча
  json["macAddr"] = macAddr;
  json["freeSketchSpace"] = freeSketchSpace;
  json["cpuFreqMHz"] = cpuFreqMHz;  // Частота CPU как число
  json["flashSize"] = flashSize;    // Размер флеш-памяти как число
  json["freeHeap"] = freeHeap;      // Размер свободной флеш-памяти как число
  json["chipModel"] = chipModel;    //

  // Температура ядра (переводим из Цельсия)
  uint8_t temp_raw = temprature_sens_read();
  float tempC = ((float)temp_raw - 32.0) / 1.8;  // Формула для вывода температуры в градусах цельсия
  json["coreTemperature"] = tempC;

  // Отправляем JSON клиенту
  String response;
  serializeJson(json, response);
  request->send(200, "application/json", response);
}


////////////////////////////// Update OTA /////////////////////////////
void handle_FileUpdate(AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final) {

  if (index == 0) {
    Serial.println("Starting update");
    if (!Update.begin(UPDATE_SIZE_UNKNOWN)) {  // Начинаем обновление
      Update.printError(Serial);
    }
  }

  if (len > 0) {
    if (Update.write(data, len) != len) {
      Update.printError(Serial);
    }
  }

  if (final) {
    if (Update.end(true)) {  // Перезагрузка после успешного обновления
      Serial.println("Update complete, restarting...");
      delay(1000);
      ESP.restart();
    } else {
      Update.printError(Serial);
    }
  }
}


////////////////////////////// Update FS /////////////////////////////
void handle_FileUpload(AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final) {
  static File uploadFile;
  if (index == 0) {
    // Начинаем новый файл
    if (!filename.startsWith("/")) filename = "/" + filename;
    uploadFile = LittleFS.open(filename, "w");
    if (!uploadFile) {
      request->send(500, "text/plain", "Failed to open file");
      return;
    }
  }
  if (len > 0) {
    // Записываем данные в файл
    if (uploadFile) {
      uploadFile.write(data, len);
    }
  }
  if (final) {
    // Завершение загрузки
    if (uploadFile) {
      uploadFile.close();
    }
    request->send(200, "text/plain", "OK");
  }
}

// Обработчик удаления файла (DELETE /edit?file=filename)
void handle_Delete(AsyncWebServerRequest *request) {
  if (request->hasArg("file")) {
    String filename = request->arg("file");
    if (!filename.startsWith("/")) filename = "/" + filename;
    if (LittleFS.exists(filename)) {
      LittleFS.remove(filename);
      request->send(200, "text/plain", "Deleted");
    } else {
      request->send(404, "text/plain", "File not found");
    }
  } else {
    request->send(400, "text/plain", "No file specified");
  }
}

// Обработчик получения списка файлов (/list?dir=/)
void handle_List(AsyncWebServerRequest *request) {
  String dirPath = "/";
  if (request->hasArg("dir")) {
    dirPath = request->arg("dir");
  }

  //DynamicJsonDocument jsonDoc(4096);
  JsonDocument jsonDoc;
  JsonArray arr = jsonDoc.to<JsonArray>();

  File root = LittleFS.open(dirPath);
  if (!root || !root.isDirectory()) {
    request->send(404, "application/json", "[]");
    return;
  }

  File file = root.openNextFile();
  while (file) {
    // JsonObject obj = arr.createNestedObject();
    JsonObject obj = arr.add<JsonObject>();
    obj["name"] = String(file.name());
    obj["type"] = file.isDirectory() ? "directory" : "file";
    obj["size"] = file.isDirectory() ? 0 : file.size();  // добавляем размер файла
    file = root.openNextFile();
  }

  String response;
  serializeJson(arr, response);
  request->send(200, "application/json", response);
}


////////////////////////////// TIME /////////////////////////////
void handle_time_zone(AsyncWebServerRequest *request) {
  //void handle_time_set(AsyncWebServerRequest *request) {
  //  NtpServerName = request->arg("ntpserver").c_str();
  timezone = request->arg("timezone").toInt();
  //isDayLightSaving = request->arg("isDayLightSaving").toInt();
  saveConfig();
  timeSync();
  //Serial.println("NTP Time Zone: " + String(timezone) + ",  isDayLightSaving: " + String(isDayLightSaving));
  request->send(200, "text/plain", "OK");
}

void handle_ntp_server(AsyncWebServerRequest *request) {
  NtpServerName = request->arg("ntpserver").c_str();
  saveConfig();
  timeSync();
  //Serial.println("NtpServerName: " + NtpServerName + ", useRTC: " + useRTC);
  request->send(200, "text/plain", "OK");
}

void handle_Time(AsyncWebServerRequest *request) {
  timeSync();
  request->send(200, "text/plain", "OK");
}


////////////////////////////// Weather //////////////////////////////
// Обработчик для получения текущих данных погоды в JSON формате
void handle_Weather(AsyncWebServerRequest *request) {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi not connected");
    return;
  }
  JsonDocument json;

  json["name"] = weather.name;
  json["description"] = weather.descript;
  json["temp"] = weather.temp;
  json["feels_like"] = weather.feels_like;
  json["pressure_hPa"] = weather.pressure_hPa;
  json["pressure_mmHg"] = weather.pressure_mmHg;
  json["humidity"] = weather.humidity;
  json["clouds_all"] = weather.clouds_all;
  json["wind_speed"] = weather.wind_speed;
  json["wind_gust"] = weather.wind_gust;

  String windDirStr = getWindDirection(weather.wind_deg);
  json["wind_direction"] = windDirStr;

  String response;
  serializeJson(json, response);
  request->send(200, "application/json", response);
  //Serial.println("Ответ /weather отправлен");
}

//volatile bool needFetchWeather = false;
void handle_SetWeather(AsyncWebServerRequest *request) {
  Location = request->arg("location");
  API_Key = request->arg("apikey");
  saveConfig();

  needFetchWeather = true;  // Устанавливаем флаг
  request->send(200, "text/plain", "OK");
}

void handle_UpdateWeather(AsyncWebServerRequest *request) {
  needFetchWeather = true;  // Устанавливаем флаг // повторно запрашиваем погоду
  request->send(200, "application/json", "{\"status\":\"ok\"}");
}


////////////////////////////// Alarm //////////////////////////////
void handle_AlarmSetup(AsyncWebServerRequest *request) {
  bool changed = false;

  // Обработка ASound
  int newASound = request->arg("ASound").toInt();
  if (newASound == 1 && !ASound) {
    ASound = true;
    changed = true;
  } else if (newASound != 1 && ASound) {
    ASound = false;
    changed = true;
  }

  int newasound = request->arg("asound").toInt();
  if (newasound != asound) {
    asound = newasound;
    changed = true;
  }

  // Обработка AHood
  int newAHood = request->arg("AHood").toInt();
  if (newAHood == 1 && !AHood) {
    AHood = true;
    changed = true;
  } else if (newAHood != 1 && AHood) {
    AHood = false;
    changed = true;
  }

  int newahood = request->arg("ahood").toInt();
  if (newahood != ahood) {
    ahood = newahood;
    changed = true;
  }

  // int newSHood = request->arg("SHood").toInt();
  // if (newSHood != SHood) {SHood = newSHood;changed = true;}

  if (changed) { saveConfig(); }
  request->send(200, "text/plain", "OK");
}

void handle_AlarmLevl(AsyncWebServerRequest *request) {
  // bool changed = false;
  lowlevltemp = request->arg("lowlevltemp").toInt();
  // lowlevleCO2 = request->arg("lowlevleCO2").toInt()
  lowlevlCO2 = request->arg("lowlevlCO2").toInt();
  lowlevlCO = request->arg("lowlevlCO").toInt();
  lowlevlTVOC = request->arg("lowlevlTVOC").toInt();
  lowlevlPM25 = request->arg("lowlevlPM25").toInt();

  midlevltemp = request->arg("midlevltemp").toInt();
  // midlevleCO2 = request->arg("midlevleCO2").toInt();
  midlevlCO2 = request->arg("midlevlCO2").toInt();
  midlevlCO = request->arg("midlevlCO").toInt();
  midlevlTVOC = request->arg("midlevlTVOC").toInt();
  midlevlPM25 = request->arg("midlevlPM25").toInt();

  highlevltemp = request->arg("highlevltemp").toInt();
  // highlevleCO2 = request->arg("highlevleCO2").toInt();
  highlevlCO2 = request->arg("highlevlCO2").toInt();
  highlevlCO = request->arg("highlevlCO").toInt();
  highlevlTVOC = request->arg("highlevlTVOC").toInt();
  highlevlPM25 = request->arg("highlevlPM25").toInt();

  saveConfig();
  request->send(200, "text/plain", "OK");
}

void handle_AlarmStatus(AsyncWebServerRequest *request) {
  // if (AlarmState) {
  //   request->send(200, "text/plain", "ON");
  // } else {
  //   request->send(200, "text/plain", "OFF");
  // }
}


////////////////////////////// Indication //////////////////////////////
void handle_ArrowSetup(AsyncWebServerRequest *request) {
#if USE_IND == true
  bool changed = false;

  // Обработка ARROW0
  int newARROW0 = request->arg("ARROW0").toInt();
  if (newARROW0 == 1 && !ARROW0) {
    ARROW0 = true;
    changed = true;
  } else if (newARROW0 != 1 && ARROW0) {
    ARROW0 = false;
    changed = true;
  }

  int newarrow0 = request->arg("arrow0").toInt();
  if (newarrow0 != arrow0) {
    arrow0 = newarrow0;
    changed = true;
  }

  // Обработка ARROW1
  int newARROW1 = request->arg("ARROW1").toInt();
  if (newARROW1 == 1 && !ARROW1) {
    ARROW1 = true;
    changed = true;
  } else if (newARROW1 != 1 && ARROW1) {
    ARROW1 = false;
    changed = true;
  }

  int newarrow1 = request->arg("arrow1").toInt();
  if (newarrow1 != arrow1) {
    arrow1 = newarrow1;
    changed = true;
  }

  // Обработка ARROW2
  int newARROW2 = request->arg("ARROW2").toInt();
  if (newARROW2 == 1 && !ARROW2) {
    ARROW2 = true;
    changed = true;
  } else if (newARROW2 != 1 && ARROW2) {
    ARROW2 = false;
    changed = true;
  }

  int newarrow2 = request->arg("arrow2").toInt();
  if (newarrow2 != arrow2) {
    arrow2 = newarrow2;
    changed = true;
  }

  // Обработка ARROW3
  int newARROW3 = request->arg("ARROW3").toInt();
  if (newARROW3 == 1 && !ARROW3) {
    ARROW3 = true;
    changed = true;
  } else if (newARROW3 != 1 && ARROW3) {
    ARROW3 = false;
    changed = true;
  }

  int newarrow3 = request->arg("arrow3").toInt();
  if (newarrow3 != arrow3) {
    arrow3 = newarrow3;
    changed = true;
  }

  if (changed) { saveConfig(); }
  request->send(200, "text/plain", "OK");
#endif
}

// Функции для обработки запросов
void handle_TestON(AsyncWebServerRequest *request) {
  Serial.println("Запрос testON получен");
  // Плавное увеличение до максимума
  testON();
  request->send(200, "text/plain", "OK");
}

void handle_TestOFF(AsyncWebServerRequest *request) {
  Serial.println("Запрос testOFF получен");
  // Плавное уменьшение
  testOFF();
  request->send(200, "text/plain", "OK");
}

void handle_SetColor(AsyncWebServerRequest *request) {
  bool changed = false;

  int newAddrLED = request->arg("AddrLED").toInt();
  if (newAddrLED == 1 && !AddrLED) {
    AddrLED = true;
    changed = true;
  } else if (newAddrLED != 1 && AddrLED) {
    AddrLED = false;
    changed = true;
  }

  int newColorLED = request->arg("ColorLED").toInt();
  if (newColorLED != ColorLED) {
    ColorLED = newColorLED;
    changed = true;
  }

  int newBrightness = request->arg("Brightness").toInt();
  if (newBrightness != Brightness) {
    Brightness = newBrightness;
    changed = true;
  }

  if (changed) { saveConfig(); }
  updateDiodes();
  request->send(200, "text/plain", "OK");
}


////////////////////////////// Sensors //////////////////////////////
void handle_Sensors(AsyncWebServerRequest *request) {

  JsonDocument json;  // создаем JSON-документ
                      /* ваши актуальные значения с датчиков*/
  if (BME280 == true) {
    json["temp"] = temp;
    json["hum"] = hum;
    json["pres"] = pres;
    json["alt"] = alt;
  }

  if (PMS5003 == true) {
    json["val1"] = val1;
    json["val2"] = val2;
    json["val3"] = val3;
  }

  if (Z19B == true) {
    json["co2"] = Co2;
  }

  if (MQ7i == true) {
    json["co"] = CO;
  }

  if (MQ135i == true) {
    // json["sv"] = String(current_sensor_value);
    // json["aq"] = String(current_air_quality);
    // json["aql"] = current_air_quality_label;
    json["Co"] = Co;
    json["CO2"] = CO2;
  }

  if (SGP30 == true) {
    json["tvoc"] = tvoc;
    json["eco2"] = eco2;
    json["h2"] = h2;
    json["eth"] = eth;
  }


  String response;
  serializeJson(json, response);
  //Serial.println(response);
  request->send(200, "application/json", response);
}


void handle_SensorSetup(AsyncWebServerRequest *request) {
  bool changed = false;

  // Обработка BME280
  int newBME280 = request->arg("BME280").toInt();
  if (newBME280 == 1 && !BME280) {
    BME280 = true;
    changed = true;
  } else if (newBME280 != 1 && BME280) {
    BME280 = false;
    changed = true;
  }

  // Обработка SGP30
  int newSGP30 = request->arg("SGP30").toInt();
  if (newSGP30 == 1 && !SGP30) {
    SGP30 = true;
    changed = true;
  } else if (newSGP30 != 1 && SGP30) {
    SGP30 = false;
    changed = true;
  }

  // Обработка PMS5003
  int newPMS5003 = request->arg("PMS5003").toInt();
  if (newPMS5003 == 1 && !PMS5003) {
    PMS5003 = true;
    changed = true;
  } else if (newPMS5003 != 1 && PMS5003) {
    PMS5003 = false;
    changed = true;
  }


  // Обработка Z19B
  int newZ19B = request->arg("Z19B").toInt();
  if (newZ19B == 1 && !Z19B) {
    Z19B = true;
    changed = true;
  } else if (newZ19B != 1 && Z19B) {
    Z19B = false;
    changed = true;
  }

  // Обработка MQ7i
  int newMQ7i = request->arg("MQ7i").toInt();
  if (newMQ7i == 1 && !MQ7i) {
    MQ7i = true;
    changed = true;
  } else if (newMQ7i != 1 && MQ7i) {
    MQ7i = false;
    changed = true;
  }

    // Обработка MQ135i
  int newMQ135i = request->arg("MQ135i").toInt();
  if (newMQ135i == 1 && !MQ135i) {
    MQ135i = true;
    changed = true;
  } else if (newMQ135i != 1 && MQ135i) {
    MQ135i = false;
    changed = true;
  }

  if (changed) { saveConfig(); }
  request->send(200, "text/plain", "OK");
}

////////////////////////////// Restart //////////////////////////////
void handle_Restart(AsyncWebServerRequest *request) {
  String restart = request->arg("device");
  if (restart == "ok") {
    request->send(200, "text/plain", "Reset OK");
    delay(100);
    ESP.restart();
  } else {
    request->send(200, "text/plain", "No Reset");
  }
}


////////////////////////////// resetConfig //////////////////////////////
/*void handle_resetConfig(AsyncWebServerRequest *request) {
  String deviceParam = request->arg("device");
  if (deviceParam == "ok") {
    // Форматирование FS при необходимости:
    LittleFS.remove("/myconfig.json");
    delay(3000);
    request->send(200, "text/plain", "OK");
    delay(3000);
    ESP.restart();
  } else {
    request->send(200, "text/plain", "No Reset");
  }
}
*/

// void handle_resetConfig(AsyncWebServerRequest *request) {
//    String deviceParam = request->arg("device");
//   if (deviceParam.equals("ok")) {
//     // Попытка удалить файл конфигурации
//     if (LittleFS.exists("/myconfig.json")) {
//        Serial.println("Файл существует, пытаюсь удалить...");
//       bool removed = LittleFS.remove("/myconfig.json");
//       if (!removed) {
//          Serial.println(removed ? "успешно" : "не удалось");
//         request->send(500, "text/plain", "Failed to remove config");
//         return;
//       }
//     }
//     // Можно отправить ответ сразу и перезагрузить устройство чуть позже
//     request->send(200, "text/plain", "OK");
//     delay(100); // Небольшая задержка перед перезагрузкой
//     ESP.restart();
//   } else {
//     request->send(200, "text/plain", "No Reset");
//   }
// }
