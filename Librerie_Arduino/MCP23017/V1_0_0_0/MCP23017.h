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

#ifndef _MCP23017_H_
#define _MCP23017_H_

#define MCP23017_HW_ADD		0x20
#define MCP23017_INT_ERR 	255

#define CONFIGURATION_BANK0
//#define CONFIGURATION_BANK1

//------------------------------------------------------
//	definition of IOCON reg bit to bit
#define BIT_BANK	0x07
#define BIT_MIRROR	0x06
#define BIT_SEQOP	0x05
#define BIT_DISSLW	0x04
#define BIT_HAEN	0x03
#define BIT_ODR		0x02
#define BIT_INTPOL	0x01
//------------------------------------------------------

//------------------------------------------------------
//	Registers Address BANK = 0
#ifdef CONFIGURATION_BANK0
	#define MCP23017_BNK0_IODIRA   0x00
	#define MCP23017_BNK0_IODIRB   0x01
	#define MCP23017_BNK0_IPOLA    0x02
	#define MCP23017_BNK0_IPOLB    0x03
	#define MCP23017_BNK0_GPINTENA 0x04
	#define MCP23017_BNK0_GPINTENB 0x05
	#define MCP23017_BNK0_DEFVALA  0x06
	#define MCP23017_BNK0_DEFVALB  0x07
	#define MCP23017_BNK0_INTCONA  0x08
	#define MCP23017_BNK0_INTCONB  0x09
	#define MCP23017_BNK0_IOCONA   0x0A
	#define MCP23017_BNK0_IOCONB   0x0B
	#define MCP23017_BNK0_GPPUA    0x0C
	#define MCP23017_BNK0_GPPUB    0x0D
	#define MCP23017_BNK0_INTFA    0x0E
	#define MCP23017_BNK0_INTFB    0x0F
	#define MCP23017_BNK0_INTCAPA  0x10
	#define MCP23017_BNK0_INTCAPB  0x11
	#define MCP23017_BNK0_GPIOA    0x12
	#define MCP23017_BNK0_GPIOB    0x13
	#define MCP23017_BNK0_OLATA    0x14
	#define MCP23017_BNK0_OLATB    0x15
#endif
//------------------------------------------------------

//------------------------------------------------------
//	Registers Address BANK = 1
#ifdef CONFIGURATION_BANK1
	#define MCP23017_BNK1_IODIRA   0x00
	#define MCP23017_BNK1_IPOLA    0x01
	#define MCP23017_BNK1_GPINTENA 0x02
	#define MCP23017_BNK1_DEFVALA  0x03
	#define MCP23017_BNK1_INTCONA  0x04
	#define MCP23017_BNK1_IOCONA   0x05
	#define MCP23017_BNK1_GPPUA    0x06
	#define MCP23017_BNK1_INTFA    0x07
	#define MCP23017_BNK1_INTCAPA  0x08
	#define MCP23017_BNK1_GPIOA    0x09
	#define MCP23017_BNK1_OLATA    0x0A

	#define MCP23017_BNK1_IODIRB   0x10
	#define MCP23017_BNK1_IPOLB    0x11
	#define MCP23017_BNK1_GPINTENB 0x12
	#define MCP23017_BNK1_DEFVALB  0x13
	#define MCP23017_BNK1_INTCONB  0x14
	#define MCP23017_BNK1_IOCONB   0x15
	#define MCP23017_BNK1_GPPUB    0x16
	#define MCP23017_BNK1_INTFB    0x17
	#define MCP23017_BNK1_INTCAPB  0x18
	#define MCP23017_BNK1_GPIOB    0x19
	#define MCP23017_BNK1_OLATB    0x1A
#endif
//------------------------------------------------------

