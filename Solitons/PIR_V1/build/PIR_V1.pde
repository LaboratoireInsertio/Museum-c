int speed = 128;

void setup()
{
  pinMode(4, OUTPUT);
  pinMode(5, INPUT);
}

void loop()
{
  if(digitalRead(5) == 1) {
    speed = random(0, 255);
    delay(random(0, 1500));
  } 
  else {
    speed = 0;
  }
  analogWrite(4, speed);
}

