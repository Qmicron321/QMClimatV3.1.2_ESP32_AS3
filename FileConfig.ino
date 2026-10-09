//////////////// Load saved config from myconfig.json ////////////

// Загрузка логина и пароля из файла
bool loadWiFiConfig() {
  if (!LittleFS.exists("/wifi_config.json")) {
    Serial.println("Файл wifi_config не найден");
    return false;
  }

  File wifiFile = LittleFS.open("/wifi_config.json", "r");
  if (!wifiFile || wifiFile.isDirectory()) {
    Serial.println("Не удалось открыть wifi_config");
    return false;
  }

  size_t size = wifiFile.size();
  if (size == 0) {
    Serial.println("Пустой файл wifi_config");
    wifiFile.close();
    return false;
  }

  String jsonStr = wifiFile.readString();
  wifiFile.close();

  //DynamicJsonDocument jsonDoc(1024);
  JsonDocument jsonDoc;
  DeserializationError error = deserializeJson(jsonDoc, jsonStr);
  if (error) {
    Serial.print("Ошибка десериализации wifi_config: ");
    Serial.println(error.c_str());
    return false;
  }

 JsonObject obj = jsonDoc.as<JsonObject>();

  if (obj["ssid"].is<const char*>() && obj["pass"].is<const char*>()) {
    ssid = String(obj["ssid"].as<const char*>());
    pass = String(obj["pass"].as<const char*>());
    return true;
  } else {
    Serial.println("Ключи ssid или pass отсутствуют");
    return false;
  }
}

// Сохранение логина и пароля в файл
bool saveWiFiConfig() {
  //DynamicJsonDocument jsonDoc(1024);
  JsonDocument jsonDoc;
  jsonDoc["ssid"] = ssid;
  jsonDoc["pass"] = pass;

  File wifiFile = LittleFS.open("/wifi_config.json", "w");
  if (!wifiFile) {
    Serial.println("Не удалось открыть wifi_config для записи");
    return false;
  }

  size_t bytesWritten = serializeJson(jsonDoc, wifiFile);
  wifiFile.close();

  if (bytesWritten == -1) {
    Serial.println("Ошибка сериализации wifi_config");
    return false;
  }
  
  Serial.println("wifi_config сохранены");
  return true;
}


///////////////////// Global Config /////////////////////
bool loadConfig() {
  File configFile = LittleFS.open("/myconfig.json", "r");  
  if (!configFile) {            // not found
    Serial.println(F("Failed to open config file"));
    saveConfig();               //  create file
    configFile.close();
    return false;
  }
  size_t size = configFile.size();   // myconfig.json must be less 2048 byte
  if (size > 2048) {  
    Serial.println(F("Config file size is too large"));
    configFile.close();
    return false;
  }
  jsonConfig = configFile.readString(); // load config 
  configFile.close();
   //DynamicJsonDocument jsonDoc(5096); //4096
   JsonDocument jsonDoc; 
   DeserializationError error = deserializeJson(jsonDoc, jsonConfig);
   if (error) {
    //  Serial.print(F("deserializeJson() failed with code "));
    //  Serial.println(error.c_str());
      return false;
   }   
   JsonObject root = jsonDoc.as<JsonObject>();   
   
  timezone = root["timezone"];   
  NtpServerName = root["ntpserver"].as<String>();

  Location = root["location"].as<String>();
  API_Key = root["apikey"].as<String>();

  ASound = root["ASound"];
  asound = root["asound"];
  AHood = root["AHood"];
  ahood = root["ahood"];
  // SHood = root["SHood"];
  
  #if USE_IND == true
    ARROW0 = root["ARROW0"];
    ARROW1 = root["ARROW1"];
    ARROW2 = root["ARROW2"];
    ARROW3 = root["ARROW3"];
    arrow0 = root["arrow0"];
    arrow1 = root["arrow1"];
    arrow2 = root["arrow2"];
    arrow3 = root["arrow3"];
    AddrLED = root["AddrLED"];
    ColorLED = root["ColorLED"];
    Brightness = root["Brightness"];
  #endif
  
    BME280 = root["BME280"];
    SGP30 = root["SGP30"];
    PMS5003 = root["PMS5003"];
    Z19B = root["Z19B"];
    MQ7i = root["MQ7i"];
    MQ135i = root["MQ135i"];

  lowlevltemp = root["lowlevltemp"];
  // lowlevleCO2 = root["lowlevleCO2"];
  lowlevlCO2 = root["lowlevlCO2"];
  lowlevlCO = root["lowlevlCO"];
  lowlevlTVOC = root["lowlevlTVOC"];
  lowlevlPM25 = root["lowlevlPM25"];

  midlevltemp = root["midlevltemp"];
  // midlevleCO2 = root["midlevleCO2"];
  midlevlCO2 = root["midlevlCO2"];
  midlevlCO = root["midlevlCO"];
  midlevlTVOC  = root["midlevlTVOC"];
  midlevlPM25 = root["midlevlPM25"];

  highlevltemp = root["highlevltemp"];
  // highlevleCO2 = root["highlevleCO2"];
  highlevlCO2 = root["highlevlCO2"];
  highlevlCO = root["highlevlCO"];
  highlevlTVOC = root["highlevlTVOC"];
  highlevlPM25 = root["highlevlPM25"];

  return true;
  
}

