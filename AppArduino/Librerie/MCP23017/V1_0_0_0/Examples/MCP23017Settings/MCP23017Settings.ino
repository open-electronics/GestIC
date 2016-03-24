/*************************************************************************************************************
#  This is an example and shows how to configure the registers of the MCP23017 device.
#  The hardware used is referred to "Shield I/O expander for arduino" developed by 
#  Futura Elettronica srl. The code for this kit is FT1079K.
#
#  The PORTA of the MCP23017 is used as output and the PORTB as input. In this example
#  we will show you how to configure the device and how set or reset the PORTA output
#
#  Arduino Uno Rev. 3
#   
#  =============================================================================
#
#  OUTPUT DEFINITION
#                         
#  HISTORY
#  v1.0  -  Initial Release
#  
#  INSTALLATION
#  The 3 library files (MCP23017.cpp, MCP23017.h and keywords.txt) in the MCP23017 folder should be placed in your Arduino Library folder.
#  Run the MCP23017Out.ino file from your Arduino IDE.
#
#  SUPPORT
#  
#
*********************************************************************************************************/

#include <Wire.h>
#include "MCP23017.h"

//#define SET_ALL_IN_ONE;
#define SET_ONE_BY_ONE;

MCP23017 mcp23017 = MCP23017();

void setup() {
  mcp23017.Begin();

#ifdef SET_ALL_IN_ONE
  //  When using this function the user must configure the "#define const data" in the file MCP23017.h
  //  Automatically the function uses this data to configure the MCP23017 device
  mcp23017.SetAllRegBank0(0x00);
#endif

#ifdef SET_ONE_BY_ONE
  //  If the user prefers it is possible configure the register one by one.
  //  Below the configuration for the kit FT1079K I/O expander by futura Elettronica
  //  The "WriteSingleReg" function request:
  //  1) Register address
  //  2) Register data
  //  3) Device address (0 to 7). Do not confuse with the unique hardware address of MCP23017
  mcp23017.WriteSingleReg(MCP23017_BNK0_IODIRA,   0x00, 0x00);  //  Set portA as output. If "1" the corresponding pin becomes an input otherwise an output
  mcp23017.WriteSingleReg(MCP23017_BNK0_IODIRB,   0xFF, 0x00);  //  Set portB as input.  If "1" the corresponding pin becomes an input otherwise an output
  mcp23017.WriteSingleReg(MCP23017_BNK0_IPOLA,    0x00, 0x00);  //  Set input polarity portA. If "1" the corresponding GPIO register bit will reflect the inverted value on the pin
  mcp23017.WriteSingleReg(MCP23017_BNK0_IPOLB,    0x00, 0x00);  //  Set input polarity portB. If "1" the corresponding GPIO register bit will reflect the inverted value on the pin
  mcp23017.WriteSingleReg(MCP23017_BNK0_GPINTENA, 0x00, 0x00);  //  Interrupt on Change portA. If "1" the corresponding pin is enabled for interrupt-on-change
  mcp23017.WriteSingleReg(MCP23017_BNK0_GPINTENB, 0x00, 0x00);  //  Interrupt on Change portB. If "1" the corresponding pin is enabled for interrupt-on-change
  mcp23017.WriteSingleReg(MCP23017_BNK0_DEFVALA,  0x00, 0x00);  //  Default Compare Register portA. If Interrupt-On-Change enabled an opposite value on the associated pin will causde an interrupt to occur
  mcp23017.WriteSingleReg(MCP23017_BNK0_DEFVALB,  0x00, 0x00);  //  Default Compare Register portB. If Interrupt-On-Change enabled an opposite value on the associated pin will causde an interrupt to occur
  mcp23017.WriteSingleReg(MCP23017_BNK0_INTCONA,  0xFF, 0x00);  //  Interrupt Control Register. If "1" the corresponding I/O pin is compared against the associated bit in the DEFVAL register. Otherwise the corrisponding I/O pin is compared against the previous value
  mcp23017.WriteSingleReg(MCP23017_BNK0_INTCONB,  0xFF, 0x00);  //  Interrupt Control Register. If "1" the corresponding I/O pin is compared against the associated bit in the DEFVAL register. Otherwise the corrisponding I/O pin is compared against the previous value
  mcp23017.WriteSingleReg(MCP23017_BNK0_IOCONA,   0x04, 0x00);  //  I/O Expander Configuration Register portA.
  mcp23017.WriteSingleReg(MCP23017_BNK0_IOCONB,   0x04, 0x00);  //  I/O Expander Configuration Register portB.
  							        //  Bxxxxxxxx
								//   ||||||||
								//   |||||||+--->  Unimplemented
								//   ||||||+---->  INTPOL -> Sets the polarity of the INT output pin. "1" Active High, "0" Active Low
								//   |||||+----->  ODR    -> Configure INT pin as an open-drain output. "1" Open Drain Output, "0" Active Drive Output
								//   ||||+------>  HAEN   -> For MCP23S17 Only.
								//   |||+------->  DISSLW -> Slew Rate Control Bit for SDA Output. "1" slew rate disabled, "0" slew rate enabled
								//   ||+-------->  SEQOP  -> Sequential Operation mode Bit. "1" Sequential operation disabled, address pointer does not increment, "0" Sequential operation enabled, address pointer increments
								//   |+--------->  MIRROR -> INT Pins Mirror bit. "1" the INT pins are internally connected. "0" the INT pins are not connected. INTA is associated with PORTA and INTB is associated with PORTB
								//   +---------->  BANK   -> Control how the registers are addressed. "1" the registers associated with each port are separeted into different banks. "0" The register are in the same bank
  mcp23017.WriteSingleReg(MCP23017_BNK0_GPPUA,    0x00, 0x00);  // Pull-up Resistor configuration Register portA. If "1" and the corresponding pin is configured as an input the corresponding port pin is internally pulled-up with a 100K resistor
  mcp23017.WriteSingleReg(MCP23017_BNK0_GPPUB,    0x00, 0x00);  // Pull-up Resistor configuration Register portB. If "1" and the corresponding pin is configured as an input the corresponding port pin is internally pulled-up with a 100K resistor
#endif

}

void loop() {
  for (int i=0; i < 8; i++) {
      //  This function is used to set a single bit on the PORTA configured as output (OLAT REGISTER)
      //  1° Parameter is Reg. Address, the 2° parameter is bit to set, the 3° parameter is device address
      mcp23017.SetSingleBit(MCP23017_BNK0_OLATA, i, 0x00);
      delay(500);
      //  This function is used to reset a single bit on the PORTA configured as output (OLAT REGISTER)
      //  1° Parameter is Reg. Address, the 2° parameter is bit to reset, the 3° parameter is device address
      mcp23017.ResetSingleBit(MCP23017_BNK0_OLATA, i, 0x00);
  }
}
