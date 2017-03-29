// FOR PERFORMANCE ---------------

#include <XBee.h>

// create an array of Node objects with the SH + SL Address of the 
// receiving XBees and the assigned notes
Node nodes[] = { 
  Node(XBeeAddress64(0x0013a200, 0x40e66c5c), 48),  // 48 - C2
  Node(XBeeAddress64(0x0013a200, 0x40e66c2d), 49),  // 49 - C#2
  Node(XBeeAddress64(0x0013a200, 0x40e66dcb), 50),  // 50 - D2
  Node(XBeeAddress64(0x0013a200, 0x40e66c42), 51),  // 51 - D#2
  Node(XBeeAddress64(0x0013a200, 0x40e66c21), 52),  // 52 - E2   
 
  Node(XBeeAddress64(0x0013a200, 0x40e66c1d), 60),  // 60 - C3
  Node(XBeeAddress64(0x0013a200, 0x40e66c3c), 61),  // 61 - C#3
  Node(XBeeAddress64(0x0013a200, 0x40e668d4), 62),  // 62 - D3
  Node(XBeeAddress64(0x0013a200, 0x40e66c5b), 63),  // 63 - D#3
  Node(XBeeAddress64(0x0013a200, 0x40e66c46), 64),  // 64 - E3
  
  Node(XBeeAddress64(0x0013a200, 0x40e66c13), 72),  // 72 - C4
  Node(XBeeAddress64(0x0013a200, 0x40e66dd7), 73),  // 73 - C#4
  Node(XBeeAddress64(0x0013a200, 0x40e66c49), 74),  // 74 - D4
  Node(XBeeAddress64(0x0013a200, 0x40e66c18), 75),  // 75 - D#4
  Node(XBeeAddress64(0x0013a200, 0x40e66c2f), 76),  // 76 - E4
  
  Node(XBeeAddress64(0x0013a200, 0x40e66c28), 84),  // 84 - C5
  Node(XBeeAddress64(0x0013a200, 0x40e66c55), 85),  // 85 - C#5
  Node(XBeeAddress64(0x0013a200, 0x40e66c3e), 86),  // 86 - D5
  Node(XBeeAddress64(0x0013a200, 0x40e66c12), 87),  // 87 - D#5
  Node(XBeeAddress64(0x0013a200, 0x40e66c48), 88),  // 88 - E5
  
  Node(XBeeAddress64(0x0013a200, 0x40e66c50), 96),  // 96 - C6
  Node(XBeeAddress64(0x0013a200, 0x40e66c14), 97),  // 97 - C#6
  Node(XBeeAddress64(0x0013a200, 0x40e66c2b), 98),  // 98 - D6
  Node(XBeeAddress64(0x0013a200, 0x40e66c5d), 99),  // 99 - D#6
  Node(XBeeAddress64(0x0013a200, 0x40e66c32), 100)  // 100 - E6
  };
