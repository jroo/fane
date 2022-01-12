#include "Arduino.h"
#include <Wire.h>

#include "EX.h"

EX::EX(bool i2cEnabled, byte i2cAddr, bool midiEnabled, byte midiChannel)
{
  _i2cEnabled = i2cEnabled;
  _i2cAddr = i2cAddr;
  _midiEnabled = midiEnabled;
  _midiChannel = midiChannel;
  _target = 1;

  for (int i=0; i<4; i++) {
    recStatus[i] = false;
    recEnd[i] = false;
  }
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
  Wire.beginTransmission(_i2cAddr);
  for (short b = 0; b < msgLength; b++) {
    Wire.write(msg[b]);
  }
  Wire.endTransmission();
  delay(100);
}

byte EX::getState(byte loop) {
  send2(0x59, loop);

  Wire.requestFrom(_i2cAddr, 1);
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

void EX::toggleCC(byte cc) 
{
  usbMIDI.sendControlChange(cc, 0, _midiChannel);
  delay(30);
  usbMIDI.sendControlChange(cc, 127, _midiChannel);
  delay(30);
  usbMIDI.sendControlChange(cc, 0, _midiChannel);
}

void EX::setTarget(byte t)
{
  Serial.print("\nset target ");
  Serial.println(t);
  if (_i2cEnabled) send4(0x46, 0x07, 0x00, t);
  if (_midiEnabled) {
    byte trigNote[] = {48, 50, 52, 53};
    triggerNote(trigNote[t-1]);
  }
  _target = t;
}

void EX::setBitDepth()
{
  // flip bit depth from 32 to 16 to force clearing all loops
  Serial.println("\nset bitrate / clear loops");
  if (_i2cEnabled) {
    send4(0x46, 0x59, 0x00, 2);
    send4(0x46, 0x59, 0x00, 1);
  }
  if (_midiEnabled) {
    usbMIDI.sendControlChange(59, 127, _midiChannel);
    delay(30);
    usbMIDI.sendControlChange(59, 64, _midiChannel);
  }
}

void EX::clearTarget() 
{ 
  // clears target loop then sets new target
  Serial.print("\nclear target ");
  Serial.println(_target);
  if (_i2cEnabled) send1(0x58);
  randomTarget(); 
}

void EX::setRecordGain(short g)
{
  if (_i2cEnabled) send4(0x46, 8, highByte(g), lowByte(g));
  if (_midiEnabled) usbMIDI.sendControlChange(8, 111, _midiChannel);
}

void EX::setLoopPan(byte loop, short p)
{
  if (_i2cEnabled) send4(0x46, 16 + loop, highByte(p), lowByte(p)); 
  if (_midiEnabled) usbMIDI.sendControlChange(16 + loop, getCCVal(-100, 100, p), _midiChannel);
}

void EX::setLoopGain(byte loop, short g)
{
  if (_i2cEnabled) send4(0x46, 20 + loop, highByte(g), lowByte(g)); 
  if (_midiEnabled) usbMIDI.sendControlChange(20 + loop, getCCVal(-40, 6, g), _midiChannel);
}

void EX::setOverdubFade(short f)
{
  if (_i2cEnabled) send4(0x46, 49, highByte(f), lowByte(f));
  if (_midiEnabled) usbMIDI.sendControlChange(49, 125, _midiChannel);
}

void EX::randomTarget() 
{
  // sets random loop target
  setTarget(random(1,5));
}

void EX::toggleRecord() 
{ 
  // toggles between recording and not then sets new random target
  Serial.print("\ntoggle record ");
  Serial.println(_target);
  if (_i2cEnabled) toggleCommand(56);
  if (_midiEnabled) triggerNote(55);
  byte i = _target - 1;
  recStatus[i] = !recStatus[i];
  Serial.println(recStatus[i]);
  if (recStatus[i] == true) recEnd[i] = millis() + random(2000, 19000);
  randomTarget(); 
}

void EX::togglePlay() 
{ 
  // toggles between play/pause then sets new random target
  Serial.print("\nplay/pause/mute ");
  Serial.println(_target);
  if (_i2cEnabled) toggleCommand(57);
  if (_midiEnabled) triggerNote(57);
  randomTarget(); 
}

void EX::toggleReverse()
{
  //toggles between reverse and forward playback then sets new random target
  Serial.print("\nreverse ");
  Serial.println(_target);
  if (_i2cEnabled) toggleCommand(58);
  if (_midiEnabled) triggerNote(59);
  randomTarget(); 
}

void EX::toggleOctave() 
{ 
  // toggles between octave down and normal octave then sets new random target
  Serial.print("\noctave down ");
  Serial.println(_target);
  if (_i2cEnabled) toggleCommand(62);
  if (_midiEnabled) triggerNote(60);
  randomTarget(); 
}

void EX::triggerNote(byte note) 
{
  usbMIDI.sendNoteOn(note, 127, _midiChannel);
  delay(100);
  usbMIDI.sendNoteOff(note, 127, _midiChannel);
}

void EX::printStatus()
{
  Serial.println("EX Status:");
  Serial.print("MIDI Enabled: ");
  Serial.println(_midiEnabled);
  Serial.print("MIDI Channel: ");
  Serial.println(_midiChannel);
  Serial.print("I2C Enabled: ");
  Serial.println(_i2cEnabled);
  Serial.print("I2C Address: ");
  Serial.println(_i2cAddr);
}

int EX::getCCVal(int low, int high, int cval) 
{
  int span = abs(low - high) + 1;
  Serial.print("span: ");
  Serial.println(span);
  float stepSize = (float)128/(float)span;
  Serial.print("step size: ");
  Serial.println(stepSize);
  int adjVal = cval - low;
  Serial.print("adjval: ");
  Serial.println(adjVal);
  Serial.println(int(adjVal * stepSize));
  Serial.println();
  return int(adjVal * stepSize);
}
