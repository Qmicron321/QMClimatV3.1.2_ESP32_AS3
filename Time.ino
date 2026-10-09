// #define daylightOffset_sec 3600
String NtpServerNames[] = {"pool.ntp.org", "time.nist.gov"};

time_t getNtpTime() {
  if (WiFi.status() != WL_CONNECTED) return 0;

  // Получаем UTC-время от NTP
  configTime(0, 0, NtpServerNames[0].c_str(), NtpServerNames[1].c_str());

  time_t t = time(NULL);
  uint8_t attempts = 0;
  unsigned long prevMillis = millis();

  while (true) {
    if (t > 1000000000UL) break; // валидное время в секундах (примерно > 2001-09-09)
    if (attempts >= 10) break;

    if (millis() - prevMillis >= 500UL) {
      t = time(NULL);
      attempts++;
      prevMillis += 500UL;
    }
    yield();
  }

  if (t > 1000000000UL) {
    // Применяем фиксированное смещение (без DST)
    return t + (time_t)timezone * 3600;
  }
  return 0;
}

void timeSync() {
  time_t gotTime;
  if (WiFi.status() == WL_CONNECTED) { 
      gotTime = getNtpTime();
      if (gotTime<=0) {
        statusUpdateNtpTime = 0;        
      } else {
        setTime(gotTime);
      }
  }
  else statusUpdateNtpTime = 0;
}

String GetTime(bool s) { //s - show seconds
  if ((timeStatus() != timeNotSet) && (timeStatus() != timeNeedsSync)) {
    time_t tn = now();
    String Time;
    if (s) {
      Time = String(hour(tn))+":"+ (minute(tn) < 10 ? "0"+String(minute(tn)) : String(minute(tn))) +":"+ String(second(tn));  
    } else {
      Time = String(hour(tn))+":"+ (minute(tn) < 10 ? "0"+String(minute(tn)) : String(minute(tn)));
    }
    return Time; 
  } else {
    timeSync();
    return "";
  }
}

void getTimeDisp(char *psz, bool f) { // Code for reading clock time for displey only
  time_t tn = now();
  sprintf(psz, "%02d%c%02d", hour(tn), (f ? ':' : ' '), minute(tn));
}

bool compTimeInt(float tFrom, float tTo, float tNow) { //Comparing time for proper processing from 18.00 to 8.00
  if (tFrom < tTo)  {
    if ((tFrom <= tNow) && (tTo >= tNow)) return true; else return false;
  } else if (tFrom > tTo) {
    if (tNow <= 23.59 && tFrom <= tNow)  return true;
    else if (tNow >= 0 && tTo >= tNow)  return true;
    else return false;  
  } else {
    return false;
  }
}



