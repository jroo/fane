#ifndef EX_h
#define EX_h

#include "Arduino.h"
#include <Wire.h>

class EX
{
  public:
    EX(byte i2caddr);
    void sendCommand(short addr, byte msg[], int msgLength);
    byte getState(byte loop);
    void clearTarget();
    void randomTarget();
    void toggleCommand(byte cmd);
    void toggleRecord();
    void togglePlay();
    void toggleReverse();
    void toggleOctave();
  private:
    byte _i2caddr;
    byte _target;
};

#endif
