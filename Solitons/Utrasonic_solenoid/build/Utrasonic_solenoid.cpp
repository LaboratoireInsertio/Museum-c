#include "WProgram.h"
void setup();
void loop();
void setup()
{
  pinMode(4, OUTPUT);
}

void loop()
{
  if(analogRead(0) < 145) {
    digitalWrite(4, HIGH);
    delay(10);
    digitalWrite(4, LOW);
    delay(random(analogRead(0), analogRead(0)*10));
  }
}


