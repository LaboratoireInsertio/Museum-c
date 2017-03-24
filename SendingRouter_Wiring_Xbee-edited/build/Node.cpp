/*
  Node.cpp - For managing nodes on the network
 */

#include "Wiring.h"
#include "Node.h"
#include "XBee.h"

Node::Node(XBeeAddress64 addr64, uint8_t note)
{
  _addr64 = addr64;
  _note = note;
  _val = 0;
  _lastValSend = 0;
}

uint8_t Node::getNote()
{
  return _note;
}

void Node::setVal(uint8_t val)
{
  _val = val;
}

uint8_t Node::getVal()
{
  return _val;
}

boolean Node::valueHasChanged()
{
  if (_val != _lastValSend){
    _lastValSend = _val;
    return true;
  }
  return false;
}

XBeeAddress64 Node::getAddress()
{
  return _addr64;
}
