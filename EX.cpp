#include "Arduino.h"
#include <Wire.h>

#include "EX.h"

EX::EX(byte i2caddr)
{
  _i2caddr = i2caddr;
  _target = 1;
}

void EX::send1(byte b1)
{
  byte msg[] = {b1};
  sendI2CCommand(msg, 1);
}

void EX::send2(byte b1, byte b2)
{
  byte msg[] = {b1, b2};
  sendI2CCommand(msg, 2);
}

void EX::send4(byte b1, byte b2, byte b3, byte b4)
{
  byte msg[] = {b1, b2, b3, b4};
  sendI2CCommand(msg, 4);
}

void EX::sendI2CCommand(byte msg[], int msgLength)
{
  Wire.beginTransmission(_i2caddr);
  for (short b = 0; b < msgLength; b++) {
    Wire.write(msg[b]);
  }
  Wire.endTransmission();
  delay(100);
}

byte EX::getState(byte loop) {
  send2(0x59, loop);

  Wire.requestFrom(_i2caddr, 1);
  byte s;
  while (Wire.available()) { // peripheral may send less than requested
    s = Wire.read();
  }
  return s;
}

void EX::toggleCommand(byte cmd) 
{
  send4(0x46, cmd, 0x00, 0x00); // initialize at 0 to ensure toggle
  send4(0x46, cmd, 0x00, 0x01);
  send4(0x46, cmd, 0x00, 0x00);
}

void EX::setTarget(byte t)
{
  Serial.println("\nset target ");
  send4(0x46, 0x07, 0x00, t);
  _target = t;
  Serial.println(_target);
}

void EX::setBitDepth()
{
  Serial.println("\nset bitrate/clear loops");
  send4(0x46, 0x59, 0x00, 2);  //change to 32 bit
  send4(0x46, 0x59, 0x00, 2);  //change back to 16 bit
}

void EX::clearTarget() 
{ 
  // clears target loop then sets new target
  Serial.print("\nclear target ");
  Serial.println(_target);
  send1(0x58);
  randomTarget(); 
}

void EX::setRecordGain(short g)
{
  send4(0x46, 8, highByte(g), lowByte(g));
}

void EX::setLoopPan(byte loop, short p)
{
  send4(0x46, 16 + loop, highByte(p), lowByte(p)); 
}

void EX::setLoopGain(byte loop, short g)
{
  send4(0x46, 20 + loop, highByte(g), lowByte(g)); 
}

void EX::setOverdubFade(short f)
{
  send4(0x46, 49, highByte(f), lowByte(f));
}

void EX::randomTarget() 
{
  // sets random loop target
  setTarget(random(1,5));
}

void EX::toggleRecord() 
{ 
  // toggles between recording and not then sets new random target
  Serial.print("\nrecord ");
  Serial.println(_target);
  toggleCommand(56);
  randomTarget(); 
}

void EX::togglePlay() 
{ 
  // toggles between play/pause then sets new random target
  Serial.print("\nplay/pause/mute ");
  Serial.println(_target);
  toggleCommand(57);
  randomTarget(); 
}

void EX::toggleReverse()
{
  //toggles between reverse and forward playback then sets new random target
  Serial.print("\nreverse ");
  Serial.println(_target);
  toggleCommand(58);
  randomTarget(); 
}

void EX::toggleOctave() 
{ 
  // toggles between octave down and normal octave then sets new random target
  Serial.print("\noctave down ");
  Serial.println(_target);
  toggleCommand(62);
  randomTarget(); 
}
