/**
 * Copyright (c) 2009 Andrew Rapp. All rights reserved.
 *
 * This file is part of XBee-Arduino.
 *
 * XBee-Arduino is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * XBee-Arduino is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with XBee-Arduino.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <XBee.h>
#include "Node.h"
#include <Slip.h>
#include "nodeList.h"

/*
This example is for Series 2 XBee
 Sends a ZB TX request with the value of analogRead(pin5) and checks 
 the status response for success
 */

// create the XBee object
#include "WProgram.h"
void setup();
void loop();
void onReceive();
void sendPacket(XBeeAddress64 addr64, uint8_t val);
XBee xbee = XBee();

// START Slip AND LINK IT TO THE onReceive FUNCTION
// THAT WILL BE CALLED WHEN A MESSAGE IS RECEIVED
Slip slip = Slip(Serial, onReceive); 

//Zigbee Transmit Request API packet
ZBTxRequest txRequest;
uint8_t payload[] = {
  0
};

uint8_t slipEcho[] = {0,0};


void setup() {
  // starts serial communication with computer
  Serial.begin(57600);

  // starts serial communication with xBee
  Serial1.begin(57600);
  xbee.setSerial(Serial1);
  
  // Prepare the Zigbee Transmit Request API packet
  // Set the paylod for the data to be sent
  txRequest.setPayload(payload, sizeof(payload));
  // Identifies the UART data frame for the host to correlate with a
  // subsequent ACK (acknowledgment). If set to 0, no response is sent.
  txRequest.setFrameId(0);
  // Disable ACK (acknowledgement)
  txRequest.setOption(1);
}

void loop() {
  // break down 10-bit reading into two bytes and place in payload
  // USING PIN 5 FOR TESTING, UPDATE WITH VALUES FROM LIVE
  //pin5 = analogRead(5)/4;

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
    byte destinationXbee = slip.get(0); // first byte
    byte dataToSend = slip.get(1); //second byte
    dataToSend = map(dataToSend,1,127,0,255);

       for (int i = 0; i < (sizeof(nodes) / sizeof(Node)); i++) {
      if (nodes[i].getNote() == destinationXbee) {
#ifdef SEND_XBEE_ONLY_ON_CHANGE
        nodes[i].setVal(dataToSend);
#else
        sendPacket(nodes[i].getAddress(), dataToSend);
#endif
        break;
      }
    }

#ifdef SEND_XBEE_ONLY_ON_CHANGE
    // sends messages to the nodes only if the values have changed
    for (int i = 0; i < (sizeof(nodes) / sizeof(Node)); i++)
      if (nodes[i].valueHasChanged())
        sendPacket(nodes[i].getAddress(), nodes[i].getVal());
#endif

  slipEcho[0] = destinationXbee;
  slipEcho[1] = dataToSend;
  slip.write(slipEcho,2);

  }
}


void sendPacket(XBeeAddress64 addr64, uint8_t val) {
  // Set the destination address of the message
  txRequest.setAddress64(addr64);
  payload[0] = val;
  // Send the message
  xbee.send(txRequest);
}


