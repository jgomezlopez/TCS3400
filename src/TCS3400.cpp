/*
 * tcs3400.cpp
 *
 * Implementation for the TCS3400 Arduino library.
 *
 * Author: Sean Caulfield <sean@yak.net>
 * License: GPL v3.0
 * 
 */

#include <Arduino.h>
#include <Wire.h>
#include <TCS3400.h>

TCS3400::TCS3400() {
}

bool TCS3400::begin(uint8_t i2c_addr) {

  Wire.begin();
  this->addr = i2c_addr;

  // Enable device 
  this->setPowerOn();
  
  // Verify we're talking to the right device
  uint8_t devid = this->getDeviceId();
  if (devid != TCS3400_ID_13 && devid != TCS3400_ID_57) {
    return false;
  }

  // Switch ADC on, set integration time, and gain to defaults
  this->setADCOn();
  this->setIntegrationTime(INTG_TIME_1_CYCLES);
  this->setGain(GAIN_1X);

  return true;

}

uint16_t TCS3400::getRed() {
  //while (!this->isDataValid()) ;
  return this->read_word(TCS3400_REG_R_L);
}

uint16_t TCS3400::getGreen() {
  //while (!this->isDataValid()) ;
  return this->read_word(TCS3400_REG_G_L);
}

uint16_t TCS3400::getBlue() {
  //while (!this->isDataValid())
    //;
  return this->read_word(TCS3400_REG_B_L);
}

uint16_t TCS3400::getVisible() {
  this->setClearModeVisible();
  //while (!this->isDataValid()) ;
  return this->read_word(TCS3400_REG_C_L);
}

uint16_t TCS3400::getIR() {
  this->setClearModeIR();
  //while (!this->isDataValid()) ;
  return this->read_word(TCS3400_REG_C_L);
}

bool TCS3400::isDataValid() {
  return TCS3400_STATUS_AVALID & this->read_byte(TCS3400_REG_STATUS);
}

uint8_t TCS3400::getDeviceId(void) {
  return this->read_byte(TCS3400_REG_ID);
}

void TCS3400::setIntegrationTime(enum integrationTime it) {
  this->write_byte(TCS3400_REG_ATIME, (uint8_t)it);
}

void TCS3400::setGain(enum gain g) {
  this->write_byte(TCS3400_REG_ATIME, (uint8_t)g);
}

void TCS3400::setClearModeVisible(void) {
  this->write_byte(TCS3400_REG_IR, TCS3400_IR_DISABLED);
}

void TCS3400::setClearModeIR(void) {
  this->write_byte(TCS3400_REG_IR, TCS3400_IR_ENABLED);
}

void TCS3400::setPowerOn() {
  this->write_byte(TCS3400_REG_ENABLE, TCS3400_EN_POW_ON);
}

void TCS3400::setPowerOff() {
  this->write_byte(TCS3400_REG_ENABLE, (uint8_t)0);
}

void TCS3400::setADCOn() {
  this->write_byte(TCS3400_REG_ENABLE, TCS3400_EN_POW_ON|TCS3400_EN_ADC_ON);
}

void TCS3400::setADCOff() {
  this->write_byte(TCS3400_REG_ENABLE, TCS3400_EN_POW_ON);
}

// Read a byte over i2c
uint8_t TCS3400::read_byte(uint8_t reg) {
  uint8_t value;

  // Write device address and register requested to the bus
  Wire.beginTransmission(this->addr);
  Wire.write(reg);
  Wire.endTransmission();

  // Now read byte back from device
  Wire.requestFrom(this->addr, (uint8_t)1);
  value = Wire.read();

  return value;
}

// Send a byte over i2c
void TCS3400::write_byte(uint8_t reg, uint8_t value) {

  // Write device address, register, and given value to bus
  Wire.beginTransmission(this->addr);
  Wire.write(reg);
  Wire.write(value);
  Wire.endTransmission();

}

// Read two bytes over i2c
uint16_t TCS3400::read_word(uint8_t reg) {
  uint16_t value = 0;

  // Write device address and register requested to the bus
  Wire.beginTransmission(this->addr);
  Wire.write(reg);
  Wire.endTransmission();

  // Ask nicely
  Wire.requestFrom(this->addr, (uint8_t)2);

  // Wait for data to be avaiable
  while (!Wire.available()) {
  }

  value = Wire.read();       // First byte is least-significant
  value |= Wire.read() << 8;

  return value;
}
