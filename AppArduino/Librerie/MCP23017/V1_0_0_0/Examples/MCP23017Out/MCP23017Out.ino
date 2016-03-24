/*************************************************************************************************************
#  This is an example and shows how to manage the output port of the MCP23017 device.
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

MCP23017 mcp23017 = MCP23017();

void setup() {
  mcp23017.Begin();
  //  To set the registers of the MCP23017 use this function. In the file MCP23017.h set the "#define"
  //  preprocessor to set a data for any single register
  mcp23017.SetAllRegBank0(0x01);
  //  Reset all bit of the PORTA output
  mcp23017.ClearReg(MCP23017_BNK0_OLATA, 0x01);
}

void loop() {
  for (int i=0; i < 8; i++) {
      //  This function is used to set a single bit on the PORTA configured as output (OLAT REGISTER)
      //  1° Parameter is Reg. Address, the 2° parameter is bit to set, the 3° parameter is device address
      mcp23017.SetSingleBit(MCP23017_BNK0_OLATA, i, 0x01);
      delay(500);
      //  This function is used to reset a single bit on the PORTA configured as output (OLAT REGISTER)
      //  1° Parameter is Reg. Address, the 2° parameter is bit to reset, the 3° parameter is device address
      mcp23017.ResetSingleBit(MCP23017_BNK0_OLATA, i, 0x01);
  }
  
  //  This function is used to write a single byte on the PORTA configured as output (OLAT REGISTER)
  mcp23017.WriteSingleReg(MCP23017_BNK0_OLATA, 0x55, 0x01);
  delay(500); 
  //  This function is used to write a single byte on the PORTA configured as output (OLAT REGISTER)
  mcp23017.WriteSingleReg(MCP23017_BNK0_OLATA, 0xAA, 0x01);
  delay(500);
  //  This function is used to write a single byte on the PORTA configured as output (OLAT REGISTER)
  mcp23017.WriteSingleReg(MCP23017_BNK0_OLATA, 0x33, 0x01);
  delay(500);
  //  This function is used to write a single byte on the PORTA configured as output (OLAT REGISTER)
  mcp23017.WriteSingleReg(MCP23017_BNK0_OLATA, 0xCC, 0x01);
  delay(500);
  //  This function is used to write a single byte on the PORTA configured as output (OLAT REGISTER)
  mcp23017.WriteSingleReg(MCP23017_BNK0_OLATA, 0x00, 0x01);
}
