#include <EncButton.h>
#include <GTimer.h>
GTimer<millis> tmr1;


const int NUM_SLIDERS = 5;

const int encoderPinsDT[NUM_SLIDERS] = { 7, 3, 4, 5, 6 };
const int encoderPinsCLK[NUM_SLIDERS] = { 13, 9, 10, 11, 12 };
const int buttonPinsSW[NUM_SLIDERS] = { A1, A2, A3, A4, A5 };


EncButton controls[NUM_SLIDERS];


byte volumeStates[NUM_SLIDERS];

#include <EEManager.h>
EEManager memory(volumeStates, 10000);


void setup() {
  Serial.begin(9600);

  byte stat = memory.begin(0, 'b');


  tmr1.setMode(GTMode::Interval);
  tmr1.setTime(50);
  tmr1.start();
  for (int i = 0; i < NUM_SLIDERS; i++) {
    controls[i].init(encoderPinsCLK[i], encoderPinsDT[i], buttonPinsSW[i], INPUT_PULLUP);
    if (stat == 1) volumeStates[i] = 0;
  }
}

void loop() {
  for (int i = 0; i < NUM_SLIDERS; i++) {
    controls[i].tick();
    if (controls[i].turn()) {
      int var = 0;
      var += (controls[i].fast() ? 5 : 1) * controls[i].dir();
      volumeStates[i] = constrain(volumeStates[i] + var, 0, 100);
      memory.update();
    }
  }
  memory.tick();
  if (tmr1) sendSliderValues();
}



void sendSliderValues() {
  String builtString = String("");

  for (int i = 0; i < NUM_SLIDERS; i++) {
    builtString += String((int)map(volumeStates[i], 0, 100, 0, 1023));

    if (i < NUM_SLIDERS - 1) {
      builtString += String("|");
    }
  }

  Serial.println(builtString);
}
