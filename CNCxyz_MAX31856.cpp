#include "CNCxyz_MAX31856.h"

#include <stdlib.h>
#include <SPI.h>

const SPISettings = SPISettings(500000, MSBFIRST, SPI_MODE1);

boolean CNCxyz_MAX31856::begin(void) {
	pinMode(_cs, OUTPUT);
	digitalWrite(_cs, HIGH);

	if (_sck != -1) {
		pinMode(_sck, OUTPUT);
		pinMode(_mosi, OUTPUT);
		pinMode(_miso, INPUT);
	} else {
		SPI.begin();
	}

	return true;
}

// If using hardware SPI, all we need is the chipselect pin.
CNCxyz_MAX31856::CNCxyz_MAX31856(int8_t cs) {
	_cs = cs;
	_sck = _miso = _mosi = -1;
	_tc_type = MAX31856_TC_TYPE_K;
}

// We can also specify the Thermocouple type.
CNCxyz_MAX31856::CNCxyz_MAX31856(int8_t cs, int8_t tc) {
	_cs = cs;
	_sck = _miso = _mosi = -1;
	_tc_type = tc;
}

// If using software SPI, we need all pins for bitbanging.
CNCxyz_MAX31856::CNCxyz_MAX31856(int8_t cs, int8_t mosi, int8_t miso, int8_t sck) {
	_sck = sck;
	_cs = cs;
	_miso = miso;
	_mosi = mosi;
	_tc_type = MAX31856_TC_TYPE_K;
}

// We can also specify the Thermocouple type.
CNCxyz_MAX31856::CNCxyz_MAX31856(int8_t cs, int8_t mosi, int8_t miso, int8_t sck, int8_t tc) {
	_sck = sck;
	_cs = cs;
	_miso = miso;
	_mosi = mosi;
	_tc_type = tc;
}

void CNCxyz_MAX31856::setThermocoupleType(int8_t tc){
	uint8_t x = read(MAX31856_REG_CR1, 1);
	x &= 0xF0;
	x |= (uint8_t)tc & 0x0F;
	write(MAX31856_REG_CR1, x);
}

uint8_t CNCxyz_MAX31856::getThermocoupleType(){
	uint8_t x = read(MAX31856_REG_CR1, 1);
	x &= 0x0F;
	return x;
}

float CNCxyz__MAX31856::readThermocouple(void) {

}

float CNCxyz_MAX31856::readColdJunction(void) {

}

uint8_t CNCxyz_MAX31856::readFault(void) {
	return read(MAX31856_REG_SR, 1);
}

uint8_t CNCxyz_MAX31856::read(uint8_t address, uint8_t byte){

	//Start Transfer, Open Connection
	if (_sck == -1) {
		SPI.beginTransaction(SPISettings);
	}else {
		digitalWrite(_sck, HIGH);
	}
	digitalWrite(_cs, LOW);

	transfer(address);
	out = transfer(byte);

	//End Transfer, Close Connection for other programs to use.
	if (_sck == -1) {
		SPI.endTransaction();
	}

	digitalWrite(_cs, HIGH);
	return out;

}

uint8_t CNCxyz_MAX31856::write(uint8_t address, uint8_t data){

	//Start Transfer, Open Connection
	if (_sck == -1) {
		SPI.beginTransaction(SPISettings);
	}else {
		digitalWrite(_sck, HIGH);
	}
	digitalWrite(_cs, LOW);

	transfer(address);
	transfer(data);

	//End Transfer, Close Connection for other programs to use.
	if (_sck == -1) {
		SPI.endTransaction();
	}
	digitalWrite(_cs, HIGH);

}

uint8_t CNCxyz_MAX31856::transfer(uint8_t val) {

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