//------------------------------------------------------
//	Register Data
#define K_MCP23017_IODIR_A		0x00	//	I/O Director Register.      If "1" the corresponding pin becomes an input otherwise an output
#define K_MCP23017_IODIR_B		0xFF	//	I/O Director Register.      If "1" the corresponding pin becomes an input otherwise an output
#define K_MCP23017_IPOL_A		0x00	//	Input Polarity Register.    If "1" the corresponding GPIO register bit will reflect the inverted value on the pin
#define K_MCP23017_IPOL_B		0x00	//	Input Polarity Register.    If "1" the corresponding GPIO register bit will reflect the inverted value on the pin
#define K_MCP23017_GPINTEN_A	0x00	//	Interrupt on Change.        If "1" the corresponding pin is enabled for interrupt-on-change
#define K_MCP23017_GPINTEN_B	0x00	//	Interrupt on Change.        If "1" the corresponding pin is enabled for interrupt-on-change
#define K_MCP23017_DEFVAL_A		0x00	//	Default Compare Register.   If Interrupt-On-Change enabled an opposite value on the associated pin will cause an interrupt to occur
#define K_MCP23017_DEFVAL_B		0x00	//	Default Compare Register.   If Interrupt-On-Change enabled an opposite value on the associated pin will cause an interrupt to occur
#define K_MCP23017_INTCON_A		0x00	//	Interrupt Control Register. If "1" the corresponding I/O pin is compared against the associated bit in the DEFVAL register. Otherwise the corresponding I/O pin is compared against the previous value
#define K_MCP23017_INTCON_B		0x00	//	Interrupt Control Register. If "1" the corresponding I/O pin is compared against the associated bit in the DEFVAL register. Otherwise the corresponding I/O pin is compared against the previous value
#define K_MCP23017_IOCON_A		0x00	//	I/O Expander Configuration Register
										//	Bxxxxxxxx
										//	 ||||||||
										//	 |||||||+--->	Unimplemented
										//	 ||||||+---->	INTPOL -> Sets the polarity of the INT output pin. "1" Active High, "0" Active Low
										//	 |||||+----->	ODR    -> Configure INT pin as an open-drain output. "1" Open Drain Output, "0" Active Drive Output
										//	 ||||+------>	HAEN   -> For MCP23S17 Only.
										//	 |||+------->	DISSLW -> Slew Rate Control Bit for SDA Output. "1" slew rate disabled, "0" slew rate enabled
										//	 ||+-------->	SEQOP  -> Sequential Operation mode Bit. "1" Sequential operation disabled, address pointer does not increment, "0" Sequential operation enabled, address pointer increments
										//	 |+--------->	MIRROR -> INT Pins Mirror bit. "1" the INT pins are internally connected. "0" the INT pins are not connected. INTA is associated with PORTA and INTB is associated with PORTB
										//	 +---------->	BANK   -> Control how the registers are addressed. "1" the registers associated with each port are separeted into different banks. "0" The register are in the same bank
#define K_MCP23017_IOCON_B		0x00	//	I/O Expander Configuration Register
										//	Bxxxxxxxx
										//	 ||||||||
										//	 |||||||+--->	Unimplemented
										//	 ||||||+---->	INTPOL -> Sets the polarity of the INT output pin. "1" Active High, "0" Active Low
										//	 |||||+----->	ODR    -> Configure INT pin as an open-drain output. "1" Open Drain Output, "0" Active Drive Output
										//	 ||||+------>	HAEN   -> For MCP23S17 Only.
										//	 |||+------->	DISSLW -> Slew Rate Control Bit for SDA Output. "1" slew rate disabled, "0" slew rate enabled
										//	 ||+-------->	SEQOP  -> Sequential Operation mode Bit. "1" Sequential operation disabled, address pointer does not increment, "0" Sequential operation enabled, address pointer increments
										//	 |+--------->	MIRROR -> INT Pins Mirror bit. "1" the INT pins are internally connected. "0" the INT pins are not connected. INTA is associated with PORTA and INTB is associated with PORTB
										//	 +---------->	BANK   -> Control how the registers are addressed. "1" the registers associated with each port are separeted into different banks. "0" The register are in the same bank
