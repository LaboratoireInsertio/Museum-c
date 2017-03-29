

/*  * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 Code by Thomas O Fredericks (tof@t-o-f.info)
* * * * * * * * * * * * * * * * * * * * * * * * * * * * */
// See : https://en.wikipedia.org/wiki/Serial_Line_Internet_Protocol


#ifndef Slip_h
#define Slip_h

#define SLIPBUFFERSIZE 128

#include <inttypes.h>
 #include <Stream.h>

 extern "C" {
// callback function
    typedef void (*slipCallbackFunction)(void);
}


class Slip
{

public:
  
  Slip(Stream& stream, slipCallbackFunction callback); // Link to a Stream (Serial) and set a callback function
  
  void update(); // Read from the stream and call the callback if a message is parsed
  uint8_t length(); // The number of bytes of parsed data
  uint8_t get(uint8_t i); // Get a specific byte of data
  void write(uint8_t data[],uint8_t length); // Write a SLIP to the stream

  
protected:
  uint8_t count,validCount;
    uint8_t bytes[SLIPBUFFERSIZE] ;
  bool escape;
  slipCallbackFunction callback;
  Stream* stream;
 
};

#endif


