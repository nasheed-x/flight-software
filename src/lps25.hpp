#include <Adafruit_LPS2X.h>
Adafruit_LPS25 lps;

void lps25_retrieve_data(){
  sensors_event_t temp;
  sensors_event_t pressure;
  lps.getEvent(&pressure, &temp);// get pressure
  Serial.println("LPS25 DATA RETRIEVED");
  Serial.print(" Temperature: ");Serial.print(temp.temperature);Serial.println(" degrees C");
  Serial.print(" Pressure: ");Serial.print(pressure.pressure);Serial.println(" hPa");
  Serial.println();
  // Serial.println("");
}