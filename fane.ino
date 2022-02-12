#include <Wire.h>
#include "EX.h"

// fané (faded)
// firmwave for fanés i2c controller for Disting EX looper

#define EX_I2C_ENABLED true
#define EX_I2C_ADDRESS 65 // 0x41 hex
#define EX_MIDI_ENABLED true
#define EX_MIDI_CHANNEL 1

EX ex(EX_I2C_ENABLED, EX_I2C_ADDRESS, EX_MIDI_ENABLED, EX_MIDI_CHANNEL);

bool gate = false;
int lt = 0; // last trigger time
int bpm = 90;
int m = (int)((60.0/(float)bpm)*1000);

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

  Serial.println(analogRead(14));
  
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
  lt = millis();
  endRecordings();
  
  if (random(300) == 42) 
  {
    ex.clearTarget();
    ex.randomTarget();
  }
  
  if (random(100) == 24)
  {
    ex.toggleReverse();
    ex.randomTarget();
  }
  
  if (random(33) == 11) 
  {
    ex.toggleRecord();
    ex.randomTarget();
  }
  
  if (random(500) == 19)
  { 
    ex.toggleOctave();
    ex.randomTarget();
  }
}

void initializeLooper()
{    
  ex.clearAll();
  ex.setRecordGain(0);
  ex.setLoopPan(1, -20);
  ex.setLoopPan(2, -10);
  ex.setLoopPan(3, 10);
  ex.setLoopPan(4, 20);
  ex.setLoopGain(1, -3);
  ex.setLoopGain(2, -6);
  ex.setLoopGain(3, -9);
  ex.setLoopGain(4, -12);
  ex.setOverdubFade(-50);
  ex.randomTarget();
}

void endRecordings() 
{
  // for each loop if recording and if millis() > end stop recording
  Serial.println(millis());
  for (int i=0; i<4; i++) {
    Serial.print (ex.recStatus[i]);
    Serial.print (" : ");
    Serial.println (ex.recEnd[i]);
    if (ex.recStatus[i] == true && millis() > ex.recEnd[i]) {
      ex.setTarget(i + 1);
      ex.toggleRecord();
    }
  }
  Serial.println();
}
