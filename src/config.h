
#if defined(HALOSHIP)

/*** LPS25HB Barometer ***/
#define LPS_CS PC6
#define LPS_SCK PA5
#define LPS_MISO PA6
#define LPS_MOSI PA7

/*** RFM69HW Transceiver ***/
#define RFM69_INT PB4
#define RFM69_CS PC13
#define RFM69_RST PC1
#define RF69_FREQ 915.0

/*** Exposed Digital Pin ***/
#define USER_PIN_1 PA4
#define LED USER_PIN_1

/*** PCA9635 PWM Driver ***/
#define PWM_DRIVER_ADDRESS 0x70

#endif

// // RFM69 pin definition for stm32F4
// #if defined (FEATHER_BOARD)  // Defined in Arduino framework
//   #define RFM69_INT     PC6  //
//   #define RFM69_CS      PB9  //
//   #define RFM69_RST     PB8  // "A"
//   #define LED           PC1

// // LPS25 pin definition
// // #define LPS_CS PC7

// //BMX055 I2C address (JP1,JP2,JP3 = Open)
// // BMX055　I2C address of acceleration sensor
// #define Addr_Accl 0x19
// // BMX055　I2C address of gyro sensor
// #define Addr_Gyro 0x69
// // BMX055　I2C adress of magnetic sensor
// #define Addr_Mag 0x13
// #endif
