//idea from
//https://github.com/SV-Zanshin/MB85_FRAM/blob/master/MB85_FRAM.h
//https://github.com/sosandroid/FRAM_MB85RC_I2C


#include <stdlib.h>
#include <Wire.h>
#include "FRAM24CXX.h"

FRAM24CXX::FRAM24CXX(uint8_t address, uint16_t chipDensity)
{
  i2c_addr = address;
  density = chipDensity;
  switch (density) {
    case 4:
      maxaddress = MAXADDRESS_04;
      break;
    case 16:
      maxaddress = MAXADDRESS_16;
      break;
    default:
      maxaddress = 0; /* means error */
      break;
  }
}

void FRAM24CXX::I2CAddressAdapt(uint16_t framAddr) {
  switch (density) {
    case 4:
      chipaddress = (i2c_addr | ((framAddr >> 8) & 0x1));
      break;
    case 16:
      chipaddress = (i2c_addr | ((framAddr >> 8) & 0x7));
      break;
    default:
      chipaddress = i2c_addr;
      break;
  }

#if defined(SERIAL_DEBUG) && (SERIAL_DEBUG == 1)
  Serial.print("Calculated address 0x");
  Serial.println(chipaddress, HEX);
#endif
  
  /*
  if (density < 64) {
    Wire.beginTransmission(chipaddress);
    Wire.write(framAddr & 0xFF);
  }
  else {
    Wire.beginTransmission(chipaddress);
    Wire.write(framAddr >> 8);
    Wire.write(framAddr & 0xFF);
  }*/
  return;
}


void FRAM24CXX::begin(void) {
	 
#if defined(SERIAL_DEBUG) && (SERIAL_DEBUG == 1)
    if (Serial) {
    Serial.println("FRAM_FM24XX_I2C object created");
    Serial.print("I2C device address 0x");
    Serial.println(i2c_addr, HEX);
    Serial.println("...... ...... ......");
  }
#endif

  return;
}




