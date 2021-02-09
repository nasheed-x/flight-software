/************ RFM69 Transceiver Module Setup ***************/

// If needed change to 434.0 or other frequency, must match RX's freq!
#define RF69_FREQ 915.0

#if defined (__AVR_ATmega328P__)  // Feather 328P w/wing
  #define RFM69_INT     3  // 
  #define RFM69_CS      4  //
  #define RFM69_RST     2  // "A"
  #define LED           13
#endif

#if defined (STM32F4)  // Defined in Arduino framework
  #define RFM69_INT     PB3  // 
  #define RFM69_CS      PB5  //
  #define RFM69_RST     PA10  // "A"
  #define LED           PA5
#endif