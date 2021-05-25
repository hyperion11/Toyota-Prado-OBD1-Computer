//todo
//####перерисовка 3го экрана кривая -  работает
//####нету сброса полного километража  -работает
//!!!!добавить всетаки отдельный километраж который не будет сбрасываться.  -проверить
//####индикатор акпп не показывает нейтраль    -  проверить фикс - не показывается 1я передача - проверить фикс - все работает
//####редко обновляется индикатор среднего расхода на главной панели - проверить фикс - мелькают большие цифры - проверить фикс - все работает
//####добавить схему белый тест на черном фоне - готово
//добавить экран с датой и временем
//####добавить калибровку производительности форсунок - готово
//наладить форматирование на экране настроек
//допилить измерение уровня топлива
//#####LPH не всегда обновляется и слишком много стирается справа
//#####сместить вниз и вправо весь первый экран - проверить
//#####сместить вниз и вправо весь третий экран - проверить
//#####AVG расход не всегда обновляется - проверить
//#####VF изменил диапазон от 0до5В - старая формула была верная. Вернул обратно старую формулу
//######изменил работу подсветки - проверить -
//######добавил счетчик стартов устройства. Добавил счетчик записей в FRAM. Проверить работу.
//#####уменьшил шрифт часов. Проверить центровку.
//#####добавил процентное отображение топливных коррекций
//7.12.2018 Поправил очистку экрана для Trip Fuel/Fuel Remaind > 1000
//          поправил ввывод времени поездки
//9.12.2018 Попробовал сделать коррекцию времени. setbktime, getbktime, timecorrection, FRAM.read(50, DRIFT_DUR);









#define RTC_BTP_IN_FRAM //RTC_BKP_IN_BKP
#define DEBUG_SERIAL_OUTPUT
#define DEBUG_RTC
//#define DEBUG_SCREEN
#define VZ   // VZ - 5VZ; RZ - 3RZ
//#define ADS1115 //Внешний ADC на 4 канала
//HW VERSIONS:
//2  -  PCB 2.2 - TT Делитель 5.6+3k = 2.87 9В максимум
//3  -  PCB 2.3 - TT Делитель 5.6+1.5k = 4.73 15В максимум
#define HW_VERSION 2
#define SW_VERSION 2.2
#include "defines.h"
#include "variables.h"
#include "extra.h"
#include "src\FRAM24CXX\FRAM24CXX.h"
#include "SPI.h"
#include <RTClock.h>
#include "src\MD_KeySwitch\MD_KeySwitch.h"
#include "src\rtadjust\rtadjust.h"
//#include <Adafruit_ILI9341_STM.h>

#include <Adafruit_ILI9341.h>           //     <<<<<<<<<<<<<<
#include <Adafruit_GFX.h>            //<<<<<<<<<<<<<

#include "src\CPMonoP9pt7b.h"
#include "src\CPMono_L6pt7b.h"
#include "src\CPMono_L13pt7b.h"
#include "src\DSEG7ModernMini-Bold13pt7b.h"
#include "src\DSEG7ModernMini-Bold25pt7b.h"

//конструкторы
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC); // Use hardware SPI
FRAM24CXX FRAM(0x50, 4); //chipaddress, chipDensity
RTClock rt (RTCSEL_LSE);
tm_t tm;
MD_KeySwitch S(RESET_PIN, HIGH);


