#include <Servo.h>
#include <Slip.h>

// CONTROL TWO SERVOS WITH SLIP
// SERVO A IS ON PIN 2
// SERVO B IS ON PIN 3
// SEND TWO bytes TO CONTROL THE SERVOS
// THE FIRST BYTE IS EITHER 0 OR 1 (SERVO A OR B)
// THE SECOND BYTE IS THE ANGLE

// THIS IMPLEMENTATION IS BASED ON :
// https://en.wikipedia.org/wiki/Serial_Line_Internet_Protocol

// START Slip AND LINK IT TO THE onReceive FUNCTION
// THAT WILL BE CALLED WHEN A MESSAGE IS RECEIVED
Slip slip = Slip(Serial, onReceive); 

// TWO SERVOS
Servo servoA;
Servo servoB;

void setup() {
  
  Serial.begin(57600);

  servoA.attach(2);
  servoB.attach(3);
  
}

void loop() {
  // update() MUST BE CALLED EVERY LOOP
  slip.update();
}

// THIS FUNCTION IS CALLED WHEN A SLIP MESSAGE IS RECEIVED
void onReceive() {

  // GET THE LENGTH IN BYTES OF THE RECEIVED DATA
  int length = slip.length();

  // HERE WE CAN CHECK IF WE RECEIVED THE EXPECTED NUMBER OF BYTES
  if ( length == 2 ) {
    // WE CAN GET INDIVIUAL BYTES WITH get()
    byte firstByte = slip.get(0);
    byte secondByte = slip.get(1);
    // DO SOMETHING WITH THE DATA
    if ( firstByte == 0 ) {
      servoA.write(secondByte);
    } else if ( firstByte == 1 ) {
      servoB.write(secondByte);
    }
  }
  

}


