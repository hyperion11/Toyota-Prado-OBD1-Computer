//idea from
//https://github.com/SV-Zanshin/MB85_FRAM/blob/master/MB85_FRAM.h
//https://github.com/sosandroid/FRAM_MB85RC_I2C


#ifndef _FRAM24CXX_h_
#define _FRAM24CXX_h_

#if ARDUINO >= 100
#include <Arduino.h>
#else
#include <WProgram.h>
#endif



#define SERIAL_DEBUG 0

#include <Wire.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#define MAXADDRESS_04 512
#define MAXADDRESS_16 2048
#define ERROR_11 11 // Memory address out of range
#define OK_1 1 // Memory address out of range
class FRAM24CXX {
  public:
    FRAM24CXX (uint8_t address, uint16_t chipDensity);
    void  begin(void);

    template<typename T>uint8_t write(const uint16_t framAddr, const T &value) {
      const uint8_t* bytePtr = (const uint8_t*)&value;
      if ((framAddr >= maxaddress) || ((framAddr + sizeof(T) - 1) >= maxaddress)) return ERROR_11;
      I2CAddressAdapt(framAddr);
      Wire.beginTransmission(chipaddress);
      Wire.write(framAddr & 0xFF);
      for (uint8_t i = 0; i < sizeof(T); i++)
        Wire.write(*bytePtr++);
      Wire.endTransmission();
	  return OK_1;
    }



    template< typename T > uint8_t read(const uint16_t framAddr, T &value) {
      uint8_t* bytePtr      = (uint8_t*)&value;
      if ((framAddr >= maxaddress) || ((framAddr + sizeof(T) - 1) >= maxaddress)) return ERROR_11;
      I2CAddressAdapt(framAddr);
      Wire.beginTransmission(chipaddress);
      Wire.write(framAddr & 0xFF);
      Wire.endTransmission();
      Wire.requestFrom(chipaddress,  sizeof(T));
      for (uint8_t i = 0; i < sizeof(T); i++)
        if (Wire.available()) *bytePtr++ = Wire.read();
	return OK_1;
    }

  private:
    uint8_t  i2c_addr;
    uint16_t  density;
    uint16_t  maxaddress;
    uint16_t chipaddress;
    void  I2CAddressAdapt(uint16_t framAddr);
};


#endif
