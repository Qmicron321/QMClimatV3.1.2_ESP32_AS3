#if USE_IND == true


/****************************** Test arrows ******************************/
// Функция для записи аналогового значения на все пины
void writeAnalog(int value) {
  for (int pin : pins) {
    analogWrite(pin, value);
  }
}

void testON() {
    for (int i = 0; i <= 255; i++) {
        writeAnalog(i);
        Serial.print("Value: ");
        Serial.println(i); ;
    }
}

void testOFF() {
    for (int i = 255; i >= 0; i--) {
        writeAnalog(i);
        Serial.print("Value: ");
        Serial.println(i); ;
    }
}


void Test() {
  // Плавное увеличение
  for (int i = 1; i <= 255; i++) {
    writeAnalog(i);
    delay(20);
  }
  // Плавное уменьшение
  for (int i = 255; i >= 1; i--) {
    writeAnalog(i);
    delay(20);
  }
  delay(1000);
}

/****************************** indication arrows ******************************/
/*
void arrows() {
  ///////////////////////////// индикатор 1 /////////////////////////////
  if (ARROW0 == true) {
    pinArrow0 = 32;

    if (arrow0 == 0) {
      if (BME280 == true) {
        AnalogArrow0 = map(temp.toFloat(), 18, 35, 0, 255);  //Устанавливаем минимум и максимум шкалы
      } else {
        analogWrite(pinArrow0, LOW);
        return;
      }
    }

    else if (arrow0 == 1) {
      if (BME280 == true) {
        AnalogArrow0 = map(hum.toFloat(), 10, 70, 0, 255);  // устанавливаем минимум и максимум шкалы
      } else {
        analogWrite(pinArrow0, LOW);
        return;
      }
    }

    else if (arrow0 == 2) {
      if (BME280 == true) {
        AnalogArrow0 = map(pres.toFloat(), 725, 780, 0, 255);  //Устанавливаем минимум и максимум шкалы
      } else {
        analogWrite(pinArrow0, LOW);
        return;
      }
    }

    else if (arrow0 == 3) {
      if (Z19B == true) {
        AnalogArrow0 = map(Co2.toFloat(), 400, 1800, 0, 255);  //Устанавливаем минимум и максимум шкалы
      } else {
        analogWrite(pinArrow0, LOW);
        return;
      }
    }

    else if (arrow0 == 4) {
      if (PMS5003 == true) {
        AnalogArrow0 = map(val2.toFloat(), 0, 8, 0, 255);  //Устанавливаем минимум и максимум шкалы
      } else {
        analogWrite(pinArrow0, LOW);
        return;
      }
    }

    else if (arrow0 == 5) {
      if (SGP30 == true) {
        AnalogArrow0 = map(tvoc.toFloat(), 0, 200, 0, 255);  //Устанавливаем минимум и максимум шкалы
      } else {
        analogWrite(pinArrow0, LOW);
        return;
      }
    }
    analogWrite(pinArrow0, AnalogArrow0);
  } else {
    analogWrite(pinArrow0, LOW);
    return;
  }


  //////////////////////////// индикатор 2 /////////////////////////////
  if (ARROW1 == true) {
    pinArrow1 = 33;

    if (arrow1 == 0) {
      if (BME280 == true) {
        AnalogArrow1 = map(temp.toFloat(), 18, 35, 0, 255);  //Устанавливаем минимум и максимум шкалы
      } else {
        analogWrite(pinArrow1, LOW);
        return;
      }
    }

    else if (arrow1 == 1) {
      if (BME280 == true) {
        AnalogArrow1 = map(hum.toFloat(), 10, 70, 0, 255);  // устанавливаем минимум и максимум шкалы
      } else {
        analogWrite(pinArrow1, LOW);
        return;
      }
    }

    else if (arrow1 == 2) {
      if (BME280 == true) {
        AnalogArrow1 = map(pres.toFloat(), 725, 780, 0, 255);  //Устанавливаем минимум и максимум шкалы
      } else {
        analogWrite(pinArrow1, LOW);
        return;
      }
    }

    else if (arrow1 == 3) {
      if (Z19B == true) {
        AnalogArrow1 = map(Co2.toFloat(), 400, 1800, 0, 255);  //Устанавливаем минимум и максимум шкалы
      } else {
        analogWrite(pinArrow1, LOW);
        return;
      }
    }

    else if (arrow1 == 4) {
      if (PMS5003 == true) {
        AnalogArrow1 = map(val2.toFloat(), 0, 8, 0, 255);  //Устанавливаем минимум и максимум шкалы
      } else {
        analogWrite(pinArrow1, LOW);
        return;
      }
    }

    else if (arrow1 == 5) {
      if (SGP30 == true) {
        AnalogArrow1 = map(tvoc.toFloat(), 0, 200, 0, 255);  //Устанавливаем минимум и максимум шкалы
      } else {
        analogWrite(pinArrow1, LOW);
        return;
      }
    }
    analogWrite(pinArrow1, AnalogArrow1);
  } else {
    analogWrite(pinArrow1, LOW);
    return;
  }


  ///////////////////////////// индикатор 3 /////////////////////////////
  if (ARROW2 == true) {
    pinArrow2 = 25;

    if (arrow2 == 0) {
      if (BME280 == true) {
        AnalogArrow2 = map(temp.toFloat(), 18, 35, 0, 255);  //Устанавливаем минимум и максимум шкалы
      } else {
        analogWrite(pinArrow2, LOW);
        return;
      }
    }

    else if (arrow2 == 1) {
      if (BME280 == true) {
        AnalogArrow2 = map(hum.toFloat(), 10, 70, 0, 255);  // устанавливаем минимум и максимум шкалы
      } else {
        analogWrite(pinArrow2, LOW);
        return;
      }
    }

    else if (arrow2 == 2) {
      if (BME280 == true) {
        AnalogArrow2 = map(pres.toFloat(), 725, 780, 0, 255);  //Устанавливаем минимум и максимум шкалы
      } else {
        analogWrite(pinArrow2, LOW);
        return;
      }
    }

    else if (arrow2 == 3) {
      if (Z19B == true) {
        AnalogArrow2 = map(Co2.toFloat(), 400, 1800, 0, 255);  //Устанавливаем минимум и максимум шкалы
      } else {
        analogWrite(pinArrow2, LOW);
        return;
      }
    }

    else if (arrow2 == 4) {
      if (PMS5003 == true) {
        AnalogArrow2 = map(val2.toFloat(), 0, 8, 0, 255);  //Устанавливаем минимум и максимум шкалы
      } else {
        analogWrite(pinArrow2, LOW);
        return;
      }
    }

    else if (arrow2 == 5) {
      if (SGP30 == true) {
        AnalogArrow2 = map(tvoc.toFloat(), 0, 200, 0, 255);  //Устанавливаем минимум и максимум шкалы
      } else {
        analogWrite(pinArrow2, LOW);
        return;
      }
    }
    analogWrite(pinArrow2, AnalogArrow2);
  } else {
    analogWrite(pinArrow2, LOW);
    return;
  }


  ///////////////////////////// индикатор 4 /////////////////////////////
  if (ARROW3 == true) {

    pinArrow3 = 26;

    if (arrow3 == 0) {
      if (BME280 == true) {
        AnalogArrow3 = map(temp.toFloat(), 18, 35, 0, 255);  //Устанавливаем минимум и максимум шкалы
      } else {
        analogWrite(pinArrow3, LOW);
        return;
      }
    }

    else if (arrow3 == 1) {
      if (BME280 == true) {
        AnalogArrow3 = map(hum.toFloat(), 10, 70, 0, 255);  // устанавливаем минимум и максимум шкалы
      } else {
        analogWrite(pinArrow3, LOW);
        return;
      }
    }

    else if (arrow3 == 2) {
      if (BME280 == true) {
        //  int pres = bme.readPressure() / 134.1F;        //Считываем параметр
        AnalogArrow3 = map(pres.toFloat(), 725, 780, 0, 255);  //Устанавливаем минимум и максимум шкалы
      } else {
        analogWrite(pinArrow3, LOW);
        return;
      }
    }

    else if (arrow3 == 3) {
      if (Z19B == true) {
        AnalogArrow3 = map(Co2.toFloat(), 400, 1800, 0, 255);  //Устанавливаем минимум и максимум шкалы
      } else {
        analogWrite(pinArrow3, LOW);
        return;
      }
    }

    else if (arrow3 == 4) {
      if (PMS5003 == true) {
        AnalogArrow3 = map(val2.toFloat(), 0, 8, 0, 255);  //Устанавливаем минимум и максимум шкалы
      } else {
        analogWrite(pinArrow3, LOW);
        return;
      }
    }

    else if (arrow3 == 5) {
      if (SGP30 == true) {
        AnalogArrow3 = map(tvoc.toFloat(), 0, 200, 0, 255);  //Устанавливаем минимум и максимум шкалы
      } else {
        analogWrite(pinArrow3, LOW);
        return;
      }
    }
    analogWrite(pinArrow3, AnalogArrow3);
  } else {
    analogWrite(pinArrow3, LOW);
    return;
  }
}
*/



 // Функция для обработки индикаторов
    void handleArrow(int pin, int arrowIndex, int arrow, bool BME280, bool Z19B, bool PMS5003, bool SGP30, bool MQ7i) {
        int AnalogArrow = 0;
        switch (arrow) {
            case 0:
                if (BME280) {
                    AnalogArrow = map(temp.toFloat(), 18, 35, 0, 255);
                }
                break;
            case 1:
                if (BME280) {
                    AnalogArrow = map(hum.toFloat(), 10, 70, 0, 255);
                }
                break;
            case 2:
                if (BME280) {
                    AnalogArrow = map(pres.toFloat(), 725, 780, 0, 255);
                }
                break;
            case 3:
                if (Z19B) {
                    AnalogArrow = map(Co2.toFloat(), 400, 1800, 0, 255);
                }
                break;
            case 4:
                if (PMS5003) {
                    AnalogArrow = map(val2.toFloat(), 0, 8, 0, 255);
                }
                break;
            case 5:
                if (SGP30) {
                    AnalogArrow = map(tvoc.toFloat(), 0, 200, 0, 255);
                }
                break;
            case 6:
                if (MQ7i) {
                    AnalogArrow = map(CO.toFloat(), 0, 250, 0, 255);
                }
                break;
            default:
                analogWrite(pin, LOW);
                return;
        }
        // Вывод значений в последовательный порт с номером индикатора
        // Serial.print("Indicator: ");
        // Serial.print(arrowIndex);  // Вывод номера индикатора
        // Serial.print(", Arrow: ");
        // Serial.print(arrow);
        // Serial.print(", Value: ");
        // Serial.println(AnalogArrow);

        // Запись значения на пин
        analogWrite(pin, AnalogArrow);
    }

void arrows() {
    if (ARROW0) {
        handleArrow(pinArrow0 = 32, 0, arrow0, BME280, Z19B, PMS5003, SGP30, MQ7i);
    } else {
        analogWrite(pinArrow0, LOW);
    }

    if (ARROW1) {
        handleArrow(pinArrow1 = 33, 1, arrow1, BME280, Z19B, PMS5003, SGP30, MQ7i);
    } else {
        analogWrite(pinArrow1, LOW);
    }

    if (ARROW2) {
        handleArrow(pinArrow2 = 25, 2, arrow2, BME280, Z19B, PMS5003, SGP30, MQ7i);
    } else {
        analogWrite(pinArrow2, LOW);
    }

    if (ARROW3) {
        handleArrow(pinArrow3 = 26, 3, arrow3, BME280, Z19B, PMS5003, SGP30, MQ7i);
    } else {
        analogWrite(pinArrow3, LOW);
    }
}

#endif
