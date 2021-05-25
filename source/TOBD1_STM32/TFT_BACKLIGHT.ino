void backlightadjust() {
  {
    if (readIndex < 10)
    {
      TFT_LED.in += 4095 - analogRead(LIGHT_SENSOR_PIN); //набор данных с сенсора света для усреднения
      readIndex++;
    }
    else
    {
      readIndex = 0;
      TFT_LED.in_med = (int)(TFT_LED.in / 10);   //усредняем
      TFT_LED.target = map(TFT_LED.in_med, 0, 4095, 200, 63000); //масштабируем
      TFT_LED.in = 0;
    }
    if (TFT_LED.current - TFT_LED.P > TFT_LED.target)            //+- 20 чтобы яркость не прыгала
      TFT_LED.current -= TFT_LED.P;                             //увеличиваем яркость с шагом 20
    if (TFT_LED.current + TFT_LED.P < TFT_LED.target)            // +- 20 чтобы яркость не прыгала
      TFT_LED.current += TFT_LED.P;                             //уменьшаем яркость с шагом 20
    pwmWrite(TFT_LED_PIN, TFT_LED.current);
  }
}
