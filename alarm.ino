/**
Логика работы сигнала.
* при превышения показаний первого,второго или третьего уровней, включается сигнал с разной частотой и включается флаг - тревога
**/

// 3 уроверь
void AlarmSound3() {
  int notes[] = { 1000, 1200, 1000, 1200 };  // Быстрый тревожный сигнал
  int durations[] = { 200, 200, 200, 200 };
  for (int i = 0; i < 4; i++) {
    tone(BUZ_PIN, notes[i], durations[i]);
    delay(durations[i] + 50);
  }
  noTone(BUZ_PIN);
}
// 2 уроверь
void AlarmSound2() {
  int notes[] = { 800, 1000, 800, 1000 };  // Интенсивный, высокий
  int durations[] = { 300, 300, 300, 300 };
  for (int i = 0; i < 4; i++) {
    tone(BUZ_PIN, notes[i], durations[i]);
    delay(durations[i] + 50);
  }
  noTone(BUZ_PIN);
}
// 1 уровень
void AlarmSound1() {
  int notes[] = { 600, 700, 600, 700 };  // Раздражающий звук
  int durations[] = { 400, 400, 400, 400 };
  for (int i = 0; i < 4; i++) {
    tone(BUZ_PIN, notes[i], durations[i]);
    delay(durations[i] + 50);
  }
  noTone(BUZ_PIN);
}

// Объявление глобальных переменных
bool alarmLevel3 = false;
bool alarmLevel2 = false;
bool alarmLevel1 = false;

void resetAlarmLevels() {
  alarmLevel1 = false;
  alarmLevel2 = false;
  alarmLevel3 = false;
  digitalWrite(BUZ_PIN, LOW);
}


