
#ifndef CNCXYZ_MAX31856_H
#define CNCXYZ_MAX31856_H

#include "Arduino.h"

// Register Memory Map (Datasheet Page 18)
typedef enum {
  MAX31856_REG_CR0			=	0x00, // Configuration 0 Register
  MAX31856_REG_CR1			=	0x01, // Configuration 1 Register
  MAX31856_REG_MASK			=	0x02, // Fault Mask Register
  MAX31856_REG_CJHF			=	0x03, // Cold-Junction High Fault Threshold
  MAX31856_REG_CJLF			=	0x04, // Cold-Junction Low Fault Threshold
  MAX31856_REG_LTHFTH		=	0x05, // Linearized Temperature High Fault Threshold MSB
  MAX31856_REG_LTHFTL		=	0x06, // Linearized Temperature High Fault Threshold LSB
  MAX31856_REG_LTLFTH		=	0x07, // Linearized Temperature Low Fault Threshold MSB
  MAX31856_REG_LTLFTL		=	0x08, // Linearized Temperature Low Fault Threshold LSB
  MAX31856_REG_CJTO			=	0x09, // Cold-Junction Temperature Offset Register
  MAX31856_REG_CJTH			=	0x0A, // Cold-Junction Temperature Register, MSB
  MAX31856_REG_CJTL			=	0x0B, // Cold-Junction Temperature Register, LSB
  MAX31856_REG_LTCBH		=	0x0C, // Linearized TC Temperature, Byte 2 (Read Only)
  MAX31856_REG_LTCBM		=	0x0D, // Linearized TC Temperature, Byte 1 (Read Only)
  MAX31856_REG_LTCBL		=	0x0E, // Linearized TC Temperature, Byte 0 (Read Only)
  MAX31856_REG_SR				=	0x0F, // Fault Status Register (Read Only)
} MAX31856_addressT;

// Configuration 0 Register masks (Datasheet Page 19)
typedef enum {
  MAX31856_REG_CR0_AUTOCONVERT	= 0x80, // Configuration 0 Conversion Mode
  MAX31856_REG_CR0_1SHOT			  = 0x40, // Configuration 0 One-Shot Mode
  MAX31856_REG_CR0_OCFAULT1		  = 0x20, // Configuration 0 Open-Circuit Fault Detection 1
  MAX31856_REG_CR0_OCFAULT0		  = 0x10, // Configuration 0 Open-Circuit Fault Detection 0
  MAX31856_REG_CR0_CJ				    = 0x08, // Configuration 0 Cold-Junction Sensor Disable
  MAX31856_REG_CR0_FAULT			  = 0x04, // Configuration 0 Fault Mode
  MAX31856_REG_CR0_FAULTCLR		  = 0x02, // Configuration 0 Fault Status Clear
  MAX31856_REG_CR0_NOISE_FILTER	= 0x01, // Configuration 0 50Hz/60Hz Noise Rejection Filter Selection
} MAX31856_CR0_MaskT;

// Averaging Mode (Datasheet Page 20)
typedef enum {
  MAX31856_AVG_NSAMPLES_1			=	0b000, // Default
  MAX31856_AVG_NSAMPLES_2			=	0b001,
  MAX31856_AVG_NSAMPLES_4			=	0b010,
  MAX31856_AVG_NSAMPLES_8			=	0b011,
  MAX31856_AVG_NSAMPLES_16		=	0b100,
} MAX31856_AVGSEL_MaskT;

// Thermocouple Types (Datasheet Page 20)
typedef enum {
  MAX31856_TC_TYPE_B			=	0b0000,
  MAX31856_TC_TYPE_E			=	0b0001,
  MAX31856_TC_TYPE_J			=	0b0010,
  MAX31856_TC_TYPE_K			=	0b0011, // Default
  MAX31856_TC_TYPE_N			=	0b0100,
  MAX31856_TC_TYPE_R			=	0b0101,
  MAX31856_TC_TYPE_S			=	0b0110,
  MAX31856_TC_TYPE_T			=	0b0111,
  MAX31856_VMODE_8				= 0b1000, // Voltage Mode, Gain = 8. Code = 8 x 1.6 x 217 x VIN
  MAX31856_VMODE_32				= 0b1100, // Voltage Mode, Gain = 32. Code = 32 x 1.6 x 217 x VIN
} MAX31856_TC_TYPE_MaskT;

// Fault Mask Register (Datasheet Page 21)
typedef enum {
  MAX31856_FAULT_CJRANGE		=	0x80, // Cold Junction Out-of-Range
  MAX31856_FAULT_TCRANGE		=	0x40, // Thermocouple Out-of-Range
  MAX31856_FAULT_CJHIGH			= 0x20, // Cold-Junction High Fault
  MAX31856_FAULT_CJLOW			= 0x10, // Cold-Junction Low Fault
  MAX31856_FAULT_TCHIGH			= 0x08, // Thermocouple Temperature High Fault
  MAX31856_FAULT_TCLOW			= 0x04, // Thermocouple Temperature Low Fault
  MAX31856_FAULT_OVUV				= 0x02, // Overvoltage or Undervoltage Input Fault
  MAX31856_FAULT_OPEN				= 0x01, // Thermocouple Open-Circuit Fault
} MAX31856_Fault_MaskT;

// Noise filter option
typedef enum {
  MAX31856_NoiseFilter60Hz = 0x00,  // Selects rejection of 60Hz and its harmonics
  MAX31856_NoiseFilter50Hz = 0x01,  // Selects rejection of 50Hz and its harmonics
} MAX31856_FilterT;

class CNCxyz_MAX31856 {
public:
  CNCxyz_MAX31856(const int8_t cs);
  CNCxyz_MAX31856(const int8_t cs, const MAX31856_TC_TYPE_MaskT tc);
  CNCxyz_MAX31856(const int8_t cs, const int8_t mosi, const int8_t miso, const int8_t sck);
  CNCxyz_MAX31856(const int8_t cs, const int8_t mosi, const int8_t miso, const int8_t sck, 
    const MAX31856_TC_TYPE_MaskT tc);
  void begin(void);
  void setThermocoupleType(const MAX31856_TC_TYPE_MaskT tc);
  MAX31856_TC_TYPE_MaskT getThermocoupleType(void);
  float readThermocouple(void);
  float readColdJunction(void);
  void setAvergingMode(const MAX31856_AVGSEL_MaskT avgMask);
  uint8_t getAvergingMode(void);
  void setNoiseFilter(const MAX31856_FilterT filter);
  MAX31856_FilterT getNoiseFilter(void);

private:
  int8_t _cs;
  int8_t _sck;
  int8_t _miso;
  int8_t _mosi;
  MAX31856_TC_TYPE_MaskT _tc_type;

  uint8_t read(const MAX31856_addressT address);
  void readMultiple(const MAX31856_addressT address, uint8_t* const rx_buf, const uint8_t size);
  void write(const MAX31856_addressT address, const uint8_t value);
  void writeMultiple(const MAX31856_addressT address, const uint8_t* const tx_buf, 
    const uint8_t size);
  uint8_t transfer(const uint8_t val);
  void convert(void);
};

#endif
