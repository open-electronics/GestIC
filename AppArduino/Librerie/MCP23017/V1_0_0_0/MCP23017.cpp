/*  ===========================================================================
#  This is the library for MCP23017 I2C I/O expander. 
#  
#  Written by Matteo Destro for Futura Group srl
#  www.Futurashop.it
#  www.open-electronics.org
# 
#  BSD license, all text above must be included in any redistribution
#  ===========================================================================
#
#	REVISION	1.0.0	11/05/2015
#
#  ===========================================================================
#
#  INSTALLATION
#  The 3 library files (MCP23017.cpp, MCP23017.h and keywords.txt) in the MCP123017
#  folder should be placed in your Arduino Library folder.
#
#  SUPPORT
#
#  info@open-electronics.org
#
#  ===========================================================================*/

#include <Wire.h>
#include "MCP23017.h"
#ifdef __AVR__
#include <avr/pgmspace.h>
#endif

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

// minihelper to keep Arduino backward compatibility
static inline void wiresend(uint8_t x) {
#if ARDUINO >= 100
	Wire.write((uint8_t) x);
#else
	Wire.send(x);
#endif
}

static inline uint8_t wirerecv(void) {
#if ARDUINO >= 100
	return Wire.read();
#else
	return Wire.receive();
#endif
}

//-----------------------------------------------------
//	Public Function
//	addr = device address (0 to 7. See A0, A1 e A2 configurations)
void MCP23017::Begin(void) {
	Wire.begin();
}

//-----------------------------------------------------
//	This function toggles a single bit into selected register
//  RegAdd  = Register address
//	Bit     = Bit to set
//	i2caddr = device address (0 to 7. See A0, A1 e A2 configurations)
void MCP23017::ToggleSingleBit(uint8_t RegAdd, uint8_t Bit, uint8_t i2caddr) {
	uint8_t RegData;
	if (i2caddr > 7) { i2caddr = 7; }
	if (Bit > 7) { Bit = 7; }
	
	Wire.beginTransmission(MCP23017_HW_ADD | i2caddr);
	wiresend(RegAdd);
	Wire.endTransmission();
	Wire.requestFrom(MCP23017_HW_ADD | i2caddr, 1);
	RegData = wirerecv();

	RegData ^= (0x01 << Bit);

	Wire.beginTransmission(MCP23017_HW_ADD | i2caddr);
	wiresend(RegAdd);
	wiresend(RegData);
	Wire.endTransmission();	
}
//-----------------------------------------------------

//-----------------------------------------------------
//	This function sets a single bit into selected register
//  RegAdd  = Register address
//	Bit     = Bit to set
//	i2caddr = device address (0 to 7. See A0, A1 e A2 configurations)
void MCP23017::SetSingleBit(uint8_t RegAdd, uint8_t Bit, uint8_t i2caddr) {
	uint8_t RegData;
	if (i2caddr > 7) { i2caddr = 7; }
	if (Bit > 7) { Bit = 7; }
	
	Wire.beginTransmission(MCP23017_HW_ADD | i2caddr);
	wiresend(RegAdd);
	Wire.endTransmission();
	Wire.requestFrom(MCP23017_HW_ADD | i2caddr, 1);
	RegData = wirerecv();

	RegData |= (0x01 << Bit);

	Wire.beginTransmission(MCP23017_HW_ADD | i2caddr);
	wiresend(RegAdd);
	wiresend(RegData);
	Wire.endTransmission();	
}
//-----------------------------------------------------

