#ifndef DEFINES_H
#define DEFINES_H

#define VREF_MEASURED 3.32              //Измеренное опорное напряжение с 3.3В стабилизатора

#if (HW_VERSION==2)
#define TT_PIN_DIVIDER 2.87
#elif (HW_VERSION==3)
#define TT_PIN_DIVIDER 4.73
#endif

#if defined (ADS1115)
#define ADS1115_A0_A1_DIVIDER 2
#define ADS1115_A2_A3_DIVIDER 4.73
#endif

#define FUEL_PIN_DIVIDER 4.73
#define VCC_PIN_DIVIDER 6.6
#define interval2 2                      //2ms Интервал
#define interval500 500
//#define SECOND_O2SENS  // Включение 2го сенсора кислорода для V движков
#define DEBUG_OUTPUT false // for debug option - swith output to Serial
//DEFINE пинов под входы-выходы
//ADC PIN's  3, 4, 5, 6, 7, 8, 9, 10, 11
//#define LED_PIN         33  //встроенный светодиод
#define FUEL_PIN           PB0  //D3-для датчика уровня топлива
#define TT_PIN             PA3  //D8-PA3 для сенсора ТТ АКПП
#define ENGINE_DATA_PIN    PB4  //D18-PB5-EXTI5 VF1 PIN
#define TFT_CS             PA4  //D7   //SS
#define TFT_DC             PC13 //D14
#define TFT_LED_PIN        PA8  //27 -PA2 для шим подсветки дисплея
#define LIGHT_SENSOR_PIN   PA1  //D10-PA1 Для фоторезистора
#define BUTTON_ARRAY_PIN   PA0  //D11-PA0 для массива из 4х кнопок
#define RESET_PIN          PA15  //D20 отдельно для кнопки сброса //2.1
#define VCC_PIN            PA2 //D9 сенсор напряжения 12В
#define VCC_DROP           112 //падение напряжения на диоде 0.6v/6.6/3.32

//DEFINE констант расходомера
#if defined(VZ)
#define Ncyl 6.0      //6 форсунок
#elif defined(RZ)
#define Ncyl 4.0        //4 форсунки
#else
#error “Unsupported Engine!!!”
#endif

#define FuelCapacity 90
//DEFINE OBD READER
#define  MY_HIGH  HIGH //LOW    // I have inverted the Eng line using an Opto-Coupler, if yours isn't then reverse these low & high defines.
#define  MY_LOW   LOW //HIGH
#define  TOYOTA_MAX_BYTES  24
#define OBD_INJ 0 //Injector pulse width (INJ)
#define OBD_IGN 1 //Ignition timing angle (IGN)
#define OBD_IAC 2 //Idle Air Control (IAC)
#define OBD_RPM 3 //Engine speed (RPM)
#define OBD_MAP 4 //Manifold Absolute Pressure (MAP)
#define OBD_ECT 5 //Engine Coolant Temperature (ECT)
#define OBD_TPS 6 // Throttle Position Sensor (TPS)
#define OBD_SPD 7 //Speed (SPD)
#define OBD_OXSENS 8 // Oxygen sensor 1

#ifdef SECOND_O2SENS    // Oxygen sensor 2
#define TOBD_ID_MAX 10
#define OBD_OXSENS2 9
const char* TOBD_ID_NAME[TOBD_ID_MAX] = {"INJ", "IGN", "IAC", "RPM", "MAF", "ECT", "TPS", "SPD", "VF1", "VF2"};
#else
#define TOBD_ID_MAX 9
const char* TOBD_ID_NAME[TOBD_ID_MAX] = {"INJ", "IGN", "IAC", "RPM", "MAF", "ECT", "TPS", "SPD", "VF"};
#endif

#define ILI9341_GREY 0x8410
#endif
