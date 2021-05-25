void showtime()
{
  tft.setFont(&DSEG7ModernMini_Bold25pt7b );
  if (isActive == false)                    //активируется при смене экрана
  { //независимо прошла минута или нет
    tft.setCursor(57 + 20, 60 + 4);
    if (rt.hour() < 10) tft.print(F("0"));
    tft.print(rt.hour());
    tft.print(F(":"));
    if (rt.minute() < 10) tft.print(F("0"));
    tft.print(rt.minute());
  }
  if (omm != rt.minute())                  //активируется только раз в минуту
  {
    tft.setCursor(57 + 20, 60 + 4);
    tft.fillRect(56 + 20, 0, 205, 70, BG_COLOR);
    if (rt.hour() < 10) tft.print(F("0"));
    tft.print(rt.hour());
    tft.print(F(":"));
    if (rt.minute() < 10) tft.print(F("0"));
    tft.print(rt.minute());
    omm = rt.minute();
  }
}
void show_current_gear() {
  if (CurrentDisplayIDX == 2) {
    if (OBDDATA[OBD_SPD] < 1) {
      neutral();
    }
    else {
      switch (TT.current) { //для делителя 10k + 4.7k
        case 0:
          firstgear();
          break;
        case 1:
          secondgear();
          break;
        case 2:
          secondgear();
          break;
        case 3:
          thirdgear(false);
          break;
        case 4:
          thirdgear(false);
          break;
        case 5:
          thirdgear(true);
          break;
        case 6:
          fourthgear(false);
          break;
        case 7:
          fourthgear(true);
          break;
      }
    }
  }
}

void neutral()
{
  tft.fillRoundRect(146, 71, 26, 111, 5, BG_COLOR);
}
void firstgear()
{
  tft.fillRoundRect(146, 71, 26, 111, 5, BG_COLOR);
  tft.fillRoundRect(149, 155, 20, 20, 3, RED_COLOR);  //первая передача только с начала движения
}

void secondgear() {
  tft.fillRoundRect(146, 71, 26, 111, 5, BG_COLOR);
  tft.fillRoundRect(149, 155, 20, 20, 3, RED_COLOR);   //первая передача
  tft.fillRoundRect(149, 129, 20, 20, 3, RED_COLOR);   //вторая передача
}
void thirdgear(bool lock) {
  tft.fillRoundRect(146, 71, 26, 111, 5, BG_COLOR);
  if (lock == true)
    tft.fillRoundRect(146, 97, 26, 85, 5, GT_COLOR); //gt lock
  tft.fillRoundRect(149, 155, 20, 20, 3, RED_COLOR);   //первая передача
  tft.fillRoundRect(149, 129, 20, 20, 3, RED_COLOR);   //вторая передача
  tft.fillRoundRect(149, 103, 20, 20, 3, RED_COLOR);  //третья передача
}
void fourthgear(bool lock) {
  tft.fillRoundRect(146, 71, 26, 111, 5, BG_COLOR);
  if (lock == true)
    tft.fillRoundRect(146, 71, 26, 111, 5, GT_COLOR);
  tft.fillRoundRect(149, 155, 20, 20, 3, RED_COLOR);
  tft.fillRoundRect(149, 129, 20, 20, 3, RED_COLOR);
  tft.fillRoundRect(149, 103, 20, 20, 3, RED_COLOR);
  tft.fillRoundRect(149, 77, 20, 20, 3, RED_COLOR);
}

void printpos(const __FlashStringHelper * str, uint16_t posx, uint16_t posy) {
  tft.setCursor(posx, posy);
  tft.print(str);
}

void printpos(char *str, uint16_t posx, uint16_t posy) {
  tft.setCursor(posx, posy);
  tft.print(str);
}

void printpos(const char *str, uint16_t posx, uint16_t posy) {
  tft.setCursor(posx, posy);
  tft.print(str);
}

void printpos(float data, uint8_t num, uint16_t posx, uint16_t posy) {
  if (data < 0.01) data = 0.01;
  if (data > 999999) data = 999999;
  tft.setCursor(posx, posy);
  tft.print(data, num);
}
void printpos(int32_t data, uint16_t posx, uint16_t posy) {
  tft.setCursor(posx, posy);
  tft.print(data);
}

void send_tft2(float data, uint8_t num, uint8_t h, uint8_t w, uint16_t posx, uint16_t posy) {
  if (data < 0.01) data = 0.01;
  if (data > 999999) data = 999999;
  tft.setCursor(posx, posy);
  tft.fillRect(posx - 1, posy - h - 2, w, h + 4, BG_COLOR);
  tft.print(data, num);
}

void send_tft2(const __FlashStringHelper * data, uint8_t h, uint8_t w, uint16_t posx, uint16_t posy) {
  tft.setCursor(posx, posy);
  tft.fillRect(posx - 1, posy - h - 2, w, h + 4, BG_COLOR);
  tft.print(data);
}




//-------------------------------------------------------------------------------------------
void update_tft_float(uint8_t i, uint16_t XX, uint16_t YY) {
  if  (OLDOBDDATA[i] != OBDDATA[i]) {
    tft.setCursor(XX, YY);
    tft.fillRect(tft.getCursorX(), tft.getCursorY() - 16, 76, 19, BG_COLOR);
    tft.print(OBDDATA[i], 1);
  }
}
void update_tft_float(uint8_t i, uint16_t XX, uint16_t YY, uint8_t num) {
  if  (OLDOBDDATA[i] != OBDDATA[i]) {
    tft.setCursor(XX, YY);
    tft.fillRect(tft.getCursorX(), tft.getCursorY() - 16, 76, 19, BG_COLOR);
    tft.print(OBDDATA[i], num);
  }
}

//-------------------------------------------------------------------------------------------
void update_tft_int(uint8_t i, uint16_t XX, uint16_t YY) {
  if  (OLDOBDDATA[i] != OBDDATA[i]) {
    tft.setCursor(XX, YY);
    tft.fillRect(tft.getCursorX(), tft.getCursorY() - 16, 76, 19, BG_COLOR);
    tft.print(int(OBDDATA[i]));
  }
}



//-------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------
void tft_string (const char *s) {
  for (uint8_t i = 0; i < strlen(s); i++) {
    tft.print(s[i]);
    delay (10);
  }
  tft.println();
}
//-------------------------------------------------------------------------------------------
void FramWriteIndicator(void) {
  /*индикатор записи в fram. Сначала делается проход радиусами до 10 пикселей зеленым цветом.
     после этого обнуляется и снова закрашивается красным. И так по кругу
  */
  if (FRAM_S.dir == true)
    tft.drawCircle(300, 15, FRAM_S.iteration, ILI9341_GREEN);
  else
    tft.drawCircle(300, 15, FRAM_S.iteration, ILI9341_RED);
  FRAM_S.iteration++;
  if  (FRAM_S.iteration == 10)
  {
    FRAM_S.iteration = 1;
    FRAM_S.dir = !FRAM_S.dir;
  }
}