//-----------------------------------------------------
//	This function resets a single bit into selected register
//  RegAdd  = Register address
//	Bit     = Bit to reset
//	i2caddr = device address (0 to 7. See A0, A1 e A2 configurations)
void MCP23017::ResetSingleBit(uint8_t RegAdd, uint8_t Bit, uint8_t i2caddr) {
	uint8_t RegData;
	if (i2caddr > 7) { i2caddr = 7; }
	if (Bit > 7) { Bit = 7; }
	
	Wire.beginTransmission(MCP23017_HW_ADD | i2caddr);
	wiresend(RegAdd);
	Wire.endTransmission();
	Wire.requestFrom(MCP23017_HW_ADD | i2caddr, 1);
	RegData = wirerecv();
	
	RegData &= ~(0x01 << Bit);
	
	Wire.beginTransmission(MCP23017_HW_ADD | i2caddr);
	wiresend(RegAdd);
	wiresend(RegData);
	Wire.endTransmission();	
}
//-----------------------------------------------------

//-----------------------------------------------------
//	This function writes a single register
//  RegAdd  = Register address
//	RegData = Data to write into register selected
//	i2caddr = device address (0 to 7. See A0, A1 e A2 configurations)
void MCP23017::WriteSingleReg(uint8_t RegAdd, uint8_t RegData, uint8_t i2caddr) {
	if (i2caddr > 7) { i2caddr = 7; }
	Wire.beginTransmission(MCP23017_HW_ADD | i2caddr);
	wiresend(RegAdd);
	wiresend(RegData);
	Wire.endTransmission();	
}
//-----------------------------------------------------

//-----------------------------------------------------
//	This function clear a single register
//  RegAdd  = Register address
//	i2caddr = device address (0 to 7. See A0, A1 e A2 configurations)
void MCP23017::ClearReg(uint8_t RegAdd, uint8_t i2caddr) {
	if (i2caddr > 7) { i2caddr = 7; }
	Wire.beginTransmission(MCP23017_HW_ADD | i2caddr);
	wiresend(RegAdd);
	wiresend(0x00);
	Wire.endTransmission();	
}
//-----------------------------------------------------

//-----------------------------------------------------
//	This function read a single register
//  RegAdd  = Register address
//	i2caddr = device address (0 to 7. See A0, A1 e A2 configurations)
//	This function return the value of selected register
uint8_t MCP23017::ReadSingleReg(uint8_t RegAdd, uint8_t i2caddr) {
	if (i2caddr > 7) { i2caddr = 7; }
	Wire.beginTransmission(MCP23017_HW_ADD | i2caddr);
	wiresend(RegAdd);
	Wire.endTransmission();
	Wire.requestFrom(MCP23017_HW_ADD | i2caddr, 1);
	return wirerecv();	
}
//-----------------------------------------------------

//-----------------------------------------------------
//	This functions sets all registers for the configuration BANK0
//	i2caddr = device address (0 to 7. See A0, A1 e A2 configurations)
#ifdef CONFIGURATION_BANK0
	void MCP23017::SetAllRegBank0(uint8_t i2caddr) {
		uint8_t Count = 0;
		
		if (i2caddr > 7) { i2caddr = 7; }
		UpdateArrayDataBank0();
		Wire.beginTransmission(MCP23017_HW_ADD | i2caddr);
		wiresend(MCP23017_BNK0_IODIRA);
		do {
			wiresend(ConfReg[Count]);
		} while (Count++ < sizeof(ConfReg)); 
		Wire.endTransmission();
	}
	
	void MCP23017::UpdateArrayDataBank0(void) {
		ConfReg[0]  = K_MCP23017_IODIR_A;
		ConfReg[1]  = K_MCP23017_IODIR_B;
		ConfReg[2]  = K_MCP23017_IPOL_A;
		ConfReg[3]  = K_MCP23017_IPOL_B;
		ConfReg[4]  = K_MCP23017_GPINTEN_A;
		ConfReg[5]  = K_MCP23017_GPINTEN_B;
		ConfReg[6]  = K_MCP23017_DEFVAL_A;
		ConfReg[7]  = K_MCP23017_DEFVAL_B;
		ConfReg[8]  = K_MCP23017_INTCON_A;
		ConfReg[9]  = K_MCP23017_INTCON_B;
		ConfReg[10] = K_MCP23017_IOCON_A;
		ConfReg[11] = K_MCP23017_IOCON_B;
		ConfReg[12] = K_MCP23017_GPPU_A;
		ConfReg[13] = K_MCP23017_GPPU_B;
	}
