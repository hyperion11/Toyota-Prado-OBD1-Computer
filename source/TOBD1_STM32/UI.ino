//Центральная кнопка
//Короткое нажание на главном экране - эмуляция заправки полного бака
//Длинное нажание на любом экране кроме настроек - сброс пробега и расхода
//Длинное нажание на экране настроек - очистка fram и запись настроек по умолчанию

void readbuttons() {
  switch (S.read())
  {
    case MD_KeySwitch::KS_NULL:       break;
    case MD_KeySwitch::KS_PRESS:
      {
        Serial.println(F("Reset Button pressed "));
        if (CurrentDisplayIDX == 2) {
          fuel.remains = FuelCapacity; //короткое нажатие на центральную кнопку - эмуляция полного залитого бака.
          //в перспективе - перевод показателя ДУТ в литры
        }
        if (CurrentDisplayIDX == 4) {     //если экран с настройками то
          isActive = false;
          CurrentDisplayIDX = 2;          //выход из экрана с настройками
          if (color_scheme == 0) { //Зеленая схема
            TEXT_COLOR = ILI9341_GREEN;
            BG_COLOR = ILI9341_BLACK;
            BLUE_COLOR = ILI9341_BLUE;
            RED_COLOR = ILI9341_RED;
            GT_COLOR = ILI9341_GREEN;
          }
          if (color_scheme == 1) { //ЧБ схема
            TEXT_COLOR = ILI9341_WHITE;
            BG_COLOR = ILI9341_BLACK;
            BLUE_COLOR = ILI9341_WHITE;
            RED_COLOR = ILI9341_WHITE;
            GT_COLOR = ILI9341_GREY;
          }
          FRAM.write (52, RTCCR);
          if (getrtccr() != RTCCR)
            setrtccr(RTCCR);
          if (DRIFT != getdrift())
            setbkpdrift(DRIFT);
          drawScreenSelector();           //перерисовка экрана
        }
        break;
      case MD_KeySwitch::KS_DPRESS:     break;
        {
        }
      case MD_KeySwitch::KS_LONGPRESS:    //долгое нажание
        {
          if (CurrentDisplayIDX == 4) {
            erasefram();
            CurrentDisplayIDX = 2;
            drawScreenSelector();
          }
          else {
            cleardata();                    //очистка расхода и пробега
            drawScreenSelector();           //перерисовка экрана
          }

        }
        break;
      case MD_KeySwitch::KS_RPTPRESS:   break;
      }
  }
  newKeyValue = GetKeyValue(BUTTON_ARRAY_PIN);
  if (keyValue != newKeyValue) {  // Если новое значение не совпадает со старым - реагируем на него
    keyValue = newKeyValue;       // Актуализируем переменную хранения состояния
    Serial.println(keyValue);
    switch (keyValue) {
      case 1: { //левая верхняя кнопка
          if (CurrentDisplayIDX == 4) {   //если экран с настройками то
            _id = 3;                        //переход к настройке предыдущего параметра
            //_id = 1;                      //настраиваем предыдущий параметр
            drawScreenSelector();         //перерисовка экрана настроек
            break;
          }
          if (CurrentDisplayIDX > 1 ) {  //если не экран с настройками то
            CurrentDisplayIDX--;          //переходим на предыдущий экран
            isActive = false;             //флаг перерисовки всего экрана
            drawScreenSelector();         //функция вызова экрана
          }
        } break;
      case 2: { //правая верхняя кнопка
          if (CurrentDisplayIDX == 4) {   //если экран с настройками то
            if (isActive == true)
              _id = 4;                        //переход к настройке следующего параметра
            //_id = 2;                      //настраиваем следующий параметр
            drawScreenSelector();         //перерисовка экрана настроек
          }
          if (CurrentDisplayIDX < 4) {   //если не экран с настройками то
            CurrentDisplayIDX++;          //переходим на следующий экран
            isActive = false;             //флаг перерисовки всего экрана
            drawScreenSelector();         //функция вызова экрана
            break;
          }
        } break;
      case 3: { //левая нижняя кнопка
          if (CurrentDisplayIDX == 4) {     //если экран с настройками то
            _id = 1;                      //уменьшение текущего параметра
            //_id = 3;                        //увеличение текущего параметра
            drawScreenSelector();           //перерисовка экрана настроек
          }
        } break;
      case 4: { //правая нижняя кнопка
          if (CurrentDisplayIDX == 4) {     //если экран с настройками то
            _id = 2;                      //увеличение текущего параметра
            //_id = 4;                        //уменьшение текущего параметра
            drawScreenSelector();           //перерисовка экрана настроек
          }
        } break;
    }
  }
}
