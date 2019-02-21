#include "CNCxyz_MAX31856.h"

#include <stdlib.h>
#include <SPI.h>

static const SPISettings settings = SPISettings(500000, MSBFIRST, SPI_MODE1);
static const uint16_t T_CONV_60Hz_ms = 155;
static const uint16_t T_CONV_50Hz_ms = 185;

/**
    @brief  Basic constructor
    @param  cs [in]: pin used for CS signal
    @retval None
*/
CNCxyz_MAX31856::CNCxyz_MAX31856(const int8_t cs) : _cs(cs), 
_sck(-1), _miso(-1), _mosi(-1), _tc_type(MAX31856_TC_TYPE_K) {
}

/**
    @brief  Constructor with thermocouple definition
    @param  cs [in]: pin used for CS signal
    @param  tc [in]: thremocouple type
    @retval None
*/
CNCxyz_MAX31856::CNCxyz_MAX31856(const int8_t cs, const MAX31856_TC_TYPE_MaskT tc) :
  _cs(cs), _tc_type(tc), _sck(-1), _miso(-1), _mosi(-1) {
}

/**
    @brief  Constructor for software SPI
    @param  cs [in]: pin used for CS signal
    @param  mosi [in]: pin used for MOSI signal
    @param  miso [in]: pin used for MISO signal
    @param  sck [in]: pin used for SCK signal
    @retval None
*/
CNCxyz_MAX31856::CNCxyz_MAX31856(const int8_t cs, const int8_t mosi,
  const int8_t miso, const int8_t sck) : _cs(cs), _sck(sck), _miso(miso), _mosi(mosi),
  _tc_type(MAX31856_TC_TYPE_K) {  
}

/**
    @brief  Constructor for software SPI with thermocouple definition
    @param  cs [in]: pin used for CS signal
    @param  mosi [in]: pin used for MOSI signal
    @param  miso [in]: pin used for MISO signal
    @param  sck [in]: pin used for SCK signal
    @param  tc [in]: thremocouple type
    @retval None
*/
CNCxyz_MAX31856::CNCxyz_MAX31856(const int8_t cs, const int8_t mosi,
  const int8_t miso, const int8_t sck, const MAX31856_TC_TYPE_MaskT tc) :
  _cs(cs), _mosi(mosi), _miso(miso), _sck(sck), _tc_type(tc) {
}

/**
    @brief  Hardware configuration
    @param  None
    @retval None
*/
void CNCxyz_MAX31856::begin(void) {
  pinMode(_cs, OUTPUT);
  digitalWrite(_cs, HIGH);

  if (_sck != -1) {
    pinMode(_sck, OUTPUT);
    pinMode(_mosi, OUTPUT);
    pinMode(_miso, INPUT);
  } else {
    SPI.begin();
  }
  setThermocoupleType(_tc_type);
}

/**
    @brief  Setting thermocouple type
    @param  tc [in]: thremocouple type
    @retval None
*/
void CNCxyz_MAX31856::setThermocoupleType(const MAX31856_TC_TYPE_MaskT tc) {
  uint8_t CR1 = read(MAX31856_REG_CR1);
  CR1 &= 0xF0;
  CR1 |= (uint8_t)tc;
  write(MAX31856_REG_CR1, CR1);
}

/**
    @brief  Getting thermocouple type
    @param  None
    @retval Thermocouple type
*/
MAX31856_TC_TYPE_MaskT CNCxyz_MAX31856::getThermocoupleType(void) {
  MAX31856_TC_TYPE_MaskT x = (MAX31856_TC_TYPE_MaskT)(read(MAX31856_REG_CR1) & 0x0F);
  return x;
}

/**
    @brief  Reads hot junction temperature
    @param  None
    @retval Hot junction temperature in Celsius degrees
*/
float CNCxyz_MAX31856::readThermocouple(void) {
  convert(); // Perform single conversion

  // Read Linearized TC temperature registers
  uint8_t buf[3];
  readMultiple(MAX31856_REG_LTCBH, buf, 3);

  // Converting linearized TC temperature code to real temperature
  int32_t temp_code = ((int32_t)buf[0] << 16) | ((int32_t)buf[1] << 8) | ((int32_t)buf[2] & 0xE0);
  if (temp_code & 0x800000) {
    temp_code |= 0xFF000000;
  }
  temp_code >>= 5;
  float temperature = (float) temp_code / (1 << 7);

  return temperature;
}

/**
    @brief  Reads cold junction temperature
    @param  None
    @retval Cold junction temperature in Celsius degrees
*/
float CNCxyz_MAX31856::readColdJunction(void) {
  convert(); // Perform single conversion

  // Reading cold-junction temperature registers
  uint8_t buf[2];
  readMultiple(MAX31856_REG_CJTH, buf, 2);

  // Converting code to real temperature
  int16_t temp_code = (int16_t)buf[0] << 8 + (int16_t)buf[1];
  float temperature = (float)temp_code / (1 << 8);

  return temperature;
}

/**
    @brief  Sets thermocouple voltage conversion averaging mode
    @param  avgMask[in] : number of samples to be averaged during conversion
    @retval None
*/
void CNCxyz_MAX31856::setAvergingMode(const MAX31856_AVGSEL_MaskT avgMask) {
  uint8_t CR1 = read(MAX31856_REG_CR1);
  CR1 &= 0x8F;
  CR1 |= (uint8_t)avgMask;
  write(MAX31856_REG_CR1, CR1);
}

