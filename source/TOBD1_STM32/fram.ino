void read_fram_data() {
  int check = 0;
  FRAM.read(40, color_scheme);         //Читается какая будет цветовая схема uint8_t
  if (color_scheme == 0) { //Зеленая схема
    TEXT_COLOR = ILI9341_GREEN;
    BG_COLOR = ILI9341_BLACK;
    BLUE_COLOR = ILI9341_BLUE;
    RED_COLOR = ILI9341_RED;
    GT_COLOR = ILI9341_GREEN;
  } else if (color_scheme == 1) { //ЧБ схема
    TEXT_COLOR = ILI9341_WHITE;
    BG_COLOR = ILI9341_BLACK;
    BLUE_COLOR = ILI9341_WHITE;
    RED_COLOR = ILI9341_WHITE;
    GT_COLOR = ILI9341_GREY;
  } else
  {
    color_scheme = 0;           //если чтение прошло с ошибкой то зеленая схема
    TEXT_COLOR = ILI9341_GREEN;
    BG_COLOR = ILI9341_BLACK;
    BLUE_COLOR = ILI9341_BLUE;
    RED_COLOR = ILI9341_RED;
    GT_COLOR = ILI9341_GREEN;
    FRAM.write(40, 0);
  }
  tft.setTextColor(TEXT_COLOR);
  tft.fillScreen(BG_COLOR);
  tft.setFont(&CPMono_v07_Plain9pt7b );
  tft.setCursor(0, 20);
  // Читается значение в 100й ячейке для определения работоспособности памяти.
  FRAM.read(100, check);
  tft_string("   Diagnostic...");
  tft_string("   Checking FRAM");
  if (check == 1986)    //если значение совпадает с нужным то считается что память работает нормально
  {
    tft.println(check);
    tft_string ("   Fram work good");
    _fram = true;
  } else                //в противном случае считается что память не работает
  {
    tft_string ("   Current test data");
    tft.println(check);
    FRAM.write(100, int(1986));       //Попытка заапись тестового значения в случае чистой памяти
    tft_string ("   Attempt write test data");
    FRAM.read(100, check);
    if (check == 1986) {
      tft.println(check);
      tft_string ("   Fram work good");
      _fram = true;
      delay (1000);
      // все ок, память рабочая, просто была чистая
    }
    else {
      tft_string ("   Check FRAM");
      tft_string ("   or 5v stab");
#if defined(DEBUG_SERIAL_OUTPUT)
      Serial.println("Fram not work");
#endif
      _fram = false; //отключаю запись в память если возник какой то косяк
      delay (5000);
    }
  }

  FRAM.read(104, check);
  if (check != 1234567 &&  _fram == true )       //если в 104 ячейки нету определенного значения, но в целом фрам рабочая, то предполагается что первый запуск БК. Инициализация базывых настроек
  {
    tft.fillScreen(BG_COLOR);
    tft.setTextColor(TEXT_COLOR);
    tft.setFont(&CPMono_v07_Plain9pt7b);
    printpos(F("CLEAN FRAM DETECTED!"), 10, 35);
    printpos(F("RECORDING DEFAULT DATA!"), 10, 55);
    FRAM.write(104, int(1234567));
#if defined(VZ)
    FRAM.write(36, 0.004020653f);     //Производительность форсунок FLOAT 5VZ-FE литры в час 240сс / 60 / 1000
#elif defined(RZ)
    FRAM.write(36, 0.004966667f);   //Производительность форсунок FLOAT 3RZ-FE литры в час 298сс /60 /1000
#else
    FRAM.write(36, 0.00f);   //
#endif
    FRAM.write(40, uint8_t(0) );      //цветовая схема    uint8_t
    FRAM.write(49, uint8_t(0));        //Включение алармов
    FRAM.write(50, uint16_t(3000));
    FRAM.write(52, uint8_t(0));   //дрифт RTC
    FRAM.write(53, int16_t(0));
    FRAM.write(55, uint32_t(0));
#if defined(DEBUG_SERIAL_OUTPUT)
    Serial.println("Default values was written");
#endif
    delay (5000);
  }
  FRAM.read(0, trip_km);        //Пробег. Обнуляется кнопкой на главном экране FLOAT
  FRAM.read(4, trip_time);      //Время поездки. Обнуляется кнопкой на главном экране uint32_t
  FRAM.read(8, total_km);       //Пробег. Обнуляется кнопкой на третьем экране FLOAT
  FRAM.read(12, total_inj_dur); //Длительность впрыска. Обнуляется кнопкой на третьем экране FLOAT
  FRAM.read(16, trip_inj_dur);  //длительность впрыска. Обнуляется кнопкой на главном экране FLOAT
  FRAM.read(20, total_time);    //Время работы двигателя. Обнуляется кнопкой на третьем экране uint32_t
  FRAM.read(24, time_RO);       //Время работы двигателя. Не обнуляется никак uint32_t
  FRAM.read(28, km_RO);         //Пробег. Не обнуляется никак FLOAT
  FRAM.read(32, inj_dur_RO);    //Длительность впрыска. Не обнуляется никак FLOAT
  FRAM.read(36, Ls);            //Производительность форсунок FLOAT
  FRAM.read(41, start_count);   //ко-во стартов БК  uint16_t
  FRAM.read(43, write_count);   //кол-во записей в FRAM uint16_t
  FRAM.read(45, fuel.remains);   //остаток топлива в баке. float
  FRAM.read(49, alarms);         //Включенные ли алармы 0-нет 1-да boolean
  FRAM.read (52, RTCCR);         
  FRAM.read (53, DRIFT);
  FRAM.read (55, BKPTIME);
  Serial.print("RTCCR in fram: ");
  Serial.println(RTCCR);
  Serial.print("RTCCR in bkp: ");
  Serial.println(getrtccr());
  /*
  if (RTCCR != getrtccr() && RTCCR >= 0 && RTCCR <= 127)   //если во FRAM хранится калибровка отличная от той что хранится к BKP регистре то считаем что была замена батарейки
    setrtccr(RTCCR);      //и соответсвенно надо восстановить эту настройку.
  Serial.print("DRIFT in fram: ");
  Serial.println(DRIFT);
  Serial.print("DRIFT in bkp: ");
  Serial.println(getdrift());
  if (DRIFT != getdrift() && DRIFT > -32768 && DRIFT < DRIFT_MAX)  //если во FRAM хранится калибровка отличная от той что хранится к BKP регистре то считаем что была замена батарейки
    setbkpdrift(DRIFT);      //и соответсвенно надо восстановить эту настройку.
    */
#if defined(DEBUG_SERIAL_OUTPUT)
  Serial.println("Fram read complete");
#endif

}

