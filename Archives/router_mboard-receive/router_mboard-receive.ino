// Tested with Arduino 1.7.10 from Arduino.org
// This is for Series 2 XBee

// XBee-Arduino by Andrew Rapp (2009)
// https://github.com/andrewrapp/xbee-arduino
#include <XBee.h>
#include "Node.h"
#include <Slip.h>
#include "nodeList.h"

// create the XBee object
XBee xbee = XBee();
XBeeResponse response = XBeeResponse();
// create reusable response objects for responses we expect to handle
ZBRxResponse rx = ZBRxResponse();
ModemStatusResponse msr = ModemStatusResponse();

// START Slip AND LINK IT TO THE onReceive FUNCTION
// THAT WILL BE CALLED WHEN A MESSAGE IS RECEIVED
Slip slip = Slip(Serial, onReceive);

//Zigbee Transmit Request API packet
ZBTxRequest txRequest;
uint8_t payload[] = {
  0
};

uint8_t slipEcho[] = {0, 0};

int sensor;


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
  // continuously reads packets, looking for ZB Receive or Modem Status
  xbee.readPacket();

  if (xbee.getResponse().isAvailable()) {
    // got something
    if (xbee.getResponse().getApiId() == ZB_RX_RESPONSE) {
      // got a zb rx packet

      // now fill our zb rx class
      xbee.getResponse().getZBRxResponse(rx);

      // get value of the first byte in the data
      sensor = rx.getData(0);

      slipEcho[0] = sensor;
      slip.write(slipEcho, 1);
      Serial.println(sensor);
    }
  }
  
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
    dataToSend = map(dataToSend, 1, 127, 0, 255);

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
    slip.write(slipEcho, 2);

  }
}


void sendPacket(XBeeAddress64 addr64, uint8_t val) {
  // Set the destination address of the message
  txRequest.setAddress64(addr64);
  payload[0] = val;
  // Send the message
  xbee.send(txRequest);
}

