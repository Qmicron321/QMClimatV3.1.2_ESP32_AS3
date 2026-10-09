
// Функция для получения данных погоды
void fetchWeather() {
       if (WiFi.status() != WL_CONNECTED) {
     Serial.println("WiFi not connected");
     return;
   }

  WiFiClient client;
  HTTPClient http;

  String requestUrl = "http://api.openweathermap.org/data/2.5/weather?q=" + Location + "&units=metric&lang=ru&appid=" + API_Key;
  Serial.println(requestUrl);

  if (http.begin(client, requestUrl)) {
    int httpCode = http.GET();
    if (httpCode > 0) {
      if (httpCode == HTTP_CODE_OK) {
        jsonConfig = http.getString();
        //Serial.println("Данные погоды получены");
        parseData();
      } else {
        Serial.printf("Ошибка HTTP: %d\n", httpCode);
      }
    } else {
      Serial.printf("Ошибка запроса погоды: %s\n", http.errorToString(httpCode).c_str());
    }
    http.end();
  } else {
    Serial.println("Не удалось подключиться к серверу погоды");
  }
}

// Парсинг данных погоды
bool parseData() {
  JsonDocument jsonBuffer;
  auto error = deserializeJson(jsonBuffer, jsonConfig);
  if (error) {
    Serial.print("Ошибка десериализации погоды: ");
    Serial.println(error.c_str());
    return false;
  }

  JsonObject root = jsonBuffer.as<JsonObject>();
  weather.name = root["name"].as<const char*>();

  auto weatherArray = root["weather"].as<JsonArray>();
  if (weatherArray.size() > 0) {
    JsonObject weatherObj = weatherArray[0];
    weather.descript = weatherObj["description"].as<const char*>();
    weather.descript = translateDescription(weather.descript);

    JsonObject mainObj = root["main"];
    weather.temp = mainObj["temp"];
    weather.feels_like = mainObj["feels_like"];
    weather.humidity = mainObj["humidity"];
    weather.pressure_hPa = mainObj["pressure"];
    weather.pressure_mmHg = weather.pressure_hPa * 0.750061;

    JsonObject cloudsObj = root["clouds"];
    weather.clouds_all = cloudsObj["all"];

    JsonObject windObj = root["wind"];
    weather.wind_speed = windObj["speed"].as<float>();
    weather.wind_gust = windObj["gust"].as<float>();
    // weather.wind_gust = (windObj.containsKey("gust") && !windObj["gust"].isNull()) ? windObj["gust"].as<float>() : 0;
    weather.wind_deg = windObj["deg"];

    //Serial.println("Данные погоды успешно распарсены");
  } else {
  //  Serial.println("Нет данных о погоде");
    return false;
  }
  return true;
}

// Функция перевода описания погоды на русский
// String translateDescription(const String& desc) {
//   if (desc == "clear sky") return "Ясно";
//   else if (desc == "few clouds") return "Малооблачно";
//   else if (desc == "scattered clouds") return "Рассеянные облака";
//   else if (desc == "broken clouds") return "Облачно с прояснениями";
//   else if (desc == "overcast clouds") return "Пасмурно";
//   else if (desc == "light snow") return "Легкий снег";
//   else if (desc == "moderate snow") return "Умеренный снег";
//   else if (desc == "heavy snow") return "Сильный снег";
//   else if (desc == "snow showers") return "Снегопад";
//   else if (desc == "mist") return "Мгла";
//   else if (desc == "fog") return "Туман";
//   else if (desc == "haze") return "Мгла/дымка";
//   else if (desc == "thunderstorm") return "Гроза";
//   else if (desc == "light rain") return "Легкий дождь";
//   else if (desc == "moderate rain") return "Умеренный дождь";
//   else if (desc == "heavy rain") return "Сильный дождь";
//   else if (desc == "freezing rain") return "Мокрый снег или ледяной дождь";
//   else if (desc == "drizzle") return "Морось";
//   else if (desc == "shower rain") return "Ливень";
//   else if (desc == "rain") return "Дождь";

//   else return desc; // если перевод не найден, возвращаем исходный текст
// }
String translateDescription(const String& desc) {
  if (desc == "clear sky")            return "Ясно";
  if (desc == "few clouds")           return "Малооблачно";
  if (desc == "scattered clouds")     return "Рассеянные облака";
  if (desc == "broken clouds")        return "Облачно с прояснениями";
  if (desc == "overcast clouds")      return "Пасмурно";
  if (desc == "light rain")           return "Легкий дождь";
  if (desc == "moderate rain")        return "Умеренный дождь";
  if (desc == "heavy rain")           return "Сильный дождь";
  if (desc == "drizzle")              return "Морось";
  if (desc == "shower rain")          return "Ливень";
  if (desc == "thunderstorm")         return "Гроза";
  if (desc == "mist")                 return "Мгла";
  if (desc == "fog")                  return "Туман";
  if (desc == "haze")                 return "Дымка";
  if (desc == "light snow")           return "Легкий снег";
  if (desc == "moderate snow")        return "Умеренный снег";
  if (desc == "heavy snow")           return "Сильный снег";
  if (desc == "snow showers")         return "Снегопад";
  if (desc == "freezing rain")        return "Мокрый снег/ледяной дождь";
  // если не нашли перевод
  return desc;
}
// Определение направления ветра по градусам
// String getWindDirection(int deg) {
//   if (deg >= 337.5 || deg < 22.5) return "Север";
//   if (deg >= 22.5 && deg < 67.5) return "Северо-Восток";
//   if (deg >= 67.5 && deg < 112.5) return "Восток";
//   if (deg >= 112.5 && deg < 157.5) return "Юго-Восток";
//   if (deg >= 157.5 && deg < 202.5) return "Юг";
//   if (deg >= 202.5 && deg < 247.5) return "Юго-Запад";
//   if (deg >= 247.5 && deg < 292.5) return "Запад";
//   return "Северо-Запад";
// }
String getWindDirection(int deg) {
  static const char* dirs[] = {
    "Север", "Северо-Восток", "Восток", "Юго-Восток",
    "Юг", "Юго-Запад", "Запад", "Северо-Запад"
  };
  int idx = (int)((deg + 22.5) / 45) & 7;
  return String(dirs[idx]);
}