void erasefram() {
  uint8_t testread;
  uint16_t testsum = 0;
  uint8_t i;
  tft.fillScreen(BG_COLOR);
  tft.setTextColor(TEXT_COLOR);
  tft.setFont(&CPMono_v07_Plain9pt7b);
  printpos(F("ERASING FRAM!"), 10, 35);
  printpos(F("WILL START IN 10s!"), 10, 55);
  printpos(F("IF ERASING IS MISTAKE!"), 10, 75);
  printpos(F("POWER OFF VEHICLE!"), 10, 95);
  delay (10000);
  for ( i = 0; i < 255; i++)
  {
    Serial.println (i);
    FRAM.write (i, uint8_t(0));
  }
  for (uint8_t i = 0; i < 255; i++)
  {
    Serial.print ("READING: ");
    FRAM.read (i, testread);
    Serial.println (testread);
    testsum += testread;
  }
  if (testsum == 0) {
    tft.fillScreen(BG_COLOR);
    printpos(F("ERASING FRAM COMPLETE!"), 10, 35);
    printpos(F("HAVE A NICE DAY!"), 10, 55);
#if defined(DEBUG_SERIAL_OUTPUT)
    Serial.println("Erasing complete");
#endif

    delay (2000);
    read_fram_data();
  }
  else
  {
    tft.fillScreen(BG_COLOR);
    printpos(F("ERASING FRAM FAILED!"), 10, 35);
    printpos(F("IT IS A GOOD DAY TO DIE!"), 10, 55);
#if defined(DEBUG_SERIAL_OUTPUT)
    Serial.println("Erasing failed");
#endif
    delay (2000);
  }
}

void cleardata() {
  FRAM.write(0, 0.0f);
  FRAM.write(4, 0l);
  FRAM.write(16, 0.0f);
  if (CurrentDisplayIDX == 3) {
    FRAM.write(8, 0.0f);
    FRAM.write(12, 0.0f);
    FRAM.write(20, 0l);
  }
  delay(10);
  FRAM.read(0, trip_km);            //километраж поездки. Обнуляется кнопкой при активном 2м экране
  FRAM.read(4, trip_time);          //Время поездки. Обнуляется кнопкой при активном 2м экране
  FRAM.read(8, total_km);           //полный пробег. Обнуляется кнопкой при активном 3м экране
  FRAM.read(12, total_inj_dur);     //полная длительность впрыска. Обнуляется кнопкой при активном 3м экране
  FRAM.read(16, trip_inj_dur);      //длительность впрыска. Обнуляется кнопкой. Обнуляется кнопкой при активном 2м экране
  FRAM.read(20, total_time);        //полное время работы двигателя. Обнуляется кнопкой при активном 3м экране
}


void SaveToFram() {
  uint32_t check;
  if (_fram) //если FRAM инициализирована нормально
  {
    write_count++;
    Serial.println("SAVE FRAM");
    FRAM.write(0, trip_km);
    FRAM.write(4, trip_time);
    FRAM.read(4, check);                              //примитивная проверка корректности записи. Мало ли
    if (check != trip_time) {
      tft.fillScreen(BG_COLOR);
      tft.setFont(&DSEG7ModernMini_Bold13pt7b );
      tft.setCursor(16, 120);
      _fram = false;
      tft.print(F("FRAM ERROR!!"));
    }
    FRAM.write(8, total_km);
    FRAM.write(12, total_inj_dur);
    FRAM.write(16, trip_inj_dur);
    FRAM.write(20, total_time);
    FRAM.write(24, time_RO);       //Время работы двигателя. Не обнуляется никак
    FRAM.write(28, km_RO);         //Пробег. Не обнуляется никак
    FRAM.write(32, inj_dur_RO);    //Длительность впрыска. Не обнуляется никак
    FRAM.write(43, write_count);   //кол-во записей в FRAM uint16_t
    FRAM.write(45, fuel.remains);   //остаток топлива в баке. float
    FRAM.read(20, check);
    if (check != total_time) {
      tft.fillScreen(BG_COLOR);
      tft.setFont(&DSEG7ModernMini_Bold13pt7b );
      tft.setCursor(16, 120);
      _fram = false;
      tft.print(F("FRAM ERROR!!"));
    }
    flagNulSpeed = true;                                  //запрет повторной записи
    last_log_time = millis();                             //чтобы не писать лишний раз
    FramWriteIndicator();
  }
}
