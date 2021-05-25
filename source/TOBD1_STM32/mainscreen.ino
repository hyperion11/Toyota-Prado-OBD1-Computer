void mainscreen(void) {
  tft.setTextColor(TEXT_COLOR);
  if (isActive == false) {                      //если первый раз экран появился то рисуются подписи
    tft.fillScreen(BG_COLOR);
    showtime();
    tft.setFont(&CPMono_v07_Light6pt7b );
    printpos(ADC.vcc_avg * VREF_MEASURED * VCC_PIN_DIVIDER / 4095.0, 1, 20, 30);
    printpos(F("TRIP FUEL"), 16, 90);
    printpos(F("COOLANT TEMP"), 16, 146);
    printpos(F("INSTANT FUEL"), 196, 90);
    printpos(F("AVERAGE TRIP"), 196, 146);
    printpos(F("TRIP"), 48, 209);
    tft.setFont(&CPMono_v07_Plain9pt7b );
    printpos(F("L"), 117, 120);
    printpos(F("C"), 117, 182);
    tft.drawCircle(111, 170, 3, TEXT_COLOR);
    printpos(F("LPK"), 270, 182);
    if (OBDDATA[OBD_SPD] > 1)       //если есть движение то литры на 100км
      printpos(F("LPK"), 270, 120);
    if (OBDDATA[OBD_SPD] < 1)   //если стоим то литры в час
      printpos(F("LPH"), 270, 120);
    tft.setFont(&DSEG7ModernMini_Bold13pt7b );
    printpos(trip_fuel_consumption, 1, 16, 120);
    if (OBDDATA[OBD_ECT] < 50)
      tft.setTextColor(BLUE_COLOR);
    else if (OBDDATA[OBD_ECT] < 99) tft.setTextColor(TEXT_COLOR);
    else tft.setTextColor(RED_COLOR);
    printpos(OBDDATA[OBD_ECT], 1, 16, 182);
    tft.setTextColor(TEXT_COLOR);
    if (OBDDATA[OBD_SPD] > 1)        //если есть движение то литры на 100км
      printpos(maindata[0], 1, 188, 120);
    else printpos(maindata[1], 1, 188, 120); //иначе литры в час
    if (trip_avg_fuel_consumption < 100)
      printpos(trip_avg_fuel_consumption, 1, 188, 182);
    else
      printpos(F("99.9"), 188, 182);
    tft.drawRoundRect(8, 75, 128, 55, 12, RED_COLOR);
    tft.drawRoundRect(8, 134, 128, 55, 12, RED_COLOR);
    tft.drawRoundRect(184, 75, 130, 55, 12, RED_COLOR);
    tft.drawRoundRect(184, 134, 130, 55, 12, RED_COLOR);
    if (OBDDATA[OBD_SPD] < 1) {
      neutral();
    } else {
      show_current_gear(); //принудительная перерисовка
    }
    isActive = true;
  }// если экран не меняется то подписи не обновляются
  // if (OBDDATA[OBD_SPD] < 1) {
  //   neutral();
  // } else
  // { //только при движении
  //  show_current_gear(false); //только при изменении передачи
  //}
  //TT.last = TT.current;
  maindata[0] = 100  * (OBDDATA[OBD_INJ] * OBDDATA[OBD_RPM] / OBDDATA[OBD_SPD] * Ls / 1000 / 2 * 60 * Ncyl);  //LPK
  if (maindata[0] > 100.0f) maindata[0] = 99.9;                       //если больше 100 то выводить 99.9
  maindata[1] = OBDDATA[OBD_INJ] * OBDDATA[OBD_RPM] * Ls / 1000 / 2 * 60 * Ncyl;    //LPH
  if (maindata[1] > 100.0f) maindata[1] = 99.9;                       //если больше 100 то выводить 99.9
  maindata[2] = trip_avg_fuel_consumption;
  if (maindata[2] > 100.0f) maindata[2] = 99.9;                       //если больше 100 то выводить 99.9
  maindata[3] = trip_fuel_consumption;
  if (maindata[3] > 100.0f) maindata[3] = 99.9;                       //если больше 100 то выводить 99.9
  maindata[4] = ADC.vcc_avg * VREF_MEASURED * VCC_PIN_DIVIDER / 4095.0;
  tft.setTextColor(TEXT_COLOR);
  if (fabs(maindata[4] - maindataold[4]) > 0.09)
  {
    tft.setFont(&CPMono_v07_Light6pt7b );
    tft.fillRect(20, 0, 40, 30, BG_COLOR);
    printpos(maindata[4], 1, 20, 30);
    maindataold[4] = maindata[4];
  }
  tft.setFont(&CPMono_v07_Light6pt7b );
  if (flag.isTrip) {
    if (!flag.TripActive) {
      tft.setFont(&CPMono_v07_Light6pt7b );
      tft.fillRect(16, 82, 100, 12, BG_COLOR);
      printpos(F("TRIP FUEL"), 16, 90);
      tft.fillRect(231, 196, 40, 19, BG_COLOR);
      printpos(F("KM"), 234, 209);
      flag.TripActive = true;
      tft.setFont(&DSEG7ModernMini_Bold13pt7b );
      tft.fillRect(16, 95, 100, 27, BG_COLOR);
      printpos(trip_fuel_consumption, 1, 16, 120);
      tft.fillRect(85, 196, 145, 37, BG_COLOR);
      printpos(F("   "), 90, 225);
      if (trip_km < 10)
        printpos(F("000"), 90, 225);
      else if (trip_km < 100)
        printpos(F("00"), 90, 225);
      else if (trip_km < 1000)
        printpos(F("0"), 90, 225);
      tft.print(trip_km, 1);
    }
    if (fabs(maindata[3] - maindataold[3]) > 0.09)
    {
      tft.setFont(&DSEG7ModernMini_Bold13pt7b );
      tft.fillRect(16, 95, 90, 27, BG_COLOR);
      printpos(maindata[3], 1, 16, 120);
      maindataold[3] = maindata[3];
    }
  }
  if (!flag.isTrip) {
    if (!flag.TripActive) {
      tft.setFont(&CPMono_v07_Light6pt7b );
      tft.fillRect(16, 82, 100, 12, BG_COLOR);
      printpos(F("FUEL REMAINS"), 16, 90);
      tft.fillRect(231, 196, 40, 19, BG_COLOR);
      printpos(F("TIME"), 234, 209);
      tft.setFont(&DSEG7ModernMini_Bold13pt7b );
      tft.fillRect(16, 95, 100, 27, BG_COLOR);
      printpos(fuel.remains, 1, 16, 120);
      tft.fillRect(85, 196, 145, 37, BG_COLOR);
      printpos(TimeToString(trip_time), 90, 225);
      flag.TripActive = true;
    }
    tft.setFont(&DSEG7ModernMini_Bold13pt7b );
    tft.fillRect(16, 95, 90, 27, BG_COLOR);
    printpos(fuel.remains, 1, 16, 120);
  }
  if (OBDDATA[OBD_ECT] < 50)
    tft.setTextColor(BLUE_COLOR);
  else if (OBDDATA[OBD_ECT] < 99) tft.setTextColor(TEXT_COLOR);
  else tft.setTextColor(RED_COLOR);

  if (fabs(OBDDATA[OBD_ECT] - OLDOBDDATA[OBD_ECT]) > 0.09)
  {
    tft.setFont(&DSEG7ModernMini_Bold13pt7b );
    tft.fillRect(16, 157, 90, 27, BG_COLOR);
    printpos(OBDDATA[OBD_ECT], 1, 16, 182);
  }
  tft.setTextColor(TEXT_COLOR);
  if (OBDDATA[OBD_SPD] > 1)       //если есть движение то литры на 100км
  {
    if (_isLPK == false)          //смена экрана  - перерисовка надписи и значений
    {
      tft.setFont(&CPMono_v07_Plain9pt7b );
      send_tft2(F("LPK"), 12, 40, 270, 120);
      tft.setFont(&DSEG7ModernMini_Bold13pt7b );
      tft.fillRect(188, 95, 82, 27, BG_COLOR);
      printpos(maindata[0], 1, 188, 120);
      _isLPK = true;
    }
    if (fabs(maindata[0] - maindataold[0]) > 0.09)  //перерисовка значений только если отличаются на 0.1
    {
      tft.setFont(&DSEG7ModernMini_Bold13pt7b );
      tft.fillRect(188, 95, 82, 27, BG_COLOR);
      printpos(maindata[0], 1, 188, 120);
      maindataold[0] = maindata[0];
    }
  }
  if (OBDDATA[OBD_SPD] < 1)   //если стоим то литры в час
  {
    if (_isLPK == true)         //смена экрана  - перерисовка надписи и значений
    {
      tft.setFont(&CPMono_v07_Plain9pt7b );
      send_tft2(F("LPH"), 12, 40, 270, 120);
      tft.setFont(&DSEG7ModernMini_Bold13pt7b );
      tft.fillRect(188, 95, 82, 27, BG_COLOR);
      printpos(maindata[1], 1, 188, 120);
      _isLPK = false;
    }
    if (fabs(maindata[1] - maindataold[1]) > 0.09) //перерисовка значений только если отличаются на 0.1
    {
      tft.setFont(&DSEG7ModernMini_Bold13pt7b );
      tft.fillRect(188, 95, 82, 27, BG_COLOR);
      printpos(maindata[1], 1, 188, 120);
      maindataold[1] = maindata[1];
    }
  }
  if (fabs(maindata[2] - maindataold[2]) > 0.09)
  {
    tft.setFont(&DSEG7ModernMini_Bold13pt7b );
    tft.fillRect(188, 157, 82, 27, BG_COLOR);
    printpos(maindata[2], 1, 188, 182);
    maindataold[2] = maindata[2];
  }
  showtime();
  }
