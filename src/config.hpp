/************ RFM69 Transceiver Module Setup ***************/

// If needed change to 434.0 or other frequency, must match RX's freq!
#define RF69_FREQ 915.0

#if defined (__AVR_ATmega328P__)  // Feather 328P w/wing
  #define RFM69_INT     3  // 
  #define RFM69_CS      4  //
  #define RFM69_RST     2  // "A"
  #define LED           13
#endif

// RFM69 pin definition for stm32F4
#if defined (STM32F4)  // Defined in Arduino framework
  // #define RFM69_INT     PB3  // 
  // #define RFM69_CS      PB5  //
  // #define RFM69_RST     PA10  // "A"
  // #define LED           PA5
  #define RFM69_INT     PC6  // 
  #define RFM69_CS      PB9  //
  #define RFM69_RST     PB8  // "A"
  #define LED           PC1
#endif

// LPS25 pin definition
#define LPS_CS PC7

//BMX055 I2C address (JP1,JP2,JP3 = Open)
// BMX055　I2C address of acceleration sensor 
#define Addr_Accl 0x19
// BMX055　I2C address of gyro sensor
#define Addr_Gyro 0x69
// BMX055　I2C adress of magnetic sensor
#define Addr_Mag 0x13