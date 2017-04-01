#include <Slip.h>

// THIS IMPLEMENTATION IS BASED ON :
// https://en.wikipedia.org/wiki/Serial_Line_Internet_Protocol

// START Slip AND LINK IT TO THE onReceive FUNCTION
// THAT WILL BE CALLED WHEN A MESSAGE IS RECEIVED
Slip slip = Slip(Serial, onReceive); 

void setup() {
  
  Serial.begin(57600);
  
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
    byte data0 = slip.get(0);
    byte data1 = slip.get(1);
    // DO SOMETHING WITH THE DATA
  }
  

}


