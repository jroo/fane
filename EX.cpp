#include "Arduino.h"
#include <Wire.h>

#include "EX.h"

EX::EX(byte i2caddr)
{
  _i2caddr = i2caddr;
  _target = 1;
}

void EX::sendCommand(short addr, byte msg[], int msgLength)
{
  Wire.beginTransmission(addr);
  Serial.print(millis());
  Serial.println(" transmission started");
  for (short b = 0; b < msgLength; b++) {
    Wire.write(msg[b]);
  }
  Serial.print("transmission ended with status of ");
  Serial.println(Wire.endTransmission());
  delay(100);
  Serial.println();
}

byte EX::getState(byte loop) {
  byte msg[] = {0x59, loop};
  sendCommand(_i2caddr, msg, sizeof(msg));

  Wire.requestFrom(_i2caddr, 1);
  Serial.print("Loop ");
  Serial.print(loop);
  Serial.print(" current State: ");
  byte s;
  while (Wire.available()) { // peripheral may send less than requested
    s = Wire.read();
  }
  Serial.println(s, DEC);
  return s;
}

void EX::toggleCommand(byte cmd) 
{
  byte msg1[] = {0x46, cmd, 0x00, 0x00};
  byte msg2[] = {0x46, cmd, 0x00, 0x01};
  sendCommand(_i2caddr, msg1, 4); // initialize at 0 to ensure toggle
  sendCommand(_i2caddr, msg2, 4);
  sendCommand(_i2caddr, msg1, 4); 
}

void EX::clearTarget() 
{ 
  // clears target loop then sets new target
  sendCommand(_i2caddr, {0x58}, 1);
  randomTarget(); 
}

void EX::randomTarget() {
  Serial.println("random target");
  byte loop = random(1,5);
  _target = loop;
  byte msg[] = {0x46, 0x07, 0x00, loop};
  sendCommand(_i2caddr, msg, sizeof(msg));
}

void EX::toggleRecord() 
{ 
  // toggles between recording and not then sets new random target
  Serial.println("record");
  toggleCommand(56);
  randomTarget(); 
}


void EX::togglePlay() 
{ 
  // toggles between play/pause then sets new random target
  Serial.println("play/pause/mute");
  toggleCommand(57);
  randomTarget(); 
}

void EX::toggleReverse()
{
  //toggles between reverse and forward playback then sets new random target
  Serial.println("reverse");
  toggleCommand(58);
  randomTarget(); 
}

void EX::toggleOctave() 
{ 
  // toggles between octave down and normal octave then sets new random target
  Serial.println("octave down");
  toggleCommand(62);
  randomTarget(); 
}
