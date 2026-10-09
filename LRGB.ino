
void updateDiodes() {
  if (AddrLED == true) {
    pixels.setBrightness(Brightness);
    uint8_t r, g, b;

    if (ColorLED == 0) {
      r = 0;
      g = 255;
      b = 0;  //Зеленый
    } else if (ColorLED == 1) {
      r = 255;
      g = 255;
      b = 0;  //Жолтый
    } else if (ColorLED == 2) {
      r = 0;
      g = 0;
      b = 255;  //Синий
    } else if (ColorLED == 3) {
      r = 255;
      g = 255;
      b = 255;  //Белый установлен по умолчанию
    } else {
      r = 0;
      g = 0;
      b = 0;
    }
    pixels.show();

    // Устанавливаем цвет всех диодов
    for (int i = 0; i < NUM_LEDS; i++) {
      pixels.setPixelColor(i, pixels.Color(r, g, b));
    }


    ////// Подсветка приборов при превышении ////////
    if (ARROW0 == true) {
      if (arrow0 == 0) {
        if (BME280 == true) {
          if (temp.toFloat() > lowlevltemp) { pixels.setPixelColor(0, pixels.Color(255, 0, 0)); }
        }
      } else if (arrow0 == 3) {
        if (Z19B == true) {
          if (Co2.toFloat() > lowlevlCO2) { pixels.setPixelColor(0, pixels.Color(255, 0, 0)); }
        }
      } else if (arrow0 == 4) {
        if (PMS5003 == true) {
          if (val2.toFloat() > lowlevlPM25) { pixels.setPixelColor(0, pixels.Color(255, 0, 0)); }
        }
      } else if (arrow0 == 5) {
        if (SGP30 == true) {
          if (tvoc.toFloat() > lowlevlTVOC) { pixels.setPixelColor(0, pixels.Color(255, 0, 0)); }
        }
      } else if (arrow0 == 6) {
        if (MQ7i == true) {
          if (CO.toFloat() > lowlevlCO) { pixels.setPixelColor(0, pixels.Color(255, 0, 0)); }
        }
      }
    }

    if (ARROW1 == true) {
      if (arrow1 == 0) {
        if (BME280 == true) {
          if (temp.toFloat() > lowlevltemp) { pixels.setPixelColor(1, pixels.Color(255, 0, 0)); }
        }
      } else if (arrow1 == 3) {
        if (Z19B == true) {
          if (Co2.toFloat() > lowlevlCO2) { pixels.setPixelColor(1, pixels.Color(255, 0, 0)); }
        }
      } else if (arrow1 == 4) {
        if (PMS5003 == true) {
          if (val2.toFloat() > lowlevlPM25) { pixels.setPixelColor(1, pixels.Color(255, 0, 0)); }
        }
      } else if (arrow1 == 5) {
        if (SGP30 == true) {
          if (tvoc.toFloat() > lowlevlTVOC) { pixels.setPixelColor(1, pixels.Color(255, 0, 0)); }
        }
      } else if (arrow1 == 6) {
        if (MQ7i == true) {
          if (CO.toFloat() > lowlevlCO) { pixels.setPixelColor(1, pixels.Color(255, 0, 0)); }
        }
      }
    }
    
    if (ARROW2 == true) {
      if (arrow2 == 0) {
        if (BME280 == true) {
          if (temp.toFloat() > lowlevltemp) { pixels.setPixelColor(2, pixels.Color(255, 0, 0)); }
        }
      } else if (arrow2 == 3) {
        if (Z19B == true) {
          if (Co2.toFloat() > lowlevlCO2) { pixels.setPixelColor(2, pixels.Color(255, 0, 0)); }
        }
      } else if (arrow2 == 4) {
        if (PMS5003 == true) {
          if (val2.toFloat() > lowlevlPM25) { pixels.setPixelColor(2, pixels.Color(255, 0, 0)); }
        }
      } else if (arrow2 == 5) {
        if (SGP30 == true) {
          if (tvoc.toFloat() > lowlevlTVOC) { pixels.setPixelColor(2, pixels.Color(255, 0, 0)); }
        }
      } else if (arrow2 == 6) {
        if (MQ7i == true) {
          if (CO.toFloat() > lowlevlCO) { pixels.setPixelColor(2, pixels.Color(255, 0, 0)); }
        }
      }
    }

    if (ARROW3 == true) {
      if (arrow3 == 0) {
        if (BME280 == true) {
          if (temp.toFloat() > lowlevltemp) { pixels.setPixelColor(3, pixels.Color(255, 0, 0)); }
        }
      } else if (arrow3 == 3) {
        if (Z19B == true) {
          if (Co2.toFloat() > lowlevlCO2) { pixels.setPixelColor(3, pixels.Color(255, 0, 0)); }
        }
      } else if (arrow3 == 4) {
        if (PMS5003 == true) {
          if (val2.toFloat() > lowlevlPM25) { pixels.setPixelColor(3, pixels.Color(255, 0, 0)); }
        }
      } else if (arrow3 == 5) {
        if (SGP30 == true) {
          if (tvoc.toFloat() > lowlevlTVOC) { pixels.setPixelColor(3, pixels.Color(255, 0, 0)); }
        }
      } else if (arrow3 == 6) {
        if (MQ7i == true) {
          if (CO.toFloat() > lowlevlCO) { pixels.setPixelColor(3, pixels.Color(255, 0, 0)); }
        }
      }
    }

    pixels.show();

  } else {
    for (int i = 0; i < NUM_LEDS; i++) {
      pixels.setPixelColor(i, pixels.Color(0, 0, 0));  // выключить все диоды
    }
    pixels.show();
  }
}
