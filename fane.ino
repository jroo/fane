#include <Wire.h>
#include "EX.h"

// fané (faded)
// firmwave for fanés i2c controller for Disting EX looper

#define EX_I2C_ENABLED true
#define EX_I2C_ADDRESS 65 // 0x31 hex
#define EX_MIDI_ENABLED false
#define EX_MIDI_CHANNEL 1

EX ex(EX_I2C_ENABLED, EX_I2C_ADDRESS, EX_MIDI_ENABLED, EX_MIDI_CHANNEL);

bool gate = false;
int clock = 0;

void setup()
{  
  pinMode(13, OUTPUT);
  pinMode(14, INPUT_PULLUP);
  pinMode(15, INPUT_PULLUP);

  Serial.begin(115200);
  randomSeed(analogRead(2));
  Wire.begin();

  ex.printStatus();
  initializeLooper();
}

void loop()
{
  digitalWrite(13, HIGH);
  digitalWrite(13, LOW);

  // with this circuit gates are open when digitalRead is false 
  if (gate and (digitalRead(14) == HIGH)) {
    gate = false; // gate closed
  }
  if (!gate and (digitalRead(14) == LOW)) {
    gate = true; // gate opened
    triggerEvent();
  }
}

void triggerEvent()
{
  Serial.print('.');
  clock = (clock + 1) % 16000;
  if (random(300) == 42) ex.clearTarget();
  if (random(100) == 24) ex.toggleReverse();
  if (random(33) == 11) ex.toggleRecord();
  if (random(500) == 19) ex.toggleOctave();
}

void clearAll() 
{
  // clears all loops by setting bitrate
  ex.setBitDepth();
}

void initializeLooper()
{    
  clearAll();
  ex.setRecordGain(0);
  ex.setLoopPan(1, -20);
  ex.setLoopPan(2, -10);
  ex.setLoopPan(3, 10);
  ex.setLoopPan(4, 20);
  ex.setLoopGain(1, -6);
  ex.setLoopGain(2, -12);
  ex.setLoopGain(3, -18);
  ex.setLoopGain(4, -24);
  ex.setOverdubFade(-6);
  ex.randomTarget();
}
