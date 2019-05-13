// Wire Master Reader
// by Nicholas Zambetti <http://www.zambetti.com>

// Demonstrates use of the Wire library
// Reads data from an I2C/TWI slave device
// Refer to the "Wire Slave Sender" example for use with this

// Created 29 March 2006

// This example code is in the public domain.


#include <Wire.h>
#include <PCA9685_Servo.h>

void setup() {

  Serial.println("servo_setup");
  servo.begin(&Wire1, 0x40);
  servo.sleep(false);
}

void loop() {

  int8_t v = (uint8_t)(millis()/10);
  servo.set(0, v);
  servo.set(1, v+8, tip ));
  servo.set(2, v+16, tip ));
  servo.set(3, v+24, tip ));
  servo.set(4, v+32, tip ));
  servo.set(5, v+40, tip ));
  servo.set(6, v+48, tip ));
  servo.set(7, v+56, tip ));
  uint8_t byt= servo.update(0,7);

}