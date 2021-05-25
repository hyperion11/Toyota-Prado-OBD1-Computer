
void getOBD() {
  OBDDATA[OBD_INJ] = ToyotaData[OBD_INJ + 1] * 0.125;            //0
  OBDDATA[OBD_IGN] = ToyotaData[OBD_IGN + 1] * 0.47 - 30.0;        //1
  OBDDATA[OBD_IAC] = ToyotaData[OBD_IAC + 1] * 0.39215;          //2
  OBDDATA[OBD_RPM] = ToyotaData[OBD_RPM + 1] * 25.0;               //3
  OBDDATA[OBD_MAP] = ToyotaData[OBD_MAP + 1] * 2.0; //MAF         4
  if (ToyotaData[OBD_ECT + 1] >= 243)
    OBDDATA[OBD_ECT] = ((float)(ToyotaData[OBD_ECT + 1] - 243.0) * 9.8) + 122.0;
  else if (ToyotaData[OBD_ECT + 1] >= 237)
    OBDDATA[OBD_ECT] = ((float)(ToyotaData[OBD_ECT + 1] - 237.0) * 3.83) + 99.0;
  else if (ToyotaData[OBD_ECT + 1] >= 228)
    OBDDATA[OBD_ECT] = ((float)(ToyotaData[OBD_ECT + 1] - 228.0) * 2.11) + 80.0;
  else if (ToyotaData[OBD_ECT + 1] >= 210)
    OBDDATA[OBD_ECT] = ((float)(ToyotaData[OBD_ECT + 1] - 210.0) * 1.11) + 60.0;
  else if (ToyotaData[OBD_ECT + 1] >= 180)
    OBDDATA[OBD_ECT] = ((float)(ToyotaData[OBD_ECT + 1] - 180.0) * 0.67) + 40.0;
  else if (ToyotaData[OBD_ECT + 1] >= 135)
    OBDDATA[OBD_ECT] = ((float)(ToyotaData[OBD_ECT + 1] - 135.0) * 0.44) + 20.0;
  else if (ToyotaData[OBD_ECT + 1] >= 82)
    OBDDATA[OBD_ECT] = ((float)(ToyotaData[OBD_ECT + 1] - 82.0) * 0.38);
  else if (ToyotaData[OBD_ECT + 1] >= 39)
    OBDDATA[OBD_ECT] = ((float)(ToyotaData[OBD_ECT + 1] - 39.0) * 0.47) - 20.0;
  else if (ToyotaData[OBD_ECT + 1] >= 15)
    OBDDATA[OBD_ECT] = ((float)(ToyotaData[OBD_ECT + 1] - 15.0) * 0.83) - 40.0;
  else
    OBDDATA[OBD_ECT] = ((float)(ToyotaData[OBD_ECT + 1] - 15.0) * 2.0) - 60.0; // 5
  //endif OBD_ECT
  OBDDATA[OBD_TPS] = ToyotaData[OBD_TPS + 1] / 1.8;            //6
  OBDDATA[OBD_SPD] = ToyotaData[OBD_SPD + 1];                  //7/
  OBDDATA[OBD_OXSENS] = map((float)ToyotaData[OBD_OXSENS + 1] * 0.01953125f, 0, 5, -20, 20); //8    0-5V  -20%....20%
#ifdef SECOND_O2SENS
  OBDDATA[OBD_OXSENS2] = (float)ToyotaData[OBD_OXSENS2 + 1] * 0.01953125f; //9
#endif
  OBDFLAG[0] = bitRead(ToyotaData[11], 0); //  Переобогащение после запуска 1-Вкл
  OBDFLAG[1] = bitRead(ToyotaData[11], 1); //Холодный двигатель 1-Да
  OBDFLAG[2] = bitRead(ToyotaData[11], 4); //Детонация 1-Да
  OBDFLAG[3] = bitRead(ToyotaData[11], 5); //Обратная связь по лямбда зонду 1-Да
  OBDFLAG[4] = bitRead(ToyotaData[11], 6); //Дополнительное обогащение 1-Да
  OBDFLAG[5] = bitRead(ToyotaData[12], 0); //Стартер 1-Да
  OBDFLAG[6] = bitRead(ToyotaData[12], 1); //Признак ХХ (Дроссельная заслонка) 1-Да(Закрыта)
  OBDFLAG[7] = bitRead(ToyotaData[12], 2); //Кондиционер 1-Да
  OBDFLAG[8] = bitRead(ToyotaData[12], 3); //Нейтраль 1-Да
  OBDFLAG[9] = bitRead(ToyotaData[12], 4); //Смесь  первой половины 1-Богатая, 0-Бедная
#ifdef SECOND_O2SENS //Вторая лябмда для Vобразных движков
  OBDFLAG[10] = bitRead(ToyotaData[12], 5); //Смесь второй половины 1-Богатая, 0-Бедная
#endif
}