void setup() {
  Serial.begin(115200);
  Serial1.begin(115200);
  delay (300);
  Wire.begin();
  tft.begin();
  tft.fillScreen(ILI9341_RED);
  FRAM.begin();
  pinMode(ENGINE_DATA_PIN, INPUT); // VF1 PIN
  pinMode(TT_PIN, INPUT_ANALOG);
  pinMode(FUEL_PIN, INPUT_ANALOG);
  pinMode(VCC_PIN, INPUT_ANALOG);
  pinMode(BUTTON_ARRAY_PIN, INPUT_ANALOG);
  pinMode(TFT_LED_PIN, PWM);
  pinMode(LIGHT_SENSOR_PIN, INPUT_ANALOG);
  Serial.println("Pins attached");
  attachInterrupt(ENGINE_DATA_PIN, ChangeState, CHANGE); //setup Interrupt for data line
#if defined(DEBUG_SERIAL_OUTPUT)
  Serial.println("Interrrupt attached");
#endif
  S.begin();
  S.enableDoublePress(false);
  S.enableLongPress(true);
  S.enableRepeat(false);
  S.enableRepeatResult(false);
  S.setDoublePressTime(300);
  S.setLongPressTime(2000);
  //S.setDebounceTime(100);
  tft.setRotation(3);
  bkp_init();
  read_fram_data();
  tft_string ("   ENGINE:");
#if defined(VZ)
  tft_string ("   5VZ-FE");
#elif defined(RZ)
  tft_string ("   3RZ-FE");
#else
  tft_string ("   Unknown Engine");
#endif
  tft.print("HW VERSION: ");
  tft.println(HW_VERSION);
  tft.print("SW VERSION: ");
  tft.println(SW_VERSION, 1);

  delay (2000);
#if defined(DEBUG_SERIAL_OUTPUT)
  Serial.println("Buttons attached");
#endif
  CurrentDisplayIDX = 2; // set to display 2
  last_log_time = 0;
  t = millis();
  omm = rt.minute();
  start_count++;
  FRAM.write(41, start_count);
  writeHeader();
  isActive = false;
  ADC.index = 0;
  ADC.tt_in = 0;
  ADC.fuel_in = 0;
  ADC.vcc_in = 0;
  init_ = false;
#if defined(DEBUG_SERIAL_OUTPUT)
  Serial.println("INIT COMPLETE");
#endif
    adjtime();
} // END VOID SETUP

