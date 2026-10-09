
void init_sensor() {
  ///////////////////////////// BME280 /////////////////////////////
  if (BME280 == true) bme.begin(0x76);

  ////////////////////////////// TVOC ///////////////////////////////
  if (SGP30 == true) sgp.begin();  //(0x58);

  ///////////////////////////// PMS5003 /////////////////////////////
  if (PMS5003 == true) {
    Serial.begin(PMS::BAUD_RATE);
    DEBUG_OUT.begin(9600);

    pms.passiveMode();  // Switch to passive mode.
    pms.wakeUp();

#ifdef DEEP_SLEEP
    delay(PMS_READ_DELAY);
    readData();
    pms.sleep();
    ESP.deepSleep(PMS_READ_INTERVAL * 1000);
#endif  // DEEP_SLEEP
  }


  ///////////////////////////// MHZ19B /////////////////////////////
  if (Z19B == true) {
    pinMode(CO2power, OUTPUT);
    digitalWrite(CO2power, HIGH);  //пин инвертирован. Используется Р-тип транзистора (LOW - питание на датчик активировано HIGH - Питание сброшено)
    pinMode(CO2_IN, INPUT);        // или INPUT, если есть внешняя подтяжка
  }


  ///////////////////////////// MQ-135 /////////////////////////////
  if (MQ135i == true) {
    // MQ135.setRL(RL_MQ135);
    MQ135.setRegressionMethod(1);  //_PPM =  a*ratio^b
    MQ135.init();

#if USE_MQ_CAlibration == true
    /*****************************  MQ CAlibration ********************************************/
    // Разкоментировать для выполнения калибровки
    Serial.print("Calibrating please wait.");
    float calcR0 = 0;
    for (int i = 1; i <= 10; i++) {
      MQ135.update();  // Update data, the arduino will read the voltage from the analog pin
      calcR0 += MQ135.calibrate(RatioMQ135CleanAir);
      Serial.print(".");
    }
    MQ135.setR0(calcR0 / 10);
    Serial.println("  done!.");

    if (isinf(calcR0)) {Serial.println("Warning: Connection issue, R0 is infinite (Open circuit detected) please check your wiring and supply");while (1);}
    if (calcR0 == 0) {Serial.println("Warning: Connection issue found, R0 is zero (Analog pin shorts to ground) please check your wiring and supply");while (1);}
    /***************************************/
#else
MQ135.setR0(R0_MQ135);
#endif


    // Serial.println("** Values from MQ-135 ****");
    // Serial.println("|    CO    |    CO2    |");
    // MQ135.serialDebug(true);
  }


  ///////////////////////////// MQ-7 /////////////////////////////
  if (MQ7i == true) {
    // MQ7.setRL(RL_MQ7);
    MQ7.setRegressionMethod(1);  // _PPM =  a*ratio^b
    MQ7.init();

  #if USE_MQ_CAlibration == true
    /*****************************  MQ CAlibration ********************************************/
    // Разкоментировать для выполнения калибровки
    Serial.print("Calibrating please wait.");
    float calcR0 = 0;
    for (int i = 1; i <= 10; i++) {
      MQ7.update();  // Обновление данных
      calcR0 += MQ7.calibrate(RatioMQ7CleanAir);
      Serial.print(".");
    }
    MQ7.setR0(calcR0 / 10);
    Serial.println("  done!.");

    if (isinf(calcR0)) {Serial.println("Warning: Connection issue, R0 is infinite (Open circuit detected) please check your wiring and supply");while (1);}
    if (calcR0 == 0) { Serial.println("Warning: Connection issue found, R0 is zero (Analog pin shorts to ground) please check your wiring and supply");while (1);}
    /***************************************/
#else
MQ7.setR0(R0_MQ7);
#endif
    // MQ7.setR0(R0_MQ7);  // Раскоментировать строку полсе калибровки
    // Serial.println("** Values from MQ-7 ****");
    // Serial.println("|    CO    |");
    // MQ7.serialDebug(true);
  }

}  //init_sensor()