/**
    @brief  Gets thermocouple voltage conversion averaging mode
    @param  None
    @retval Number of samples used in one conversion
*/
uint8_t CNCxyz_MAX31856::getAvergingMode(void) {
  // Read the bitfield
  uint8_t avgMode = (read(MAX31856_REG_CR1) >> 4) & 0x07;

  // Calculate the number of samples
  uint8_t nSamples = 0;
  if (avgMode > 3) {
    nSamples = 16;
  } else {
    nSamples = 1 << avgMode;
  }
  return nSamples;
}

/**
    @brief  Sets noise rejection filter
    @param  filter[in] : filter option
    @retval None
*/
void CNCxyz_MAX31856::setNoiseFilter(const MAX31856_FilterT filter) {
  uint8_t CR0 = read(MAX31856_REG_CR0);
  CR0 &= ~0x01;

  // Set flag for 50Hz filter
  if (MAX31856_NoiseFilter50Hz == filter) {
    CR0 |= 0x01;
  }

  write(MAX31856_REG_CR0, CR0);
}

/**
    @brief  Gets moise rejection filter option
    @param  None
    @retval Noise rejection filter option
*/
MAX31856_FilterT CNCxyz_MAX31856::getNoiseFilter(void) {
  uint8_t filterFlag = read(MAX31856_REG_CR0) & 0x01;

  return filterFlag ? MAX31856_NoiseFilter50Hz : MAX31856_NoiseFilter60Hz;
}

//------------------------------ Private functions ----------------------------
/**
    @brief  Read MAX31856 register
    @param  address [in]: register address
    @retval Register value
*/
uint8_t CNCxyz_MAX31856::read(const MAX31856_addressT address) {
  uint8_t value;
  readMultiple(address, &value, 1);
  return value;
}

/**
    @brief  Read MAX31856 multiple registers
    @param  address [in]: register address
    @param  rx_buf [out]: receiving buffer pointer
    @param  size [in]: number of bytes to read
    @retval None
*/
void CNCxyz_MAX31856::readMultiple(const MAX31856_addressT address, uint8_t* const rx_buf, const uint8_t size) {
  //Start Transfer, Open Connection
  if (_sck == -1) {
    SPI.beginTransaction(settings);
  } else {
    digitalWrite(_sck, HIGH);
  }
  digitalWrite(_cs, LOW);

  // Send address and read specified number of bytes
  transfer(address);
  for (uint8_t i = 0; i < size; ++i) {
    rx_buf[i] = transfer(0xFF);
  }

  //End Transfer, Close Connection for other programs to use.
  if (_sck == -1) {
    SPI.endTransaction();
  }
  digitalWrite(_cs, HIGH);
}

/**
    @brief  Write MAX31856 register
    @param  address [in]: register address
    @param  value [in]: register value
    @retval None
*/
void CNCxyz_MAX31856::write(const MAX31856_addressT address, const uint8_t value) {
  writeMultiple(address, &value, 1);
}

/**
    @brief  Write MAX31856 register
    @param  address [in]: register address
    @param  tx_buf [in]: data buffer pointer
    @param  size [in]: number of bytes to write
    @retval None
*/
void CNCxyz_MAX31856::writeMultiple(const MAX31856_addressT address, 
  const uint8_t* const tx_buf, const uint8_t size) {

  //Start Transfer, Open Connection
  if (_sck == -1) {
    SPI.beginTransaction(settings);
  } else {
    digitalWrite(_sck, HIGH);
  }
  digitalWrite(_cs, LOW);

  // Send address and specified number of bytes
  transfer(address | 0x80);
  for (uint8_t i = 0; i < size; ++i) {
    transfer(tx_buf[i]);
  }

  //End Transfer, Close Connection for other programs to use.
  if (_sck == -1) {
    SPI.endTransaction();
  }
  digitalWrite(_cs, HIGH);
}

/**
    @brief  SPI byte transfer
    @param  val [in]: byte to transfer
    @retval Received byte
*/
uint8_t CNCxyz_MAX31856::transfer(const uint8_t val) {

  //If Hardware SPI, use default SPI class.
  if (_sck == -1)
    return SPI.transfer(val);

  //If Software SPI, bitbang our way to glory.
  uint8_t out = 0;
  uint8_t bval = 0;
  for (uint8_t bit = 0; bit < 8; bit++) {
    digitalWrite(_sck, HIGH);
    bval = digitalRead(_miso);
    out >>= 1;
    out |= bval << 7;
    digitalWrite(_sck, LOW);
    digitalWrite(_mosi, val & (1 << bit) ? HIGH : LOW);
  }
  return out;
}

/**
    @brief  Execute temperature conversion
    @param  None
    @retval None
*/
void CNCxyz_MAX31856::convert(void) {
  // Get current settings  
  uint16_t convertionTime_ms;
  uint8_t samples = getAvergingMode();
  MAX31856_FilterT noiseFilter = getNoiseFilter();
  
  // Start single conversion
  uint8_t CR0 = read(MAX31856_REG_CR0);
  CR0 &= ~MAX31856_REG_CR0_AUTOCONVERT;
  CR0 |= MAX31856_REG_CR0_1SHOT;
  write(MAX31856_REG_CR0, CR0);

  // Calculate conversion time(p.20)
  switch (noiseFilter) {
    case MAX31856_NoiseFilter60Hz:
      convertionTime_ms = T_CONV_60Hz_ms + (samples - 1) * 34;
      break;
    case MAX31856_NoiseFilter50Hz:
      convertionTime_ms = T_CONV_50Hz_ms + (samples - 1) * 40;
      break;
  }

  // Wait until the end of conversion
  delay(convertionTime_ms);
}
