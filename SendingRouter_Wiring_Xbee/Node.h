/*
  Node.h - For managing nodes on the network
 */

#ifndef Node_h
#define Node_h

#include "Wiring.h"
#include "XBee.h"

class Node
{
public:
  Node(XBeeAddress64 addr64, uint8_t note);
  //void setActu(uint8_t _actu);
  uint8_t getNote();
  void setVal(uint8_t val);
  uint8_t getVal();
  boolean valueHasChanged();
  XBeeAddress64 getAddress();
private:
  XBeeAddress64 _addr64;
  uint8_t _note;
  //uint8_t actu;
  uint8_t _val;
  uint8_t _lastValSend;
};

#endif
