#ifndef EX_h
#define EX_h

#include "Arduino.h"
#include <Wire.h>

class EX
{
  public:
    EX(bool i2cEnabled, byte i2cAddr, bool midiEnabled, byte midiChannel);
    byte getState(byte loop);
    void clearTarget();
    void clearAll();
    void printStatus();
    void setBitDepth();
    void setRecordGain(short g);
    void setLoopPan(byte loop, short p);
    void setLoopGain(byte loop, short g);
    void setOverdubFade(short f);
    void setTarget(byte t);
    void randomTarget();
    void toggleCommand(byte cmd);
    void toggleCC(byte cc);
    void toggleRecord();
    void togglePlay();
    void toggleReverse();
    void toggleOctave();
    long recStatus[4];
    long recEnd[4];
  private:
    bool _i2cEnabled;
    byte _i2cAddr;
    bool _midiEnabled;
    byte _midiChannel;
    byte _target;
    void sendI2CCommand(byte msg[], int msgLength);
    void send1(byte b1);
    void send2(byte b1, byte b2);
    void send4(byte b1, byte b2, byte b3, byte b4);
    void triggerNote(byte note);
    int getCCVal(int low, int high, int cval);
};

#endif
