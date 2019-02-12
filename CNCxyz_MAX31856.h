
#ifndef CNCXYZ_MAX31856_H
#define CNCXYZ_MAX31856_H

#include "Arduino.h"

// Register Memory Map (Datasheet Page 18)
#define MAX31856_REG_CR0				0x00 // Configuration 0 Register
#define MAX31856_REG_CR1				0x01 // Configuration 1 Register
#define MAX31856_REG_MASK				0x02 // Fault Mask Register
#define MAX31856_REG_CJHF				0x03 // Cold-Junction High Fault Threshold
#define MAX31856_REG_CJLF				0x04 // Cold-Junction Low Fault Threshold
#define MAX31856_REG_LTHFTH				0x05 // Linearized Temperature High Fault Threshold MSB
#define MAX31856_REG_LTHFTL				0x06 // Linearized Temperature High Fault Threshold LSB
#define MAX31856_REG_LTLFTH				0x07 // Linearized Temperature Low Fault Threshold MSB
#define MAX31856_REG_LTLFTL				0x08 // Linearized Temperature Low Fault Threshold LSB
#define MAX31856_REG_CJTO				0x09 // Cold-Junction Temperature Offset Register
#define MAX31856_REG_CJTH				0x0A // Cold-Junction Temperature Register, MSB
#define MAX31856_REG_CJTL				0x0B // Cold-Junction Temperature Register, LSB
#define MAX31856_REG_LTCBH				0x0C // Linearized TC Temperature, Byte 2 (Read Only)
#define MAX31856_REG_LTCBM				0x0D // Linearized TC Temperature, Byte 1 (Read Only)
#define MAX31856_REG_LTCBL				0x0E // Linearized TC Temperature, Byte 0 (Read Only)
#define MAX31856_REG_SR					0x0F // Fault Status Register (Read Only)

// Configuration 0 Register (Datasheet Page 19)
#define MAX31856_REG_CR0_AUTOCONVERT	0x80 // Configuration 0 Conversion Mode
#define MAX31856_REG_CR0_1SHOT			0x40 // Configuration 0 One-Shot Mode
#define MAX31856_REG_CR0_OCFAULT1		0x20 // Configuration 0 Open-Circuit Fault Detection 1
#define MAX31856_REG_CR0_OCFAULT0		0x10 // Configuration 0 Open-Circuit Fault Detection 0
#define MAX31856_REG_CR0_CJ				0x08 // Configuration 0 Cold-Junction Sensor Disable
#define MAX31856_REG_CR0_FAULT			0x04 // Configuration 0 Fault Mode
#define MAX31856_REG_CR0_FAULTCLR		0x02 // Configuration 0 Fault Status Clear
#define MAX31856_REG_CR0_NOISE_FILTER	0x01 // Configuration 0 50Hz/60Hz Noise Rejection Filter Selection

// Averaging Mode (Datasheet Page 20)
#define MAX31856_AVGSEL_1				0b000 // Default
#define MAX31856_AVGSEL_2				0b001
#define MAX31856_AVGSEL_4				0b010
#define MAX31856_AVGSEL_8				0b011
#define MAX31856_AVGSEL_16				0b100

// Thermocouple Types (Datasheet Page 20)
#define MAX31856_TC_TYPE_B				0b0000
#define MAX31856_TC_TYPE_E				0b0001
#define MAX31856_TC_TYPE_J				0b0010
#define MAX31856_TC_TYPE_K				0b0011 // Default
#define MAX31856_TC_TYPE_N				0b0100
#define MAX31856_TC_TYPE_R				0b0101
#define MAX31856_TC_TYPE_S				0b0110
#define MAX31856_TC_TYPE_T				0b0111
#define MAX31856_VMODE_8				0b1000 // Voltage Mode, Gain = 8. Code = 8 x 1.6 x 217 x VIN
#define MAX31856_VMODE_32				0b1100 // Voltage Mode, Gain = 32. Code = 32 x 1.6 x 217 x VIN

// Fault Mask Register (Datasheet Page 21)
#define MAX31856_FAULT_CJRANGE			0x80 // Cold Junction Out-of-Range
#define MAX31856_FAULT_TCRANGE			0x40 // Thermocouple Out-of-Range
#define MAX31856_FAULT_CJHIGH			0x20 // Cold-Junction High Fault
#define MAX31856_FAULT_CJLOW			0x10 // Cold-Junction Low Fault
#define MAX31856_FAULT_TCHIGH			0x08 // Thermocouple Temperature High Fault
#define MAX31856_FAULT_TCLOW			0x04 // Thermocouple Temperature Low Fault
#define MAX31856_FAULT_OVUV				0x02 // Overvoltage or Undervoltage Input Fault
#define MAX31856_FAULT_OPEN				0x01 // Thermocouple Open-Circuit Fault


#endif
