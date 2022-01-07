#include <Wire.h>
#include "EX.h"

// fané (faded)
// firmwave for fanés i2c controller for Disting EX looper

#define EX_ADDRESS 65 // 0x31 hex

EX ex(EX_ADDRESS);

bool gate = false;
int clock = 0;

void setup()
{
  randomSeed(analogRead(A0));
  
  pinMode(13, OUTPUT);
  pinMode(14, INPUT_PULLUP);
  pinMode(15, INPUT_PULLUP);

  Serial.begin(9600);
  Wire.begin();

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
  // clears all loops
  for (byte i=1; i<5; i++) 
  {
    ex.setTarget(i);
    ex.clearTarget();
  }
}

void initializeLooper()
{    
  // clear all loops
  clearAll();
  
  ex.send4(0x46, 8, 0, 1); // record gain 0
  ex.send4(0x46, 17, highByte(-20), lowByte(-20)); // loop 1 pan -20
  ex.send4(0x46, 18, highByte(-10), lowByte(-10)); // loop 2 pan -10
  ex.send4(0x46, 19, 0, 10); // loop 3 pan 10
  ex.send4(0x46, 20, 0, 20); // loop 4 pan 20
  ex.send4(0x46, 21, highByte(-6), lowByte(-6)); // loop 1 gain -6
  ex.send4(0x46, 22, highByte(-12), lowByte(-12)); // loop 2 gain -12
  ex.send4(0x46, 23, highByte(-18), lowByte(-18)); // loop 3 gain -18
  ex.send4(0x46, 24, highByte(-24), lowByte(-24)); // loop 4 gain -24
  ex.send4(0x46, 49, highByte(-6), lowByte(-6)); // overdub fade -6

  ex.randomTarget(); // set random target

}
