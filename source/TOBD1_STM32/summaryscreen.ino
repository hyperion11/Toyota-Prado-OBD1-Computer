void summary_screen(void) {
  tft.setTextColor(TEXT_COLOR);
  tft.setFont(&CPMono_v07_Plain9pt7b);
  if (isActive == false) {                    //форсирую перерисовку  после переключения экрана
    tft.fillScreen(BG_COLOR);
    isActive = true;        // ставлю флаг активного экрана чтобы не перерисовывать лишнее
    printpos(F("TRIP FUEL  L"), 10, 35);
    printpos(F("TRIP AVG   L"), 10, 55);
    printpos(F("TRIP      KM"), 10, 75);
    printpos(F("TRIP AVG SPD"), 10, 95);
    printpos(F("TOTAL FUEL L"), 10, 115);
    printpos(F("TOTAL AVG  L"), 10, 135);
    printpos(F("TOTAL     KM"), 10, 155);
    printpos(F("TOTAL AVGSPD"), 10, 175);
    printpos(F("FUEL TANK  V"), 10, 195);
    printpos(F("FUEL TANK V2"), 10, 215);
    printpos(F("TOTAL   TIME"), 10, 235);
    printpos(trip_fuel_consumption, 1, 160, 35);
    printpos(trip_avg_fuel_consumption, 1, 160, 55);
    printpos(trip_km, 1, 160, 75);
    printpos(trip_avg_speed, 1, 160, 95);
    printpos(total_fuel_consumption, 1, 160, 115);
    printpos(total_avg_fuel_consumption, 1, 160, 135);
    printpos(total_km, 1, 160, 155);
    printpos(total_avg_spd, 1, 160, 175);
    printpos(fuel.volt, 2, 160, 195);
    printpos(fuel.volt_avg, 2, 160, 215);
    printpos(TimeToString(total_time), 160, 235);
  }                        //конец перерисовки
  else
  {
    //данные, число разрядов, высота символов, длина стираемой строки, позицияХ, позицияУ
    send_tft2(trip_fuel_consumption, 1, 12, 160, 160, 35);
    send_tft2(trip_avg_fuel_consumption, 1, 12, 160, 160, 55);
    send_tft2(trip_km, 1, 12, 160, 160, 75);
    send_tft2(trip_avg_speed, 1, 12, 160, 160, 95);
    send_tft2(total_fuel_consumption, 1, 12, 160, 160, 115);
    send_tft2(total_avg_fuel_consumption, 1, 12, 160, 160, 135);
    send_tft2(total_km, 1, 12, 160, 160, 155);
    send_tft2(total_avg_spd, 1, 12, 160, 160, 175);
    send_tft2(fuel.volt, 2, 12, 160, 160, 195);
    send_tft2(fuel.volt_avg, 2, 12, 160, 160, 215);
    tft.fillRect(157, 217, 138, 25, BG_COLOR);
    printpos(TimeToString(total_time), 160, 235);
  } // end void drawalldata
}