void alarm() {

  if (ASound == true) {  //Проверяем разрешена ли тревога

    /****************************** Temp ******************************/
    if (asound == 0) {

      if (BME280 == true) {

        // Высокий уровень (3)
        if (temp.toFloat() >= highlevltemp && !alarmLevel3) {
          AlarmSound3();
          alarmLevel3 = true;
          alarmLevel2 = false;
          alarmLevel1 = false;
          Serial.println("Alarm: sound 3");

        } else if (temp.toFloat() >= midlevltemp && temp.toFloat() < highlevltemp && !alarmLevel2) {
          AlarmSound2();
          alarmLevel2 = true;
          alarmLevel3 = false;
          alarmLevel1 = false;
          Serial.println("Alarm: sound 2");

        } else if (temp.toFloat() >= lowlevltemp && temp.toFloat() < midlevltemp && !alarmLevel1) {
          AlarmSound1();
          alarmLevel1 = true;
          alarmLevel2 = false;
          alarmLevel3 = false;
          Serial.println("Alarm: sound 1");

        } else if (temp.toFloat() < lowlevltemp) {
          resetAlarmLevels();
        }
      }
    }


    /****************************** Co2 ******************************/
    if (asound == 1) {  //Проверяем какой датчик выбран
      if (Z19B == true) {
        if (Co2.toFloat() >= highlevlCO2 && !alarmLevel3) {
          AlarmSound3();
          alarmLevel3 = true;
          alarmLevel2 = false;
          alarmLevel1 = false;
          Serial.println("Alarm: sound 3");

        } else if (Co2.toFloat() >= midlevlCO2 && Co2.toFloat() < highlevlCO2 && !alarmLevel2) {
          AlarmSound2();
          alarmLevel2 = true;
          alarmLevel3 = false;
          alarmLevel1 = false;
          Serial.println("Alarm: sound 2");

        } else if (Co2.toFloat() >= lowlevlCO2 && Co2.toFloat() < midlevlCO2 && !alarmLevel1) {
          AlarmSound1();
          alarmLevel1 = true;
          alarmLevel2 = false;
          alarmLevel3 = false;
          Serial.println("Alarm: sound 1");
        }
        // сбрасываем все уровни
        else if (Co2.toFloat() < lowlevlCO2) {
          resetAlarmLevels();
        }
      }
    }


    /****************************** val2 ******************************/
    if (asound == 2) {  //Проверяем какой датчик выбран
      if (PMS5003 == true) {

        if (val2.toFloat() >= highlevlPM25 && !alarmLevel3) {
          AlarmSound3();
          alarmLevel3 = true;
          alarmLevel2 = false;
          alarmLevel1 = false;
          Serial.println("Alarm: sound 3");

        } else if (val2.toFloat() >= midlevlPM25 && val2.toFloat() < highlevlPM25 && !alarmLevel2) {
          AlarmSound2();
          alarmLevel2 = true;
          alarmLevel3 = false;
          alarmLevel1 = false;
          Serial.println("Alarm: sound 2");

        } else if (val2.toFloat() >= lowlevlPM25 && val2.toFloat() < midlevlPM25 && !alarmLevel1) {
          AlarmSound1();
          alarmLevel1 = true;
          alarmLevel2 = false;
          alarmLevel3 = false;
          Serial.println("Alarm: sound 1");
        }
        //сбрасываем все уровни
        else if (val2.toFloat() < lowlevlPM25) {
          resetAlarmLevels();
        }
      }
    }


    /****************************** tvoc ******************************/
    if (asound == 3) {  //Проверяем какой датчик выбран
      if (SGP30 == true) {

        if (tvoc.toFloat() >= highlevlTVOC && !alarmLevel3) {
          AlarmSound3();
          alarmLevel3 = true;
          alarmLevel2 = false;
          alarmLevel1 = false;
          Serial.println("Alarm: sound 3");

        } else if (tvoc.toFloat() >= midlevlTVOC && tvoc.toFloat() < highlevlTVOC && !alarmLevel2) {  // Средний уровень:
          AlarmSound2();
          alarmLevel2 = true;
          alarmLevel3 = false;
          alarmLevel1 = false;
          Serial.println("Alarm: sound 2");

        } else if (tvoc.toFloat() >= lowlevlTVOC && tvoc.toFloat() < midlevlTVOC && !alarmLevel1) {
          AlarmSound1();
          alarmLevel1 = true;
          alarmLevel2 = false;
          alarmLevel3 = false;
          Serial.println("Alarm: sound 1");
        }
        //сбрасываем все уровни
        else if (tvoc.toFloat() < lowlevlTVOC) {
          resetAlarmLevels();
        }
      }
    }

    /****************************** CO ******************************/
    if (asound == 4) {  //Проверяем какой датчик выбран
      if (MQ7i == true) {
        if (CO.toFloat() >= highlevlCO && !alarmLevel3) {
          AlarmSound3();
          alarmLevel3 = true;
          alarmLevel2 = false;
          alarmLevel1 = false;
          Serial.println("Alarm: sound 3");

        } else if (CO.toFloat() >= midlevlCO && CO.toFloat() < highlevlCO && !alarmLevel2) {
          AlarmSound2();
          alarmLevel2 = true;
          alarmLevel3 = false;
          alarmLevel1 = false;
          Serial.println("Alarm: sound 2");

        } else if (CO.toFloat() >= lowlevlCO && CO.toFloat() < midlevlCO && !alarmLevel1) {
          AlarmSound1();
          alarmLevel1 = true;
          alarmLevel2 = false;
          alarmLevel3 = false;
          Serial.println("Alarm: sound 1");
        }
        // сбрасываем все уровни
        else if (CO.toFloat() < lowlevlCO) {
          resetAlarmLevels();
        }
      }
    }
  } else {  //BUZ
    digitalWrite(BUZ_PIN, LOW);
  }



  ///////////////////////////// HOOD /////////////////////////////
  if (AHood == true) {

    if (ahood == 0) {  //Проверяем какой датчик выбран

      if (Z19B == true) {
        if (Co2.toFloat() >= highlevlCO2) {  //Проверяем уровень датчика
          if (!alarmLogged) {
            digitalWrite(HOOD_PIN, HIGH);  // Включаем вытяжку
            Serial.println("Alarm: hood ON");
            alarmLogged = true;  // Отметить, что тревога сработала
          }
        } else {
          if (alarmLogged) {
            digitalWrite(HOOD_PIN, LOW);
            Serial.println("Alarm: hood OFF");
            alarmLogged = false;
          }
        }
      }
    } else {
      digitalWrite(HOOD_PIN, LOW);
    }


    if (ahood == 1) {  //Проверяем какой датчик выбран
      if (PMS5003 == true) {

        if (val2.toFloat() >= highlevlPM25) {  //Проверяем уровень датчика
          if (!alarmLogged) {
            digitalWrite(HOOD_PIN, HIGH);  // Включаем устройство
            Serial.println("Alarm: hood ON");
            alarmLogged = true;  // Отметить, что тревога сработала
          }
        } else {
          if (alarmLogged) {
            digitalWrite(HOOD_PIN, LOW);
            Serial.println("Alarm: hood OFF");
            alarmLogged = false;
          }
        }
      }
    } else {
      digitalWrite(HOOD_PIN, LOW);
    }


    if (ahood == 2) {  //Проверяем какой датчик выбран
      if (SGP30 == true) {
        if (tvoc.toFloat() >= highlevlTVOC) {  //Проверяем уровень датчика
          if (!alarmLogged) {
            digitalWrite(HOOD_PIN, HIGH);  // Включаем устройство
            Serial.println("Alarm: hood ON");
            alarmLogged = true;  // Отметить, что тревога сработала
          }
        } else {
          if (alarmLogged) {
            digitalWrite(HOOD_PIN, LOW);
            Serial.println("Alarm: hood OFF");
            alarmLogged = false;
          }
        }
      }
    } else {
      digitalWrite(HOOD_PIN, LOW);
    }

    if (ahood == 3) {  //Проверяем какой датчик выбран
      if (MQ7i == true) {
        if (CO.toFloat() >= highlevlCO) {  //Проверяем уровень датчика
          if (!alarmLogged) {
            digitalWrite(HOOD_PIN, HIGH);  // Включаем вытяжку
            Serial.println("Alarm: hood ON");
            alarmLogged = true;  // Отметить, что тревога сработала
          }
        } else {
          if (alarmLogged) {
            digitalWrite(HOOD_PIN, LOW);
            Serial.println("Alarm: hood OFF");
            alarmLogged = false;
          }
        }
      }
    } else {
      digitalWrite(HOOD_PIN, LOW);
    }

  } else {  //Hood
    digitalWrite(HOOD_PIN, LOW);
  }

}  //void alarm