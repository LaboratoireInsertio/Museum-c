

//ADDED FOR COMPATIBILITY WITH WIRING
extern "C" {
  #include <stdlib.h>
}

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include "Slip.h"


// See : https://en.wikipedia.org/wiki/Serial_Line_Internet_Protocol
#define SLIP_END 0xC0 
#define SLIP_ESC 0xDB
#define SLIP_ESC_END 0xDC
#define SLIP_ESC_ESC 0xDD


Slip::Slip(Stream& s  ,slipCallbackFunction c ) {
	stream = &s;
	callback = c;
}

void Slip::update( ) {

	while ( stream->available() ) {
		int data = stream->read();
		if ( data == SLIP_END ) {
				validCount = count;
		     	if ( count > 0 && callback != NULL) (*callback)();
		    	count = 0;
		    	escape = false;
		    	validCount = 0;
			} else if ( data == SLIP_ESC ) {
		       escape = true;
			} else {
				if ( escape ) {
					if ( data == SLIP_ESC_END ) data = SLIP_END;
		            else if ( data == SLIP_ESC_ESC ) data = SLIP_ESC;
		            escape = false;
				}
		       bytes[count] = data;
		       count++;
		       if ( count == SLIPBUFFERSIZE ) count =0;
			}

	}


 	
 
	
}


uint8_t Slip::length() {
   return validCount;
}


uint8_t Slip::get(uint8_t i) {
	return bytes[i];
}

void Slip::write(uint8_t data[], uint8_t  length ) {

   for ( uint8_t i = 0; i < length ; i++ ) {
   	uint8_t b = data[i];
   	if ( b == SLIP_END) {
   	  stream->write(SLIP_ESC);
   	  stream->write(SLIP_ESC_END);
   	} else if ( b == SLIP_ESC ) {
       stream->write(SLIP_ESC);
   	   stream->write(SLIP_ESC_ESC);
   	} else {
   		stream->write(b);
   	}

   }

   stream->write(SLIP_END);
}


/*

uint8_t ByteStream::available() {
    
  while ( Serial.available() ) {
    
    uint8_t data = Serial.read();
    
    if ( data & B10000000 ) { // START+LENGTH B1LLLLLLL
       
		in_int = 0;
        in_index = 0;
        expected_bytes = data & B01111111;
        if ( expected_bytes == 0 ) expected_bytes = 128; //in 8bit bytes
        //Serial.print("Bytes expected : ");
        //Serial.println(expected_bytes);
	    in_bits = 0;
 
    } else { // 7 BIT DATA B0XXXXXXX
       
       in_int = ((data & B01111111) << in_bits) | in_int;
       in_bits = in_bits + 7;
	
		while ( in_bits > 7 ) {
		
			in[in_index] = ((unsigned char)(in_int & 0xFF));	
			
			in_index++;
			in_bits = in_bits - 8;
			in_int = in_int >> 8;
		}
			
		// CHECK LENGTH
	   
		if ( in_index  >= expected_bytes ) {
		  
		    in_length = in_index;
		    in_index = 0;
			read_index = 0;
		  
			return  1;
		  
		}

	    return 0;
		}
		
		
	}
  
  return 0;
  
}

uint8_t ByteStream::length() {
	return in_length;
}

uint8_t ByteStream::read() {
    
    if ( read_index < MAXLENGTH ) {
	    uint8_t b = in[read_index];
	    read_index++;
		return b;
	}
	return 0;
}
*/