///////////////// Write to myconfig.json ////////////////////////
bool saveConfig() {
  //DynamicJsonDocument jsonDoc(5096);//4096
  JsonDocument jsonDoc;
  DeserializationError error = deserializeJson(jsonDoc, jsonConfig);
  if (error) {
    //  Serial.print(F("deserializeJson() failed with code "));
    // Serial.println(error.c_str());
      return false;
  }     
  JsonObject json = jsonDoc.as<JsonObject>();  

  json["timezone"] = timezone;
  json["ntpserver"] = NtpServerName;

  json["location"] = Location;
  json["apikey"] = API_Key;
  
  json["ASound"] = ASound;
  json["asound"] = asound;
  json["AHood"] = AHood;
  json["ahood"] = ahood;
  // json["SHood"] = SHood;

  #if USE_IND == true
    json["ARROW0"] = ARROW0;
    json["ARROW1"] = ARROW1;
    json["ARROW2"] = ARROW2;
    json["ARROW3"] = ARROW3;
    json["arrow0"] = arrow0;
    json["arrow1"] = arrow1;
    json["arrow2"] = arrow2;
    json["arrow3"] = arrow3;
    json["AddrLED"] = AddrLED;
    json["ColorLED"] = ColorLED;
    json["Brightness"] = Brightness;
  #endif

    json["BME280"] = BME280;
    json["SGP30"] = SGP30;
    json["PMS5003"] = PMS5003;
    json["Z19B"] = Z19B;
    json["MQ7i"] = MQ7i;
    json["MQ135i"] = MQ135i;

  json["lowlevltemp"] = lowlevltemp;
  //  json["lowlevleCO2"] = lowlevleCO2;
  json["lowlevlCO2"] = lowlevlCO2;
  json["lowlevlCO"] = lowlevlCO;
  json["lowlevlTVOC"] = lowlevlTVOC;
  json["lowlevlPM25"] = lowlevlPM25;

  json["midlevltemp"] = midlevltemp;
  // json["midlevleCO2"] = midlevleCO2;
  json["midlevlCO2"] =  midlevlCO2;
  json["midlevlCO"] =  midlevlCO;
  json["midlevlTVOC"] = midlevlTVOC;
  json["midlevlPM25"] = midlevlPM25;

  json["highlevltemp"] =  highlevltemp;
  // json["highlevleCO2"] = highlevleCO2;
  json["highlevlCO2"] = highlevlCO2;
  json["highlevlCO"] = highlevlCO;
  json["highlevlTVOC"] = highlevlTVOC;
  json["highlevlPM25"] = highlevlPM25;


 serializeJson(json, jsonConfig);
  File configFile = LittleFS.open("/myconfig.json", "w");
  if (!configFile) {
    Serial.println(F("Failed to open config file for writing"));
    configFile.close();
    return false;
  }
  serializeJson(json, configFile);
  configFile.close();
  return true;
}
