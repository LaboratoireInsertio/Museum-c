// Tested with Arduino 1.7.10 from Arduino.org

// XBee-Arduino by Andrew Rapp (2009)
// https://github.com/andrewrapp/xbee-arduino
#include <XBee.h>
#include <Button.h>

#define DC       0
#define SOLENOID 2


/*
  This example is for Series 2 XBee
  Receives a ZB RX packet and sets a PWM value based on packet data.
  Error led is flashed if an unexpected packet is received
*/

XBee xbee = XBee();
XBeeResponse response = XBeeResponse();
// create reusable response objects for responses we expect to handle
ZBRxResponse rx = ZBRxResponse();
ModemStatusResponse msr = ModemStatusResponse();

const int buttonPin = 14;   // the number of the pushbutton pin



Button switch_solenoide = Button(9, BUTTON_PULLUP_INTERNAL);

int actuator = DC;

int data = 0;

int sensor = 0;
int previousSensorValue = 0;

//Zigbee Transmit Request API packet
ZBTxRequest txRequest;
uint8_t payload[] = {
  0
};


void setup() {
  // start serial (Serial: 0 (RX) and 1 (TX). Used to receive (RX)
  // and transmit (TX) TTL serial data using the ATmega32U4 hardware
  // serial capability. Note that on the Leonardo, the Serial class
  // refers to USB (CDC) communication; for TTL serial on pins 0 and
  // 1, use the Serial1 class.
  Serial.begin(57600);

  Serial1.begin(57600);
  xbee.begin(Serial1);

  // Prepare the Zigbee Transmit Request API packet
  // Set the paylod for the data to be sent
  txRequest.setPayload(payload, sizeof(payload));
  // Identifies the UART data frame for the host to correlate with a
  // subsequent ACK (acknowledgment). If set to 0, no response is sent.
  txRequest.setFrameId(0);
  // Disable ACK (acknowledgement)
  txRequest.setOption(1);

  pinMode(buttonPin, INPUT);

    //Switch D9
   if (switch_solenoide.isPressed()) {
    actuator = SOLENOID;

    //Motor 1 direction - OUT1 & OUT2
    pinMode(7, OUTPUT);
    pinMode(8, OUTPUT);
    //Motor 1 Speed
    pinMode(10, OUTPUT);

    //Motor 2 direction - OUT3 & OUT4
    pinMode(12, OUTPUT);
    pinMode(13, OUTPUT);
    //Motor 2 Speed
    pinMode(11, OUTPUT);

  } else {
    actuator = DC;

    //Motor 1 direction - OUT1 & OUT2
    pinMode(7, OUTPUT);
    pinMode(8, OUTPUT);
    //Motor 1 Speed
    pinMode(10, OUTPUT);

    //Motor 2 direction - OUT3 & OUT4
    pinMode(12, OUTPUT);
    pinMode(13, OUTPUT);
    //Motor 2 Speed
    pinMode(11, OUTPUT);

  }
}


// continuously reads packets, looking for ZB Receive or Modem Status
void loop() {

  
  
  if (digitalRead(buttonPin) == LOW && previousSensorValue == HIGH ) {

    previousSensorValue = LOW;
    // to send only to the coordinator
   // sendPacket(XBeeAddress64(0x00000000, 0x00000000), 1);
     // to send to specific XBee
    sendPacket(XBeeAddress64(0x0013a200, 0x40e66dc3), 1);
     /*ROUTERADDRESS
   sendPacket(XBeeAddress64(0x0013a200, 0x40e668d2), 1);*/

    digitalWrite(13, HIGH);

    //Serial.println(1);

  } else if (digitalRead(buttonPin) == HIGH && previousSensorValue == LOW ) {

    previousSensorValue = HIGH;
    
    // to send only to the coordinator
   // sendPacket(XBeeAddress64(0x00000000, 0x00000000), 0);
     // to send to specific XBee
    sendPacket(XBeeAddress64(0x0013a200, 0x40e66dc3), 0);
     /*ROUTERADDRESS
   sendPacket(XBeeAddress64(0x0013a200, 0x40e668d2), 0);*/

    digitalWrite(13, LOW);

    //Serial.println(0);

  }


  xbee.readPacket();

  if (xbee.getResponse().isAvailable()) {
    // got something
    if (xbee.getResponse().getApiId() == ZB_RX_RESPONSE) {
      // got a zb rx packet

      // now fill our zb rx class
      xbee.getResponse().getZBRxResponse(rx);

      // get value of the first byte in the data
      data = rx.getData(0);

      //Serial.println(data);


        // Actuator Solenoide - Switch D9
      if (actuator == SOLENOID) {

        digitalWrite(7, HIGH);
        digitalWrite(8, LOW);

        digitalWrite(12, HIGH);
        digitalWrite(13, LOW);

        digitalWrite(10, HIGH);
        digitalWrite(11, HIGH);
        delay (map(data, 0, 255, 1, 25));
        digitalWrite(10, LOW);
        digitalWrite(11, LOW);



      } else if (actuator == DC) {
        data = map(data, 0, 255, 0, 255);


#ifdef BIDIRECTIONNAL_DC

        // TWO DIRECTIONAL DC MOTOR - OPTIONAL

        if (data < 128) {
          digitalWrite(7, LOW);
          digitalWrite(8, HIGH);

          analogWrite(10, 255 - map(data, 0, 127, 0, 255));

          digitalWrite(12, LOW);
          digitalWrite(13, HIGH);

          analogWrite(11, 255 - map(data, 0, 127, 0, 255));

        } else {
          digitalWrite(7, HIGH);
          digitalWrite(8, LOW);

          analogWrite(10, map(data, 128, 255, 0, 255));

          digitalWrite(12, HIGH);
          digitalWrite(13, LOW);

          analogWrite(11, map(data, 128, 255, 0, 255));
        }


#else

        // ONE DIRECTION DC MOTOR

        digitalWrite(7, HIGH);
        digitalWrite(8, LOW);

        analogWrite(10, data);

        digitalWrite(12, HIGH);
        digitalWrite(13, LOW);

        analogWrite(11, data);
#endif
      }
    }
  }
  data = 0;
}

void sendPacket(XBeeAddress64 addr64, uint8_t val) {
  // Set the destination address of the message
  txRequest.setAddress64(addr64);
  payload[0] = val;
  // Send the message
  xbee.send(txRequest);
}

