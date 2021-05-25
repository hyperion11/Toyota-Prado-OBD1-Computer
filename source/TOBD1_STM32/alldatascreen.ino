void drawAllData(void) {
  uint8_t i;
  tft.setTextColor(TEXT_COLOR);
  tft.setFont(&CPMono_v07_Plain9pt7b);
  if (isActive == false) {                    //форсирую перерисовку  после переключения экрана
    tft.fillScreen(BG_COLOR);
    isActive = true;        // ставлю флаг активного экрана чтобы не перерисовывать лишнее
    for (i = 0; i < TOBD_ID_MAX; i++)
      printpos(TOBD_ID_NAME[i], 15, 18 * i + 36);
    printpos(OBDDATA[OBD_INJ], 2, 69, 36);
    printpos(OBDDATA[OBD_IGN], 0, 69, 54);
    printpos(OBDDATA[OBD_IAC], 1, 69, 72);
    printpos(OBDDATA[OBD_RPM], 0, 69, 90);
    printpos(OBDDATA[OBD_MAP], 2, 69, 108);
    printpos(OBDDATA[OBD_ECT], 0, 69, 126);
    printpos(OBDDATA[OBD_TPS], 0, 69, 144);
    printpos(OBDDATA[OBD_SPD], 0, 69, 162);
    printpos(OBDDATA[OBD_OXSENS], 0, 69, 180);
    if (OBDFLAG[0] == true) {
      printpos(F("START"), 160, 36);
      printpos(F("ENRICH"), 160, 54);
    }
    if (OBDFLAG[1] == true)
      printpos(F("COLD"), 160, 72);
    if (OBDFLAG[2] == true)
      printpos(F("KNOCKING"), 160, 90);
    if (OBDFLAG[3] == true)
      printpos(F("CLOSED LOOP"), 160, 108);
    if (OBDFLAG[4] == true)  {
      printpos(F("ACCEL"), 160, 126);
      printpos(F("ENRICH"), 160, 144);
    }
    if (OBDFLAG[5] == true)
      printpos(F("STARTER"), 160, 162);
    if (OBDFLAG[6] == true)
      printpos(F("IDLE"), 160, 180);
    if (OBDFLAG[7] == true)
      printpos(F("AIR COND"), 160, 198);
    if (OBDFLAG[8] == true)
      printpos(F("NEUTRAL"), 160, 216);
    if (OBDFLAG[9] == true)  {
      tft.fillRect(160, 221, 160, 19, BG_COLOR);
      printpos(F("LEAN"), 160, 234);
    } else if (OBDFLAG[9] == false) {
      tft.fillRect(160, 221, 160, 19, BG_COLOR);
      printpos(F("RICH"), 160, 234);
    }
  }                        //конец перерисовки
  else
  { //тут данные обновляются только после их изменения
    update_tft_float(OBD_INJ, 69, 36);
    update_tft_int(OBD_IGN, 69, 54);
    update_tft_int(OBD_IAC, 69, 72);
    update_tft_int(OBD_RPM, 69, 90);
    update_tft_float(OBD_MAP, 69, 108);
    update_tft_int(OBD_ECT, 69, 126);
    update_tft_int(OBD_TPS, 69, 144);
    update_tft_int(OBD_SPD, 69, 162);
    update_tft_float(OBD_OXSENS, 69, 180, 0);
    if ((OBDFLAG[0] == true) && (OBDFLAG[0] != OLDOBDFLAG[0])) {
      printpos(F("START"), 160, 36);
      printpos(F("ENRICH"), 160, 54);
    }
    else if ((OBDFLAG[0] == false) && (OBDFLAG[0] != OLDOBDFLAG[0])) tft.fillRect(160, 2 + 18, 160, 42, BG_COLOR);
    if ((OBDFLAG[1] == true) && (OBDFLAG[1] != OLDOBDFLAG[1]))
      printpos(F("COLD"), 160, 72);
    else if ((OBDFLAG[1] == false) && (OBDFLAG[1] != OLDOBDFLAG[1])) tft.fillRect(160, 42 + 18, 160, 19, BG_COLOR);
    if ((OBDFLAG[2] == true) && (OBDFLAG[2] != OLDOBDFLAG[2]))
      printpos(F("KNOCKING"), 160, 90);
    else if ((OBDFLAG[2] == false) && (OBDFLAG[2] != OLDOBDFLAG[2])) tft.fillRect(160, 59 + 18, 160, 19, BG_COLOR);
    if ((OBDFLAG[3] == true) && (OBDFLAG[3] != OLDOBDFLAG[3]))
      printpos(F("CLOSED LOOP"), 160, 108);
    else if ((OBDFLAG[3] == false) && (OBDFLAG[3] != OLDOBDFLAG[3]))  tft.fillRect(160, 78 + 18, 160, 19, BG_COLOR);
    if ((OBDFLAG[4] == true) && (OBDFLAG[4] != OLDOBDFLAG[4])) {
      printpos(F("ACCEL"), 160, 126);
      printpos(F("ENRICH"), 160, 144);
    } else if ((OBDFLAG[4] == false) && (OBDFLAG[4] != OLDOBDFLAG[4])) tft.fillRect(160, 95 + 18, 160, 42, BG_COLOR);
    if ((OBDFLAG[5] == true) && (OBDFLAG[5] != OLDOBDFLAG[5]))
      printpos(F("STARTER"), 160, 162);
    else if ((OBDFLAG[5] == false) && (OBDFLAG[5] != OLDOBDFLAG[5])) tft.fillRect(160, 130 + 18, 160, 19, BG_COLOR);
    if ((OBDFLAG[6] == true) && (OBDFLAG[6] != OLDOBDFLAG[6])) {
      printpos(F("IDLE"), 160, 180);
    } else if ((OBDFLAG[6] == false) && (OBDFLAG[6] != OLDOBDFLAG[6])) tft.fillRect(160, 149 + 18, 160, 19, BG_COLOR);
    if ((OBDFLAG[7] == true) && (OBDFLAG[7] != OLDOBDFLAG[7])) {
      printpos(F("AIR COND"), 160, 198);
    } else if ((OBDFLAG[7] == false) && (OBDFLAG[7] != OLDOBDFLAG[7])) tft.fillRect(160, 168 + 18, 160, 19, BG_COLOR);
    if ((OBDFLAG[8] == true) && (OBDFLAG[8] != OLDOBDFLAG[8])) {
      printpos(F("NEUTRAL"), 160, 216);
    } else  if ((OBDFLAG[8] == false) && (OBDFLAG[8] != OLDOBDFLAG[8])) tft.fillRect(160, 185 + 18, 160, 19, BG_COLOR);
    if (OBDFLAG[9] != OLDOBDFLAG[9]) tft.fillRect(160, 221, 160, 19, BG_COLOR);
    if ((OBDFLAG[9] == true) && (OBDFLAG[9] != OLDOBDFLAG[9]))
      printpos(F("LEAN"), 160, 234);
    else if ((OBDFLAG[9] == false) && (OBDFLAG[9] != OLDOBDFLAG[9]))
      printpos(F("RICH"), 160, 234);
  }
} // end void drawalldata
