#ifndef VARIABLES_H
#define VARIABLES_H

struct TFT_LED_STRUCT
{
  uint16_t in_med;
  uint16_t current;
  uint16_t target;
  const uint8_t P = 5;
  uint16_t in = 0;
};

struct LOG_STRUCT
{
  boolean enabled;
  boolean started;
};

struct ADC_STRUCT
{
  uint32_t vcc_in;
  uint32_t fuel_in;
  uint32_t tt_in;
  uint8_t index;
  uint8_t tt_index;
  uint16_t vcc_avg;
  uint16_t fuel_avg;
  uint16_t tt_avg;
  boolean is_ready;
};


struct FUEL_STRUCT
{
  float volt;
  float volt_avg;
  float remains;
};

struct TT_STRUCT
{
  uint8_t last;
  uint8_t current;
};
struct FLAG_STRUCT
{
  uint8_t f_num = 0;
  boolean TripActive = false;
  boolean isTrip = false;
};

struct FRAM_STRUCT
{
  uint8_t iteration = 1;
  boolean dir = true;
};
ADC_STRUCT ADC;
LOG_STRUCT logging;
FUEL_STRUCT fuel;
TFT_LED_STRUCT TFT_LED;
FLAG_STRUCT flag;
TT_STRUCT TT;
FRAM_STRUCT FRAM_S;

const int16_t DRIFT_MAX = 8255;     //1048576 / 127;
float OBDDATA[TOBD_ID_MAX], OLDOBDDATA[TOBD_ID_MAX];
bool OBDFLAG[TOBD_ID_MAX + 1], OLDOBDFLAG[TOBD_ID_MAX + 1];
boolean _isLPK = false;
float total_fuel_consumption = 0;
float total_inj_dur = 0, inj_dur_RO, Ls;
float trip_fuel_consumption = 0;
float trip_avg_fuel_consumption, total_avg_fuel_consumption;
float trip_inj_dur = 0;
float trip_avg_speed, total_avg_spd;
float trip_km, total_km, km_RO;

float cycle_obd_inj_dur = 0;
float cycle_trip = 0;
float maindata[5], maindataold[5];
uint32_t trip_time, total_time, time_RO;
uint32_t t, TEXT_COLOR, BG_COLOR, RED_COLOR, BLUE_COLOR, GT_COLOR;
uint32_t last_log_time = 0;
uint32_t new_t, readings = 0, erase_start, erase_end;
bool flagNulSpeed = true, isActive = false, _fram;
boolean LoggingOn = false, init_ = true, alarms = false;

volatile uint8_t RTCCR  = 0;
volatile int16_t DRIFT = 0;
volatile uint32_t BKPTIME = 0;
uint16_t old_trip_km = 0;
uint16_t readIndex = 0;
uint16_t diff_t;
uint16_t start_count, write_count;
uint8_t CurrentDisplayIDX;
uint8_t work_id = 0;
uint8_t color_scheme = 0;
volatile uint8_t ToyotaNumBytes, ToyotaID, ToyotaData[TOYOTA_MAX_BYTES];
volatile uint16_t ToyotaFailBit = 0;
uint8_t omm;
uint8_t keyValue  =  0;
uint8_t newKeyValue;
uint8_t _id = 10; //чтобы ничего не происходило по дефолту
const char * months[] = {"Dummy", "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };
const char* colorscheme[2] = {"GREEN", "B/W"};
const char * delim = " :";
char s[128]; // for sprintf
#endif
