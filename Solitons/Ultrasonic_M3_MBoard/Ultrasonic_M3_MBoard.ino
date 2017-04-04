int speed = 128;

void setup()
{
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);
}

void loop()
{
  if (analogRead(0) < 145) {
    speed = random(0, 255);
    delay(random(analogRead(0), analogRead(0) * 10));
  }  else {
    speed = 0;
  }
  digitalWrite(7, HIGH);
  digitalWrite(8, LOW);

  analogWrite(10, speed);

  digitalWrite(12, HIGH);
  digitalWrite(13, LOW);

  analogWrite(11, speed);
}

