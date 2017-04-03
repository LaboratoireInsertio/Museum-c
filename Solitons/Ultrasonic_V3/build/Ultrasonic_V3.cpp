#include "WProgram.h"
void setup();
void loop();
int speed = 128;

void setup()
{
  pinMode(4, OUTPUT);
}

void loop()
{
  if(analogRead(0) < 145) {
    speed = random(0, 255);
    delay(random(analogRead(0), analogRead(0)*10));
  } 
  else {
    speed = 0;
  }
  analogWrite(4, speed);
}


