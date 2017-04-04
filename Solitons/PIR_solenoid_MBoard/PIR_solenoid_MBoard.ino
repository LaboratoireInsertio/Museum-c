int speed = 128;

void setup()
{
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);
  pinMode(5, INPUT);
}

void loop()
{
  if (digitalRead(5) == 1) {
    digitalWrite(7, HIGH);
    digitalWrite(8, LOW);

    digitalWrite(12, HIGH);
    digitalWrite(13, LOW);

    digitalWrite(10, HIGH);
    digitalWrite(11, HIGH);;
    delay(10);
    digitalWrite(10, LOW);
    digitalWrite(11, LOW);
    delay(random(0, 1500));
  }
}

