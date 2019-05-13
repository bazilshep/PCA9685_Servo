
#include <Wire.h>

#define PCA9685_CHANNELS 16

class PCA9685_Servo {
  public:
    PCA9685_Servo(uint8_t);
  
    //update the specified channels using the data in PositionData
    void update(int8_t *PositionData, uint8_t start, uint8_t count);
	
	//update the specified channels using the data in the internal buffer
	void update(uint8_t start, uint8_t count);
	
	//set the internal buffered  
	void set( uint8_t channel, int8_t position );
	
	//set or clear sleep mode
    void sleep(bool s);
	
	//setup communication using i2c port (comm) and device address (addr)
    void begin( TwoWire *comm, uint8_t addr);
	
  public:
    /*
	Buffer for registers 0x06 to 0x45
	aka LED0_ON_L to LED15_OFF_H
	*/
    uint8_t _buff[PCA9685_CHANNELS*4];
	uint8_t _i2caddr;
	TwoWire *_wireInst;

};