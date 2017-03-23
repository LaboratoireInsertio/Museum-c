// Tested with Arduino 1.7.10 from Arduino.org

// XBee-Arduino by Andrew Rapp (2009)
// https://github.com/andrewrapp/xbee-arduino
#include <XBee.h>
#include <Servo.h>
#include <Button.h>

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


const int buttonPin = A0;   // the number of the pushbutton pin
const int sensorPin = A4;
const int ledPin =  5;      // the number of the LED pin

//Zigbee Transmit Request API packet
ZBTxRequest txRequest;
uint8_t payload[] = {
  0
};


int lastSend = 0;

int dataIn = 0;

// variable for slow transitions
float outputValue = 0.0;


void setup() {
  // start serial (Serial: 0 (RX) and 1 (TX). Used to receive (RX)
  // and transmit (TX) TTL serial data using the ATmega32U4 hardware
  // serial capability. Note that on the Leonardo, the Serial class
  // refers to USB (CDC) communication; for TTL serial on pins 0 and
  // 1, use the Serial1 class.
  Serial.begin(57600);

  Serial1.begin(57600);
  //xbee.begin(Serial1);
  xbee.setSerial(Serial1);

  // Prepare the Zigbee Transmit Request API packet
  // Set the paylod for the data to be sent
  txRequest.setPayload(payload, sizeof(payload));
  // Identifies the UART data frame for the host to correlate with a
  // subsequent ACK (acknowledgment). If set to 0, no response is sent.
  txRequest.setFrameId(0);
  // Disable ACK (acknowledgement)
  txRequest.setOption(1);

  pinMode(buttonPin, INPUT);
  pinMode(ledPin, OUTPUT);
}


// continuously reads packets, looking for ZB Receive or Modem Status
void loop() {
  int buttonValue = digitalRead(buttonPin);
  int sensorValue = analogRead(sensorPin) / 4;






  int desiredValue = 50;

  // if someone is detected
  if (sensorValue > 120) {
    // set the output value to the maximum
    desiredValue = 255;

    // smoothly transition towards the max value
    float dif = desiredValue - outputValue;
    if (abs(dif) > 1.0) {
      outputValue = outputValue + dif / 8.0;
    }
  } else {
    // set the output value to the maximum
    desiredValue = 50;

    // smoothly transition towards the max value
    float dif = desiredValue - outputValue;
    if (abs(dif) > 1.0) {
      outputValue = outputValue + dif / 64.0;
    }
  }

  // change the analog out value:
  analogWrite(ledPin, outputValue);
  // print out the value you read:
  Serial.println(outputValue);
  delay(30);        // delay in between reads for stability







  //if (abs(lastSend - sensorValue) > 30) {
  //  sendPacket(XBeeAddress64(0x00000000, 0x00000000), sensorValue);
  //sendPacket(XBeeAddress64(0x0013a200, 0x40e66dcd), sensorValue);
  //  lastSend = sensorValue;

  //  Serial.println(sensorValue);
  //}

  //if (buttonValue == HIGH)
  //  digitalWrite(ledPin, HIGH);
  //else
  //  digitalWrite(ledPin, LOW);

  if (abs(lastSend - buttonValue) > 0) {
    // to send only to the coordinator
    //sendPacket(XBeeAddress64(0x00000000, 0x00000000), buttonValue);
    // to broadcast the message
    //sendPacket(XBeeAddress64(0x00000000, 0x0000ffff), buttonValue);
    // to send to specific XBee
    sendPacket(XBeeAddress64(0x0013a200, 0x40e66da0), buttonValue);
    sendPacket(XBeeAddress64(0x0013a200, 0x40e66dcd), buttonValue);
    sendPacket(XBeeAddress64(0x0013a200, 0x40e66c35), buttonValue);
    sendPacket(XBeeAddress64(0x0013a200, 0x40e66dca), buttonValue);
    sendPacket(XBeeAddress64(0x0013a200, 0x40e66c1b), buttonValue);

    lastSend = buttonValue;

    Serial.println(buttonValue);
  }


  xbee.readPacket();

  if (xbee.getResponse().isAvailable()) {
    // got something
    if (xbee.getResponse().getApiId() == ZB_RX_RESPONSE) {
      // got a zb rx packet

      // now fill our zb rx class
      xbee.getResponse().getZBRxResponse(rx);

      // get value of the first byte in the data
      dataIn = rx.getData(0);

      digitalWrite(ledPin, dataIn);
      Serial.println(dataIn);

    }
  }

  //if (dataIn == HIGH)
  //  digitalWrite(ledPin, HIGH);
  //else
  //  digitalWrite(ledPin, LOW);

}

void sendPacket(XBeeAddress64 addr64, uint8_t val) {
  // Set the destination address of the message
  txRequest.setAddress64(addr64);
  payload[0] = val;
  // Send the message
  xbee.send(txRequest);
}