void loop(void) {
#if defined(DEBUG_RTC)
  if (Serial.available())
    processkey();
#endif
  if (millis() % 2 == 0)
  {
    readbuttons();     //каждые 2мс происходит чтение состояния кнопок
    backlightadjust(); //каждые 2мс оценивается яркость и корректируется
    if (ADC.index < 20)
    {
      ADC.vcc_in += analogRead(VCC_PIN) + VCC_DROP;
      ADC.fuel_in += analogRead(FUEL_PIN);
      ADC.index++;
    }
    if (ADC.tt_index < 100) {
      ADC.tt_in += analogRead(TT_PIN);
      ADC.tt_index++;
    }
    else ADC.is_ready = true;
  }
  if (millis() % 500 == 0 && ADC.is_ready == true) {
    ADC.tt_avg = ADC.tt_in / ADC.tt_index;
    TT.current = (int)(ADC.tt_avg * VREF_MEASURED * TT_PIN_DIVIDER / 4095.0  + 0.5);
    if (TT.last != TT.current) {
      show_current_gear(); //принудительная перерисовка
      TT.last = TT.current;
    }
    ADC.tt_index = 0;
    ADC.tt_in = 0;
    ADC.is_ready = false;
  }
  if ((ToyotaNumBytes > 0) && (CurrentDisplayIDX != 4))  {  // if found bytes
    if (flag.f_num == 3) {        //    ротация
      flag.isTrip = !flag.isTrip; //    данных
      flag.TripActive = false;    //     на
      flag.f_num = 0;             //    главном экране
    }                             //    раз в три новых
    flag.f_num++;                 //    пакета OBD данных
    getOBD();   //конвертация данных + оформление их в массив
    new_t = millis();
    if (new_t > t && OBDDATA[OBD_RPM] > 100 ) {// выполняем только  на работающем двигателе
      diff_t = new_t - t;
      cycle_obd_inj_dur = OBDDATA[OBD_RPM]  * Ncyl * (float)diff_t  * OBDDATA[OBD_INJ] / 120000.0; //Время открытых форсунок за 1 такт данных. В МС
      //форсунка срабатывает раз в 2 оборота КВ
      //время цикла мс в с. Получаем кол-во срабатываний за время цикла. Умножаем на время открытия форсунки, получаем время открытия 6 форсунок В МИЛЛИСЕКУНДАХ
      trip_inj_dur += cycle_obd_inj_dur;                                                              //Время открытых форсунок за поездку        В МС
      total_inj_dur += cycle_obd_inj_dur;                                                           //Время открытых форсунок за все время. EEPROM    В МС
      inj_dur_RO += cycle_obd_inj_dur;
      cycle_trip = (float)diff_t  * OBDDATA[OBD_SPD] / 3600000.0;  //расстояние пройденное за такт обд данных
      trip_km += cycle_trip;  //Полное пройденное расстояние. EEPROM. В КМ
      total_km += cycle_trip;       //электронный одометр. Хранится в еепром и не стирается кнопкой
      km_RO += cycle_trip;
      trip_time += diff_t;             //Время в пути в миллисекундах с момента включения
      total_time += diff_t;
      time_RO += diff_t;
      fuel.remains -= cycle_obd_inj_dur * Ls / 1000.0; //остаток в баке стал децл меньше
      t = new_t;//тест
      ADC.fuel_avg = ADC.fuel_in / ADC.index;
      fuel.volt = (float)analogRead(FUEL_PIN) * VREF_MEASURED * FUEL_PIN_DIVIDER / 4095.0;
      fuel.volt_avg = (float)ADC.fuel_avg * VREF_MEASURED * FUEL_PIN_DIVIDER / 4095.0;
      ADC.vcc_avg = ADC.vcc_in / ADC.index;
      logData();
      if (millis() - last_log_time > 60000) SaveToFram(); //запись данных каждую минуту
      if (OBDDATA[OBD_SPD] == 0 && flagNulSpeed == false) SaveToFram(); //запись данных при остановке
      if (OBDDATA[OBD_SPD] != 0) flagNulSpeed = false;     //начали двигаться - разрешаем запись
    }
    trip_fuel_consumption = trip_inj_dur  * Ls / 1000.0;    //потребление топлива за поездку в литрах
    total_fuel_consumption = total_inj_dur  * Ls / 1000.0;  //потребление топлива за все время. Из ЕЕПРОМ в литрах
    trip_avg_fuel_consumption = 100.0 * trip_fuel_consumption / trip_km; //средний расход за поездку
    total_avg_fuel_consumption = 100.0 * total_fuel_consumption / total_km;
    trip_avg_speed = trip_km  * 3600000.0 / (float)trip_time ;       //средняя скорость за поездку
    total_avg_spd = total_km * 3600000.0 / (float)total_time ;
    if (ADC.vcc_avg < 2317  || ADC.vcc_avg > 2767 )
    {
      if (alarms == true)
      {
        alarmscreen();
#if defined(DEBUG_SERIAL_OUTPUT)
        Serial.println("ALARM!!!");
        Serial.print("VCC: ");
        Serial.println(ADC.vcc_avg * VREF_MEASURED * VCC_PIN_DIVIDER / 4095.0, 2);
        Serial.print("VCC RAW: ");
        Serial.println(ADC.vcc_avg);
#endif
      }
      else
        drawScreenSelector();
    }
    else
      drawScreenSelector();
#if defined(DEBUG_SERIAL_OUTPUT)
    Serial.println("DEBUG MESSAGES: ");
    Serial.print("VCC: ");
    Serial.println(ADC.vcc_avg * VREF_MEASURED * VCC_PIN_DIVIDER / 4095.0, 2);
    Serial.print("TT: ");
    Serial.println(ADC.tt_avg * VREF_MEASURED * TT_PIN_DIVIDER / 4095.0, 2);
    Serial.print("FUEL: ");
    Serial.println(fuel.volt, 2);
#endif
    for (uint8_t i = 0; i < TOBD_ID_MAX; i++)
      OLDOBDDATA[i] = OBDDATA[i];
    for (uint8_t i = 0; i < TOBD_ID_MAX + 1; i++)
      OLDOBDFLAG[i] = OBDFLAG[i];
    ToyotaNumBytes = 0;     // reset the counter.
    ADC.fuel_in = 0;
    ADC.vcc_in = 0;
    ADC.index = 0;
  } // end if (ToyotaNumBytes > 0)
}


void drawScreenSelector(void) {
  if (CurrentDisplayIDX == 1)
    drawAllData();
  else if (CurrentDisplayIDX == 2)
    mainscreen();
  else if (CurrentDisplayIDX == 3)
    summary_screen();
  else if (CurrentDisplayIDX == 4)
    settings();
} // end drawScreenSelector()
