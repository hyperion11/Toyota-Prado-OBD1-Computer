////////////////////////////////////////////////////////////////////////
/////SETTINGS //////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
void showcurrentsetting() {
  tft.fillScreen(BG_COLOR);
  tft.setTextColor(TEXT_COLOR);
  tft.setFont(&DSEG7ModernMini_Bold25pt7b );
  switch (work_id) {
    case 0: {
        printpos(tm.hour, 125, 110);
        tft.setFont(&CPMono_v07_Plain9pt7b);
        printpos(F("Hour"), 120, 145);
      } break;
    case 1: {
        printpos(tm.minute, 125, 110);
        tft.setFont(&CPMono_v07_Plain9pt7b);
        printpos(F("Minute"), 120, 145);
      } break;
    case 2: {
        printpos(tm.day, 125, 110);
        tft.setFont(&CPMono_v07_Plain9pt7b);
        printpos(F("Day"), 120, 145);
      } break;
    case 3: {
        printpos(tm.month, 125, 110);
        tft.setFont(&CPMono_v07_Plain9pt7b);
        printpos(F("Month"), 120, 145);
      } break;
    case 4: {
        printpos(tm.year + 1970, 125, 110);
        tft.setFont(&CPMono_v07_Plain9pt7b);
        printpos(F("Year"), 120, 145);
      } break;
    case 5: {
        tft.setFont(&CPMono_v07_Plain9pt7b);
        printpos(colorscheme[color_scheme], 125, 110);
        printpos(F("Color Scheme"), 120, 145);
      } break;
    case 6: {
        tft.setFont(&CPMono_v07_Plain9pt7b);
        printpos(F("Injector Calibration"), 45, 30);
        printpos(F("Trip fuel"), 30, 70);
        printpos(trip_inj_dur  * Ls / 1000.0 , 2, 150, 70);
        printpos(F("Inj flow"), 30, 110);
        tft.setCursor(150, 110);
        tft.print(Ls, 7);
        printpos(F("Adjust Inj Flow to match"), 25, 140);
        printpos(F("calculated fuel consumption"), 5, 160);
        printpos(F("to real fuel consumption"), 5, 180);
        //printpos(Ls, 150, 110);

      } break;
    case 7: {
        printpos(alarms, 125, 110);
        tft.setFont(&CPMono_v07_Plain9pt7b);
        printpos(F("Alarms"), 120, 145);
      } break;
    case 8: {
        tft.setFont(&CPMono_v07_Plain9pt7b);
        printpos(F("RTC RTCCR SETTINGS"), 45, 30);
        printpos(F("IN FRAM:"), 30, 70);
        printpos(RTCCR, 150, 70);
        printpos(F("IN BKP:"), 30, 110);
        printpos(getrtccr(), 150, 110);
      } break;
    case 9: {
        tft.setFont(&CPMono_v07_Plain9pt7b);
        printpos(F("RTC DRIFT SETTINGS"), 45, 30);
        printpos(F("IN FRAM:"), 30, 70);
        printpos(DRIFT, 150, 70);
        printpos(F("IN BKP:"), 30, 110);
        printpos(getdrift(), 150, 110);
      } break;
  }
}
/////////////////////////////////////////////////////////////////////////
void settings(void) {
  if (isActive == false)
  {
    if (tm.year < 48) tm.year = 48; //ранее 2018-1970=48 дата быть не может
    tft.fillScreen(BG_COLOR);
    work_id = 0;
    //TimeSync();
    rt.breakTime(rt.now(), tm);
    isActive = true;
    showcurrentsetting();
    return;
  }
  switch (_id) {
    case 1: {
        //TimeSync();
        rt.breakTime(rt.now(), tm);
        if (tm.hour > 0 && work_id == 0) tm.hour--;   //часы минус
        if (tm.minute > 0 && work_id == 1) tm.minute--;   //минуты минус
        if (tm.day > 1 && work_id == 2) tm.day--;   //дни месяца минус
        if (tm.month > 1 && work_id == 3) tm.month--;   //месяц минус
        if (tm.year > 48 && work_id == 4) tm.year--;   //год минус  //меньше 2018 в принципе не нужно 2018-1970=48
        if (color_scheme > 0 && work_id == 5) {
          color_scheme--;  //цветовая схема минус
          FRAM.write(40, color_scheme);
        }
        if (Ls > 0.002 && work_id == 6) {
          Ls -= 0.000010;  //калибровка форсунок минус
          FRAM.write(36, Ls);
        }
        if (alarms == true && work_id == 7) {
          alarms = false; //Отключение алармов
          FRAM.write(49, alarms);
        }
        if (work_id == 8 && RTCCR > 0)
        {
          RTCCR--;
        }
        if (work_id == 9 && DRIFT > -32767)
        {
          DRIFT--;
        }
        rt.setTime(tm);
        rt.breakTime(rt.now(), tm);
        showcurrentsetting();
      } break;
    case 2: {
        rt.breakTime(rt.now(), tm);
        if (tm.hour < 23 && work_id == 0) tm.hour++;    //часы плюс
        if (tm.minute < 59 && work_id == 1) tm.minute++;    //минуты плюс
        if (tm.day < 31 && work_id == 2) tm.day++;    //дни месяца плюс
        if (tm.month < 11 && work_id == 3) tm.month++;   //месяц плюс
        if (tm.year < 90 && work_id == 4) tm.year++;    //год плюс. ну и я пессимист :) 2060-1970=90
        if (color_scheme < 1 && work_id == 5) {   // пока что только две цветовые схемы
          color_scheme++;  //цветовая схема плюс
          FRAM.write(40, color_scheme);
        }
        if (Ls < 0.006 && work_id == 6) {
          Ls += 0.000010;  //калибровка форсунок плюс
          FRAM.write(36, Ls);
        }
        if (alarms == false && work_id == 7) {
          alarms = true; //включение алармов
          FRAM.write(49, alarms);
        }
        if (work_id == 8 && RTCCR < 126)
          RTCCR++; //127 max
        if (work_id == 9 )
          DRIFT++;
        rt.setTime(tm);
        rt.breakTime(rt.now(), tm);
        showcurrentsetting();
      } break;
    case 3: {
        if (work_id > 0) work_id--;
        rt.breakTime(rt.now(), tm);
        showcurrentsetting();
      } break;
    case 4: {
        if (work_id < 9) work_id++;
        rt.breakTime(rt.now(), tm);
        showcurrentsetting();
      } break;
  }
  _id = 10;  //чтобы в новом цикле ничего само не выполнялось
  // delay(100) ;
}
