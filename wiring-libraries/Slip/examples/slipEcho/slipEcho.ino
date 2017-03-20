#include <Slip.h>

Slip slip = Slip(Serial, onReceive);

byte echo[128];

void setup() {
  
  Serial.begin(57600);
  
}

void loop() {
  slip.update();
}

void onReceive() {

  // ECHO AS ASCII
  /*
  for ( int i =0 ; i < slip.length() ; i++ ) {
    Serial.println(slip.get(i));
  }
  */

  // ECHO AS SLIP
  int echoLength = slip.length();
  for ( int i =0 ; i < echoLength ; i++ ) {
    echo[i] = slip.get(i);
  }
  slip.write(echo,echoLength);
  

}


