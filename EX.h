#ifndef EX_h
#define EX_h

#include "Arduino.h"
#include <Wire.h>

class EX
{
  public:
    EX(byte i2caddr);
    void sendI2CCommand(byte msg[], int msgLength);
    void send1(byte b1);
    void send2(byte b1, byte b2);
    void send4(byte b1, byte b2, byte b3, byte b4);

    byte getState(byte loop);
    void clearTarget();
    void setBitDepth();
    void setRecordGain(short g);
    void setLoopPan(byte loop, short p);
    void setLoopGain(byte loop, short g);
    void setOverdubFade(short f);
    void setTarget(byte t);
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
