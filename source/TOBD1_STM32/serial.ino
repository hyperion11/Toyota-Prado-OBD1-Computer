void writeHeader() {
  Serial1.println(F("Init complete!"));
  Serial1.print(F("Launch date: ;"));
  Serial1.print(rt.day());
  Serial1.print(F("."));
  Serial1.print(rt.month());
  Serial1.print(F("."));
  Serial1.println(rt.year() + 1970);
  Serial1.print(F("Launch time: ;"));
  Serial1.print(rt.hour());
  Serial1.print(F(":"));
  Serial1.print(rt.minute());
  Serial1.print(F(":"));
  Serial1.println(rt.second());
  Serial1.print(F("Launch count: ;"));
  Serial1.print(start_count);
  Serial1.print(F("; Write count: ;"));
  Serial1.println(write_count);
  Serial1.print(F("TOTAL KM: ;"));
  Serial1.print(km_RO, 0);
  Serial1.print(F("; TOTAL AVG SPD: ;"));
  Serial1.print((km_RO * 3600000.0 / (float)time_RO), 1);
  Serial1.print(F("; TOTAL AVG FUEL: ;"));
  Serial1.print(inj_dur_RO  * Ls / 10.0 / km_RO, 1);
  Serial1.print(F("; TOTAL  FUEL: ;"));
  Serial1.println(inj_dur_RO  * Ls / 1000.0, 1);
  Serial1.print(F("TIME;INJ;IGN;IAC;RPM;MAF;ECT;TPS;SPD;VF1;ASE;CL;AE;"));
  Serial1.println(F("IDL;NEUTRAL;OX;Trip Avg L; Trip L; Total L;TTavg;TTcur;Fuel Volt;Fuel Volt_AVG;Fuel Remains"));
}

void logData() {
  uint8_t i;
  Serial1.print(float(millis()) / 60000.0, 3); Serial1.print(F(";"));
  for (i = 0; i < TOBD_ID_MAX; i++)
  {
    Serial1.print(OBDDATA[i], 2);
    Serial1.print(F(";"));
  }
  Serial1.print(OBDFLAG[0]);//ASE
  Serial1.print(F(";"));
  Serial1.print(OBDFLAG[3]); //CLOSED LOOP
  Serial1.print(F(";"));
  Serial1.print(OBDFLAG[4]); //Acceleration Enrichment
  Serial1.print(F(";"));
  Serial1.print(OBDFLAG[6]); //IDLE
  Serial1.print(F(";"));
  Serial1.print(OBDFLAG[8]); //NEUTRAL
  Serial1.print(F(";"));
  Serial1.print(OBDFLAG[9]); //OX
  Serial1.print(F(";"));
  Serial1.print(trip_avg_fuel_consumption, 1);
  Serial1.print(F(";"));
  Serial1.print(trip_fuel_consumption, 1);
  Serial1.print(F(";"));
  Serial1.print(total_fuel_consumption, 1);
  Serial1.print(F(";"));
  Serial1.print(ADC.tt_avg * VREF_MEASURED * TT_PIN_DIVIDER / 4095.0, 2);
  Serial1.print(F(";"));
  Serial1.print(TT.current);
  Serial1.print(F(";"));
  Serial1.print(fuel.volt, 2);
  Serial1.print(F(";"));
  Serial1.print(fuel.volt_avg, 2);
  Serial1.print(F(";"));
  Serial1.print(fuel.remains, 2);
  Serial1.println();
  }
