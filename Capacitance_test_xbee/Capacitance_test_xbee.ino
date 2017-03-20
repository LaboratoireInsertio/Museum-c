// Tested with Arduino 1.7.10 from Arduino.org

// XBee-Arduino by Andrew Rapp (2009)
// https://github.com/andrewrapp/xbee-arduino

#include <XBee.h>
#include <Button.h>
#include <Servo.h>
#include <CapacitiveSensor.h>


#define DC       0
#define SERVO    1
#define SOLENOID 2


XBee xbee = XBee();
XBeeResponse response = XBeeResponse();
// create reusable response objects for responses we expect to handle
ZBRxResponse rx = ZBRxResponse();
ModemStatusResponse msr = ModemStatusResponse();

const int buttonPin = A0;   // the number of the pushbutton pin
const int ledPin =  5;      // the number of the LED pin

//Zigbee Transmit Request API packet
ZBTxRequest txRequest;
uint8_t payload[] = {
  0
};


int lastSend = 0;

int dataIn = 0;

Servo servoA;
Servo servoB;

Button switch_servo = Button(5, BUTTON_PULLUP_INTERNAL);
Button switch_solenoide = Button(9, BUTTON_PULLUP_INTERNAL);

int actuator = DC;

CapacitiveSensor   cs_4_2 = CapacitiveSensor(6, 15);       // 10M resistor between pins 4 & 2, pin 2 is sensor pin, add a wire and or foil if desired

void setup() {
  // put your setup code here, to run once:
  cs_4_2.set_CS_AutocaL_Millis(0xFFFFFFFF);     // turn off autocalibrate on channel 1 - just as an example

  Serial.begin (57600);

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
  pinMode(ledPin, OUTPUT);

  //Switch D5

  if (switch_servo.isPressed()) {
    actuator = SERVO;

    servoA.attach(A5);
    servoB.attach(A4);

    //Switch D9
  } else if (switch_solenoide.isPressed()) {
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


/////////////////////////////


void loop() {

  // put your main code here, to run repeatedly:
  long start = millis();
  long data =  cs_4_2.capacitiveSensor(30);
  /*  long total2 =  cs_4_6.capacitiveSensor(30);
    long total3 =  cs_4_8.capacitiveSensor(30);*/


  if (abs(lastSend - data) > 0) {
    // to send only to the coordinator
    //sendPacket(XBeeAddress64(0x00000000, 0x00000000), buttonValue);
    // to broadcast the message
    //sendPacket(XBeeAddress64(0x00000000, 0x0000ffff), buttonValue);
    // to send to specific XBee
    sendPacket(XBeeAddress64(0x0013a200, 0x40e66da0), data);
    sendPacket(XBeeAddress64(0x0013a200, 0x40e66dcd), data);
    sendPacket(XBeeAddress64(0x0013a200, 0x40e66c35), data);
    sendPacket(XBeeAddress64(0x0013a200, 0x40e66dcb), data);
    sendPacket(XBeeAddress64(0x0013a200, 0x40e66c21), data);

    lastSend = data;

    Serial.println(data);
  }

  /*Serial.print(millis() - start);        // check on performance in milliseconds
  Serial.print("\t");                    // tab character for debug windown spacing

  Serial.print(data);                  // print sensor output 1
  Serial.println("\t");
  Serial.print(total2);                  // print sensor output 2
  Serial.print("\t");
  Serial.println(total3);    */            // print sensor output 3


  xbee.readPacket();

  if (xbee.getResponse().isAvailable()) {
    // got something
    if (xbee.getResponse().getApiId() == ZB_RX_RESPONSE) {
      // got a zb rx packet

      // now fill our zb rx class
      xbee.getResponse().getZBRxResponse(rx);

      // get value of the first byte in the data
      dataIn = rx.getData(0);
      
      if (dataIn > 0) {
        data = dataIn;
      } else {
        data = cs_4_2.capacitiveSensor(30);
      }

    }
  }




  // Actuator Servo
  if (actuator == SERVO ) {
    data = map(data, 0, 410, 0, 180);

    servoA.write(data);
    servoB.write(data);

    // Actuator Solenoide - Switch D9
  } else if (actuator == SOLENOID) {

    digitalWrite(7, HIGH);
    digitalWrite(8, LOW);

    digitalWrite(12, HIGH);
    digitalWrite(13, LOW);

    digitalWrite(10, HIGH);
    digitalWrite(11, HIGH);
    delay (map(data, 0, 410, 1, 25));
    digitalWrite(10, LOW);
    digitalWrite(11, LOW);



  } else if (actuator == DC) {
    data = map(data, 0, 410, 0, 255);


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

void sendPacket(XBeeAddress64 addr64, uint8_t val) {
  // Set the destination address of the message
  txRequest.setAddress64(addr64);
  payload[0] = val;
  // Send the message
  xbee.send(txRequest);
}


