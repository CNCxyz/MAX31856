#include "CNCxyz_MAX31856.h"

#define CS_PIN  9 // Pin number used for CS
#define LOOP_DELAY_ms 1000 // Delay between subsequental conversions

CNCxyz_MAX31856 MAX31856(CS_PIN); // MAX31856 object

char string_buf[32]; // For numbers to string conversion

void setup() {
  Serial.begin(9600);
  Serial.println("Starting MAX31856 example...");
  MAX31856.begin();

  // Set TC type
  MAX31856.setThermocoupleType(MAX31856_TC_TYPE_K);
}
void loop() {
  // Convert temperature
  MAX31856.convert();

  // Read and display thermocouple temperature
  float LTC = MAX31856.readThermocouple();
  dtostrf(LTC, 8, 6, string_buf);
  Serial.print("LTC value: ");
  Serial.println(string_buf);

  // Read and display cold junction temperature
  float CJT = MAX31856.readColdJunction();
  dtostrf(CJT, 8, 6, string_buf);
  Serial.print("CJT value: ");
  Serial.println(string_buf);
  Serial.println();
  
  delay(LOOP_DELAY_ms);
}