/****************************** BME280 ******************************/
void readBME() {
  if (millis() - ReedTimeBME > 10000) {
    ReedTimeBME = millis();

    temp = bme.readTemperature();
    hum = bme.readHumidity();
    pres = bme.readPressure() / 133.3F;  // (*0.0075F) или (/ 133.3F)
    alt = bme.readAltitude(SEALEVELPRESSURE_HPA);
    // Serial.print("Temperature: "); Serial.println(temp);
    // Serial.print("Humidity: "); Serial.println(hum);
    // Serial.print("Pressure: "); Serial.println(pres);
    // Serial.print("Altitude: "); Serial.println(alt);
  }
}


/****************************** PMS5003 ******************************/
void pms5003() {
#ifndef DEEP_SLEEP
  static uint32_t timerLast = 0;
  uint32_t timerNow = millis();
  if (timerNow - timerLast >= timerInterval) {
    timerLast = timerNow;
    timerCallback();
    timerInterval = timerInterval == PMS_READ_DELAY ? PMS_READ_INTERVAL : PMS_READ_DELAY;
  }
#endif  // DEEP_SLEEP

  if (pms.read(data)) {
    val1 = data.PM_AE_UG_1_0;
    val2 = data.PM_AE_UG_2_5;
    val3 = data.PM_AE_UG_10_0;
  }
}

#ifndef DEEP_SLEEP
void timerCallback() {
  if (timerInterval == PMS_READ_DELAY) {
    readData();
    DEBUG_OUT.println("Going to sleep.");
    pms.sleep();
  } else {
    DEBUG_OUT.println("Waking up.");
    pms.wakeUp();
  }
}
#endif  // DEEP_SLEEP

void readData() {
  // Clear buffer (removes potentially old data) before read. Some data could have been also sent before switching to passive mode.
  while (Serial.available()) {
    Serial.read();
  }

  DEBUG_OUT.println("Send read request...");
  pms.requestRead();

  DEBUG_OUT.println("Reading data...");
  if (pms.readUntil(data)) {
    //   DEBUG_OUT.println();

    DEBUG_OUT.print("PM 1.0 (ug/m3): ");
    DEBUG_OUT.println(data.PM_AE_UG_1_0);

    DEBUG_OUT.print("PM 2.5 (ug/m3): ");
    DEBUG_OUT.println(data.PM_AE_UG_2_5);

    DEBUG_OUT.print("PM 10.0 (ug/m3): ");
    DEBUG_OUT.println(data.PM_AE_UG_10_0);

    //   DEBUG_OUT.println();
  } else {
    DEBUG_OUT.println("No data.");
  }
}


/****************************** MHZ19B ******************************/
void mhz19b() {
  unsigned long now = millis();

  if (now - lastCycleTime >= cycleInterval) {
    lastCycleTime = now;

    digitalWrite(CO2power, LOW);  // Включение питания для прогрева
    Serial.println("preHeating Up.");
    waitFor(warmUpTime);  // ожидание прогрева
  yield();
    // Производим 10 замеров
    long totalCO2 = 0;
    for (uint8_t i = 0; i < 10; i++) {
      int CO2_pwm = co2.readCO2PWM();
      if (CO2_pwm < 0) {
        Serial.println("Erro reading");
        break;
      } else {
        totalCO2 += CO2_pwm;
      }
      waitFor(readDelay);
    }

    Co2 = totalCO2 / 10;
    Serial.print("CO2: ");
    Serial.println(Co2);

    digitalWrite(CO2power, HIGH);  // Выключение датчика
                                   //    Serial.println("Sleep");
  }
}

void waitFor(uint32_t duration) {
  unsigned long startTime = millis();
  while (millis() - startTime < duration) {
    // yield();
  }
}


