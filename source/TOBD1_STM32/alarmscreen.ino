void alarmscreen(void) {
  isActive = false;
  tft.fillScreen(BG_COLOR);
  tft.setTextColor(ILI9341_RED);
  tft.setFont(&CPMono_v07_Plain13pt7b );
  printpos(F("ALARM!!!"), 80, 64);
  printpos(F("Voltage"), 85, 120);
  tft.setTextColor(ILI9341_WHITE);
  tft.setFont(&DSEG7ModernMini_Bold25pt7b );
  printpos(ADC.vcc_avg * VREF_MEASURED * VCC_PIN_DIVIDER / 4095.0, 1, 80, 185);
}