//-------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------
void ChangeState() {
  static uint8_t ID, EData[TOYOTA_MAX_BYTES];
  static boolean InPacket = false;
  static unsigned long StartMS;
  static uint16_t BitCount;
  int state = digitalRead(ENGINE_DATA_PIN);
  //  digitalWrite(LED_PIN, state);
  if (InPacket == false)  {
    if (state == MY_HIGH)   {
      StartMS = millis();
    }   else   { // else  if (state == MY_HIGH)
      if ((millis() - StartMS) > (15 * 8))   {
        StartMS = millis();
        InPacket = true;
        BitCount = 0;
      } // end if  ((millis() - StartMS) > (15 * 8))
    } // end if  (state == MY_HIGH)
  }  else   { // else  if (InPacket == false)
    uint16_t bits = ((millis() - StartMS) + 1 ) / 8; // The +1 is to cope with slight time errors
    StartMS = millis();
    // process bits
    while (bits > 0)  {
      if (BitCount < 4)  {
        if (BitCount == 0)
          ID = 0;
        ID >>= 1;
        if (state == MY_LOW)  // inverse state as we are detecting the change!
          ID |= 0x08;
      }   else    { // else    if (BitCount < 4)
        uint16_t bitpos = (BitCount - 4) % 11;
        uint16_t bytepos = (BitCount - 4) / 11;
        if (bitpos == 0)      {
          // Start bit, should be LOW
          if ((BitCount > 4) && (state != MY_HIGH))  { // inverse state as we are detecting the change!
            ToyotaFailBit = BitCount;
            InPacket = false;
            break;
          } // end if ((BitCount > 4) && (state != MY_HIGH))
        }  else if (bitpos < 9)  { //else TO  if (bitpos == 0)
          EData[bytepos] >>= 1;
          if (state == MY_LOW)  // inverse state as we are detecting the change!
            EData[bytepos] |= 0x80;
        } else { // else if (bitpos == 0)
          // Stop bits, should be HIGH
          if (state != MY_LOW)  { // inverse state as we are detecting the change!
            ToyotaFailBit = BitCount;
            InPacket = false;
            break;
          } // end if (state != MY_LOW)
          if ( (bitpos == 10) && ((bits > 1) || (bytepos == (TOYOTA_MAX_BYTES - 1))) ) {
            ToyotaNumBytes = 0;
            ToyotaID = ID;
            for (uint16_t i = 0; i <= bytepos; i++)
              ToyotaData[i] = EData[i];
            ToyotaNumBytes = bytepos + 1;
            if (bits >= 16)  // Stop bits of last byte were 1's so detect preamble for next packet
              BitCount = 0;
            else  {
              ToyotaFailBit = BitCount;
              InPacket = false;
            }
            break;
          }
        }
      }
      ++BitCount;
      --bits;
    } // end while
  } // end (InPacket == false)
} // end void change
