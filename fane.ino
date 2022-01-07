#include <Wire.h>
#include "EX.h"

// fané (faded)
// firmwave for fanés i2c controller for Disting EX looper

#define EX_ADDRESS 66 // 0x31 hex

EX ex(EX_ADDRESS);

bool gate = false;
int clock = 0;

void setup() {
  randomSeed(analogRead(A0));
  
  pinMode(13, OUTPUT);
  pinMode(14, INPUT_PULLUP);
  pinMode(15, INPUT_PULLUP);

  Serial.begin(9600);
  Wire.begin();

  ex.randomTarget();
}

void loop() {
  digitalWrite(13, HIGH);
  digitalWrite(13, LOW);
  //delay(0.5);

  // with this circuit gates are open when digitalRead is false 
  if (gate and (digitalRead(14) == HIGH)) {
    gate = false; // gate closed
  }
  if (!gate and (digitalRead(14) == LOW)) {
    gate = true; // gate opened
    triggerEvent();
  }
}

void triggerEvent() {
  clock = (clock + 1) % 16000;
  if (random(300) == 42) ex.clearTarget();
  if (random(100) == 24) ex.toggleReverse();
  if (random(33) == 11) ex.toggleRecord();
  if (random(500) == 19) ex.toggleOctave();
}