/****************************** SGP30 ******************************/
void sgp30() {
  if (millis() - ReedTimeTVOC > 2000) {
    ReedTimeTVOC = millis();
    printtvoc();  // Вывод TVOC

    sgp.IAQmeasure();
    eco2 = sgp.eCO2;
    //Serial.print("eco2 "); Serial.println(sgp.eCO2);
    //tvoc = sgp.TVOC;
    //Serial.print("tvoc: "); Serial.println(tvoc);

    /********** Вычисление среднего значения **********/
    long tvocSum = 0;               // локальная переменная
    for (int i = 0; i < 50; i++) {  // согласно количеству усреднений
      sgp.IAQmeasure();             // Обновляем значения перед счётом
      tvocSum += sgp.TVOC;          // суммируем значения с любого датчика в переменную
    }
    tvoc = (float)tvocSum / 50;  // находим среднее арифметическое, разделив сумму на число измерений
                                 //Serial.print("tvoc: "); Serial.println(tvoc);              // для проверки выводим в порт

    sgp.IAQmeasureRaw();
    h2 = sgp.rawH2;
    //Serial.print("Raw Hydrogen(H2) ");Serial.print(sgp.rawH2); Serial.print(" \t");  //Водородо содержащие соединения
    eth = sgp.rawEthanol;
    //Serial.print("Raw Ethanol ");Serial.print(sgp.rawEthanol); Serial.println("");  //Спирты
  }
}

void printtvoc() {
  /********** Температурныe данныe для компенсации *********/
  //#if USE_BME280 == true
  //float tempSGP = bme.readTemperature(); // [°C]
  //float humSGP = bme.readHumidity(); // [%RH]
  //#else
  float tempSGP = 22.1;  // [°C]
  float humSGP = 45.2;   // [%RH]
  //#endif

  sgp.setHumidity(getAbsoluteHumidity(tempSGP, humSGP));
  Absolutehumidity = getAbsoluteHumidity(tempSGP, humSGP);
}

/****************************** MQ135 ******************************/
void mq135() {
  // Обновление данных каждые 20 секунд
  if (millis() - ReedTimeGas > 30000) {
    ReedTimeSmoke = millis();

    MQ135.update();              // Update data, the arduino will read the voltage from the analog pin
    float correctionFactor = 0;  // Optional environmental correction

    MQ135.setA(605.18);
    MQ135.setB(-3.937);                              // Configure the equation to calculate CO concentration value
    Co = MQ135.readSensor(false, correctionFactor);  // Sensor will read PPM concentration using the model, a and b values set previously or from the setup

    MQ135.setA(110.47);
    MQ135.setB(-2.862);                               // Configure the equation to calculate CO2 concentration value
    CO2 = MQ135.readSensor(false, correctionFactor)+400;  // Sensor will read PPM concentration using the model, a and b values set previously or from the setup

    // Serial.print("|   ");
    // Serial.print(Co);
    // Serial.print("   |   ");
    // Serial.print(CO2);
    // Serial.println("   |");

    // MQ135.serialDebug();                        // Печать результатов в последовательный порт
/*
    Exponential regression:
  GAS      | a      | b
  CO       | 605.18 | -3.937  
  Alcohol  | 77.255 | -3.18 
  CO2      | 110.47 | -2.862
  Toluen  | 44.947 | -3.445
  NH4      | 102.2  | -2.473
  Aceton  | 34.668 | -3.369
  */
  }
}


/****************************** MQ7 ******************************/
void mq7() {
  // Чтение данных каждые 30 секунд
  if (millis() - ReedTimeSmoke > 30000) {
    ReedTimeSmoke = millis();

    MQ7.update();                // Обновление данных
    float correctionFactor = 0;  // Опциональная корректировка окружающей среды
    MQ7.setA(99.042);
    MQ7.setB(-1.518);                              // Конфигурация уравнения для расчета концентрации CO
    CO = MQ7.readSensor(false, correctionFactor);  // Чтение концентрации газа

    // Serial.print("|   ");
    // Serial.print(CO);
    // Serial.println("   |");

    // MQ7.serialDebug();                        // Печать результатов в последовательный порт
  }
}