#endif
//-----------------------------------------------------

//-----------------------------------------------------
//	This functions sets all registers for the configuration BANK1
//	i2caddr = device address (0 to 7. See A0, A1 e A2 configurations)
#ifdef CONFIGURATION_BANK1
	void MCP23017::SetAllRegBank1(uint8_t i2caddr) {
		uint8_t Count = 0;
		
		if (i2caddr > 7) { i2caddr = 7; }
		UpdateArrayDataBank1();
		Wire.beginTransmission(MCP23017_HW_ADD | i2caddr);
		wiresend(MCP23017_BNK1_IODIRA);
		do {
			wiresend(ConfReg[Count]);
		} while (Count++ < sizeof(ConfReg)); 
		Wire.endTransmission();
	}
	
	void MCP23017::UpdateArrayDataBank1(void) {
		ConfReg[0]  = K_MCP23017_IODIR_A;
		ConfReg[1]  = K_MCP23017_IPOL_A;
		ConfReg[2]  = K_MCP23017_GPINTEN_A;
		ConfReg[3]  = K_MCP23017_DEFVAL_A;
		ConfReg[4]  = K_MCP23017_INTCON_A;
		ConfReg[5]  = K_MCP23017_IOCON_A;
		ConfReg[6]  = K_MCP23017_GPPU_A;
		ConfReg[7]  = K_MCP23017_IODIR_B;
		ConfReg[8]  = K_MCP23017_IPOL_B;
		ConfReg[9]  = K_MCP23017_GPINTEN_B;
		ConfReg[10] = K_MCP23017_DEFVAL_B;
		ConfReg[11] = K_MCP23017_INTCON_B;
		ConfReg[12] = K_MCP23017_IOCON_B;
		ConfReg[13] = K_MCP23017_GPPU_B;
	}
#endif
//-----------------------------------------------------

/*  ===========================================================================
#	INTERRUPT ON PIN CHANGE.
#	When GPx changes the interrupt occurs
#	
#   ¯¯¯¯¯\_/¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯\_/¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯\_
#   GPx   |                     |                     |  ...................
#   _____/¯\___________________/¯\___________________/¯
#         
#   ¯¯¯¯¯\_/¯¯¯¯¯¯¯¯\_/¯¯¯¯¯¯¯¯\_/¯¯¯¯¯¯¯¯\_/¯¯¯¯¯¯¯¯\_
#    INT  |  ACTIVE  |          |  ACTIVE  |          |  ...................
#   _____/¯\________/¯\________/¯\________/¯\________/¯
#
#	===========================================================================
#	INTERRUPT ON CHANGE FROM REGISTER DEFAULT.
#	When GPx matches to DEFVAL bit the interrupt occurs
#
#    7   6   5   4   3   2   1   0
#   _______________________________
#  | X | X | X | X | X | 0 | X | X | 	DEFVAL REGISTER
#   ¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯ 
#
#   ¯¯¯¯¯¯|      |¯¯¯¯¯¯¯¯¯¯¯¯¯¯|       |¯¯¯¯¯¯¯¯¯¯
#   GP2   |______|              |_______|                ...................
#
#   ¯¯¯¯¯\_/¯¯¯¯¯¯¯¯\_/¯¯¯¯¯¯¯¯\_/¯¯¯¯¯¯¯¯\_/¯¯¯¯¯¯¯¯\_
#    INT  |  ACTIVE  |          |  ACTIVE  |          |  ...................
#   _____/¯\________/¯\________/¯\________/¯\________/¯
#
#	How to set registers step by step:
#   1) Set MIRROR bit, INTPOL bit and ODR bit in IOCON Register
#   1) Set INTCON  REGISTER
#	2) Set DEFVAL  REGISTER 
#   3) Set GPINTEN REGISTGER
#
#	How to read interrupt occurred:
#	1) The INTF register reflects the interrupt condition on the port. Is set indicates that the associated pin caused interrupt
#	2) The INTCAP register capture the GPIO port value at the time the interrupt occurred. The register will remain unchanged
#      until the interrupt is cleared via a read of INTACP or GPIO register
#
#	The INTA and INTB pins if connected to the Arduino hardware are useful
#	for intercept if the interrupt occurred
#
#  ===========================================================================*/