#define K_MCP23017_GPPU_A		0x00	//	Pull-up Resistor configuration Register. If "1" and the corresponding pin is configured as an input the corresponding port pin is internally pulled-up with a 100K resistor
#define K_MCP23017_GPPU_B		0x00	//	Pull-up Resistor configuration Register. If "1" and the corresponding pin is configured as an input the corresponding port pin is internally pulled-up with a 100K resistor
//------------------------------------------------------

// Don't forget the Wire library
class MCP23017 {
public:
	void    Begin(void);
	void    ToggleSingleBit(uint8_t RegAdd, uint8_t Bit, uint8_t i2caddr);
	void    SetSingleBit(uint8_t RegAdd, uint8_t Bit, uint8_t i2caddr);
	void    ResetSingleBit(uint8_t RegAdd, uint8_t Bit, uint8_t i2caddr);
	void    WriteSingleReg(uint8_t RegAdd, uint8_t RegData, uint8_t i2caddr);
	void    ClearReg(uint8_t RegAdd, uint8_t i2caddr);
	uint8_t ReadSingleReg(uint8_t RegAdd, uint8_t i2caddr);
	
#ifdef CONFIGURATION_BANK0	
	void    SetAllRegBank0(uint8_t i2caddr);
	void	SetIntPortABank0(uint8_t Iocon, uint8_t Intcon, uint8_t Defval, uint8_t GpintEn, uint8_t i2caddr);
	void	SetIntPortBBank0(uint8_t Iocon, uint8_t Intcon, uint8_t Defval, uint8_t GpintEn, uint8_t i2caddr);
#endif
#ifdef CONFIGURATION_BANK1
	void	SetIntPortABank1(uint8_t Iocon, uint8_t Intcon, uint8_t Defval, uint8_t GpintEn, uint8_t i2caddr);
	void	SetIntPortBBank1(uint8_t Iocon, uint8_t Intcon, uint8_t Defval, uint8_t GpintEn, uint8_t i2caddr);
#endif

	union IoconReg {
		uint8_t  IoconByte;
		struct {
			uint8_t Free		:1;
			uint8_t IntPol		:1;
			uint8_t Odr			:1;
			uint8_t Haen		:1;
			uint8_t Disslw		:1;
			uint8_t Seqop		:1;
			uint8_t Mirror		:1;
			uint8_t Bank		:1;
		} Bit;	
	} IoconReg;

	union GpInten {
		uint8_t  GpIntenByte;
		struct {
			uint8_t Bit0		:1;
			uint8_t Bit1		:1;
			uint8_t Bit2		:1;
			uint8_t Bit3		:1;
			uint8_t Bit4		:1;
			uint8_t Bit5		:1;
			uint8_t Bit6		:1;
			uint8_t Bit7		:1;
		} Bit;	
	} GpInten;
	
	union DefVal {
		uint8_t  DefValByte;
		struct {
			uint8_t Bit0		:1;
			uint8_t Bit1		:1;
			uint8_t Bit2		:1;
			uint8_t Bit3		:1;
			uint8_t Bit4		:1;
			uint8_t Bit5		:1;
			uint8_t Bit6		:1;
			uint8_t Bit7		:1;
		} Bit;	
	} DefVal;
	
	union Intcon {
		uint8_t  IntconByte;
		struct {
			uint8_t Bit0		:1;
			uint8_t Bit1		:1;
			uint8_t Bit2		:1;
			uint8_t Bit3		:1;
			uint8_t Bit4		:1;
			uint8_t Bit5		:1;
			uint8_t Bit6		:1;
			uint8_t Bit7		:1;
		} Bit;	
	} Intcon;
	
private:
	uint8_t ConfReg[14];
	
#ifdef CONFIGURATION_BANK0
	void UpdateArrayDataBank0(void);
#endif
#ifdef CONFIGURATION_BANK1
	void UpdateArrayDataBank1(void);
#endif
};

#endif
