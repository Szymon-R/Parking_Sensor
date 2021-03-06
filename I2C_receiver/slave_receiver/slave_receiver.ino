// Wire Slave Receiver
// by Nicholas Zambetti <http://www.zambetti.com>

// Demonstrates use of the Wire library
// Receives data as an I2C/TWI slave device
// Refer to the "Wire Master Writer" example for use with this

// Created 29 March 2006

// This example code is in the public domain.


#include <Wire.h>

void setup() {
  Wire.begin(4);                // join i2c bus with address #8
  Wire.onReceive(receiveEvent); // register event
  Serial.begin(9600);           // start serial for output
}

void loop() {
  delay(100);
}
union Buffer
{
  unsigned char array[4];
  float number;
};

// function that executes whenever data is received from master
// this function is registered as an event, see setup()
void receiveEvent(int howMany) 
{
  Buffer b1;
  if( Wire.available() == 4) 
  { // loop through all but the last
    for(int i=0; i<4; ++i)
    {
      b1.array[i] = Wire.read();
    }
  }
  //int x = Wire.read();    // receive byte as an integer
  Serial.println(b1.number);
}
