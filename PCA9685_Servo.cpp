
#include <Wire.h>
#include "PCA9685_Servo.h"

// Target update frequency
#ifndef PCA9685_UPDATERATE
  #define PCA9685_UPDATERATE 40
#endif

//internal clock rate
#define PCA9685_CLOCK 25000000L

// Prescaler to get target freq
#define PCA9685_PRESCALER (PCA9685_CLOCK/(4096l*PCA9685_UPDATERATE)-1)

// LED_OFF value to so that when (LED_ON=127) pulse width = 1.5ms
#define PCA9685_LED_OFF 400

//PCA9685_CLOCK/(PCA9685_UPDATERATE*667l)+127  //this should be the formula...

PCA9685_Servo::PCA9685_Servo(uint8_t ignore) {
	
  /* 
  Hold the LEDn_OFF constant and vary the on time.
  Init the buffer to the correct LEDn_OFF values
  */
  //for (uint8_t i=0; i<PCA9685_CHANNELS; i=i)
  //  ((uint16_t*) _buff)[i*2+1] = PCA9685_LED_OFF;
	
  /*
  Init the LEDn_ON to center
  */
  //for (uint8_t i=0; i<PCA9685_CHANNELS; i=i)
  //  ((uint16_t*) _buff)[i*2] = 127;

  uint8_t reg[4];
  reg[0] = 127;
  reg[1] = 0;
  reg[2] = PCA9685_LED_OFF & 0xFF;
  reg[3] = (PCA9685_LED_OFF & 0xFF00)>>8;
  
  for (uint8_t b=0; b<PCA9685_CHANNELS*4; b++ )
    _buff[b] = reg[b & 3];
  
}

void PCA9685_Servo::update(int8_t *PositionData, uint8_t start, uint8_t count) {
  if ((count+start) >= PCA9685_CHANNELS )
	  count = PCA9685_CHANNELS-start;
  for (uint8_t i=0; i<count; i++)
    _buff[(i+start)*4] = PositionData[i]+127;
	
  update(start, count);
}

void PCA9685_Servo::update(uint8_t start, uint8_t count) {
  if ((count+start) >= PCA9685_CHANNELS )
    count = PCA9685_CHANNELS-start;
	
	
  uint8_t *stop= _buff + (start + count)*4;
  uint8_t t=0;
  _wireInst->beginTransmission(_i2caddr);
  _wireInst->write(0x06+start*4); //LEDn_ON_L address for n=start
  for (uint8_t *ptr = _buff+start*4; ptr<stop; ptr++) {
    if (t >= 28) {
      _wireInst->endTransmission(); //hit send buffer limit, break here.
      t=0;

      _wireInst->beginTransmission(_i2caddr);  //continue
      _wireInst->write(0x06+(ptr-_buff));
    }
    _wireInst->write(*ptr);
	t++;
  }

}

void PCA9685_Servo::set( uint8_t channel, int8_t position ){
 if (channel >= PCA9685_CHANNELS)
    return;
	
  _buff[(channel)*4] = position+127;
}

void PCA9685_Servo::sleep(bool s)
{
	
  _wireInst->beginTransmission(_i2caddr);
  _wireInst->write(0x00); // MODE1 register address
  if (s)
    _wireInst->write(0x30); // set MODE1 = 110000xb = SLEEP and AI bit high
  else 
    _wireInst->write(0x20); // set MODE1 = 100000xb =  AI bit high
  _wireInst->endTransmission();

}

void PCA9685_Servo::begin( TwoWire *comm, uint8_t addr) {
	
	_wireInst = comm;
	_i2caddr = addr;
	
	sleep(true); //Set sleep and AI bit
	
	_wireInst->beginTransmission(_i2caddr);
	_wireInst->write(0xFE); // PRE_SCALE register address
	// set PRE_SCALE value
	_wireInst->write(PCA9685_PRESCALER);
	_wireInst->endTransmission();
	
}