//-----------------------------------------------------
//	This functions sets interrupt registers for the configuration BANK0
//	i2caddr = device address (0 to 7. See A0, A1 e A2 configurations)
//
//	If Mirror Bit = "1" the INT pins are internally connected, if "0" the INT pins
//	are not connected. INTA is associated with PORTA and INTB is associated with PORTB
//
//	If INTPOL Bit = "1" the polarity of the INT output pin is Active-High, if "0" Active-Low
//
//	If ODR Bit = "1" configures INT pin as Open-Drain
#ifdef CONFIGURATION_BANK0
	void MCP23017::SetIntPortABank0(uint8_t Iocon, uint8_t Intcon, uint8_t Defval, uint8_t GpintEn, uint8_t i2caddr) {
		WriteSingleReg(MCP23017_BNK0_IOCONA,   Iocon,   i2caddr);
		WriteSingleReg(MCP23017_BNK0_INTCONA,  Intcon,  i2caddr);
		WriteSingleReg(MCP23017_BNK0_DEFVALA,  Defval,  i2caddr);
		WriteSingleReg(MCP23017_BNK0_GPINTENA, GpintEn, i2caddr);
	}		
			
	void MCP23017::SetIntPortBBank0(uint8_t Iocon, uint8_t Intcon, uint8_t Defval, uint8_t GpintEn, uint8_t i2caddr) {
		WriteSingleReg(MCP23017_BNK0_IOCONB,   Iocon,   i2caddr);
		WriteSingleReg(MCP23017_BNK0_INTCONB,  Intcon,  i2caddr);
		WriteSingleReg(MCP23017_BNK0_DEFVALB,  Defval,  i2caddr);
		WriteSingleReg(MCP23017_BNK0_GPINTENB, GpintEn, i2caddr);
	}
#endif
//-----------------------------------------------------

//-----------------------------------------------------
//	This functions sets interrupt registers for the configuration BANK1
//	i2caddr = device address (0 to 7. See A0, A1 e A2 configurations)
#ifdef CONFIGURATION_BANK1
	void MCP23017::SetIntPortABank1(uint8_t Iocon, uint8_t Intcon, uint8_t Defval, uint8_t GpintEn, uint8_t i2caddr) {
		WriteSingleReg(MCP23017_BNK1_IOCONA,   Iocon,   i2caddr);
		WriteSingleReg(MCP23017_BNK1_INTCONA,  Intcon,  i2caddr);
		WriteSingleReg(MCP23017_BNK1_DEFVALA,  Defval,  i2caddr);
		WriteSingleReg(MCP23017_BNK1_GPINTENA, GpintEn, i2caddr);
	}		
			
	void MCP23017::SetIntPortBBank1(uint8_t Iocon, uint8_t Intcon, uint8_t Defval, uint8_t GpintEn, uint8_t i2caddr) {
		WriteSingleReg(MCP23017_BNK1_IOCONB,   Iocon,   i2caddr);
		WriteSingleReg(MCP23017_BNK1_INTCONB,  Intcon,  i2caddr);
		WriteSingleReg(MCP23017_BNK1_DEFVALB,  Defval,  i2caddr);
		WriteSingleReg(MCP23017_BNK1_GPINTENB, GpintEn, i2caddr);
	}
#endif
//-----------------------------------------------------
