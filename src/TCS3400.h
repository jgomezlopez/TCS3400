/*
 * tcs3400.h
 *
 * Header file for the TCS3400 Arduino library.
 *
 * Author: Sean Caulfield <sean@yak.net>
 * License: GPL v3.0
 * 
 */

#ifndef _TCS3400_H
#define _TCS3400_H

#include <Arduino.h>

// Four different versions of the TCS3400 are available, with two different i2c
// addresses:
//  
//   TCS34001 = addr 0x39, VCC i2c pullup
//   TCS34003 = addr 0x39, 1.8V i2c pullup
//   TCS34005 = addr 0x29, VCC i2c pullup
//   TCS34007 = addr 0x29, 1.8V i2c pullup
//

const uint8_t TCS3400_I2C_ADDR_13 = 0x39;
const uint8_t TCS3400_I2C_ADDR_57 = 0x39;

const uint8_t TCS3400_REG_ENABLE = 0x80;
const uint8_t TCS3400_REG_ATIME = 0x81;
const uint8_t TCS3400_REG_CONTROL = 0x8f;
const uint8_t TCS3400_REG_ID = 0x92;
const uint8_t TCS3400_REG_STATUS = 0x93;
const uint8_t TCS3400_REG_C_L = 0x94;
const uint8_t TCS3400_REG_C_H = 0x95;
const uint8_t TCS3400_REG_R_L = 0x96;
const uint8_t TCS3400_REG_R_H = 0x97;
const uint8_t TCS3400_REG_G_L = 0x98;
const uint8_t TCS3400_REG_G_H = 0x99;
const uint8_t TCS3400_REG_B_L = 0x9a;
const uint8_t TCS3400_REG_B_H = 0x9b;
const uint8_t TCS3400_REG_IR = 0xC0;

const uint8_t TCS3400_ID_13 = 0b10010000;
const uint8_t TCS3400_ID_57 = 0b10010011;

const uint8_t TCS3400_EN_POW_ON = 0x01;
const uint8_t TCS3400_EN_ADC_ON = 0x02;
const uint8_t TCS3400_STATUS_AVALID = 0x01;
const uint8_t TCS3400_IR_ENABLED = 0x80;
const uint8_t TCS3400_IR_DISABLED = 0x00;

class TCS3400 {
  public:

    enum integrationTime {
      INTG_TIME_1_CYCLES = 0xFF,
      INTG_TIME_10_CYCLES = 0xF6,
      INTG_TIME_37_CYCLES = 0xDB,
      INTG_TIME_64_CYCLES = 0xC0,
      INTG_TIME_256_CYCLES = 0x00,
    };

    enum gain {
      GAIN_1X = 0x00,
      GAIN_4X = 0x01,
      GAIN_16X = 0x02,
      GAIN_64X = 0x03,
    };

	const float ms_cycle = 2.78;
    TCS3400(void);
    bool begin(uint8_t i2c_addr=TCS3400_I2C_ADDR_13);

    uint16_t getRed(void);
    uint16_t getGreen(void);
    uint16_t getBlue(void);
    uint16_t getVisible(void);
    uint16_t getIR(void);

    uint16_t getOneShotRed(void);
    uint16_t getOneShotGreen(void);
    uint16_t getOneShotBlue(void);
    uint16_t getOneShotVisible(void);
    uint16_t getOneShotIR(void);


    void setIntegrationTime(enum integrationTime);
    void setGain(enum gain);

    void setClearModeVisible(void);
    void setClearModeIR(void);
    void enableVisible(void);
    void enableIR(void);
    void enable(void);
    void disable(void);
	

  protected:
    bool clear_is_visible = true;
    uint16_t chan_r = 0;
    uint16_t chan_g = 0;
    uint16_t chan_b = 0;
    uint16_t chan_ir = 0;
    uint16_t chan_vis = 0;
    uint8_t addr = 0;
	enum integrationTime integration=INTG_TIME_1_CYCLES;

    void setPowerOn(void);
    void setPowerOff(void);
    void setADCOn(void);
    void setADCOff(void);
    bool isDataValid(void);
    uint8_t getDeviceId(void);

    uint8_t read_byte(uint8_t reg);
    void write_byte(uint8_t reg, uint8_t value);
    uint16_t read_word(uint8_t reg);
	uint16_t bytes_combine(uint8_t, uint8_t);

};

#endif
