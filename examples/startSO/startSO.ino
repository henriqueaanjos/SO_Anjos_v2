#include <Bounce2.h>
#include <SoftWire.h>
#include <NewTone.h>
#include <OneButton.h>
#include <XPT2046_Touchscreen.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include "SOAnjos.h"

SOAnjos so = SOAnjos();

void _program(){
  if(so.startProgram){
      if(!so.setupProgram){
          //( setup //)
          so.setupProgram = 1;
      }
      so.setMotorPin(M1, 100);
      // loop
  }
}

//////////////////////////////////////////////////////// SETUP ////////////////////////////////////////////////////////
void setup(){
  so.initSetup();
}

//////////////////////////////////////////////////////// LOOP ////////////////////////////////////////////////////////
void loop(){
  so.updateLoop();
  _program();
}