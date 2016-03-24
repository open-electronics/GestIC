/*************************************************************************************************************
#  This is an example and shows how to manage the input port of the MCP23017 device.
#  The hardware used is referred to "Shield I/O expander for arduino" developed by 
#  Futura Elettronica srl. The code for this kit is FT1079K.
#
#  The PORTA of the MCP23017 is used as output and the PORTB as input. In this example
#  we will show you how to configure the device and how to read the PORTB input
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

//#define INPUTtoOUTPUT;  //  Activates the code to report the input status on the output
#define TOGGLE_OUTPUT;  // Activates the code to toggle the output state of the PortA related at the state of the input portB

MCP23017 mcp23017 = MCP23017();

byte InputReaded;    //  Input state of the portB read from the MCP23017 device
byte InputIvar;      //  Saves the variation of the input
byte InputStatus;    //  The stable state after debouncing
byte OutputStatus;
byte TempPortAValue;

long LastDebouncingTime = 0;
long DebouncingDelay    = 100;  //  Debouncing time 100ms

void setup() {
  mcp23017.Begin();
  //  To set the registers of the MCP23017 use this function. In the file MCP23017.h set the "#define"
  //  preprocessor to set a data for any single register
  mcp23017.SetAllRegBank0(0x00);
  //  Reset all bit of the PORTA output (This is an optional configuration for this example)
  mcp23017.ClearReg(MCP23017_BNK0_OLATA, 0x00);  
}

void loop() {
  DebouncingInput();
  
#ifdef INPUTtoOUTPUT
  //  This piece of code reports the input status of the portB on the OLATA register of the portA Output
  if ((InputStatus ^ OutputStatus) != 0) {
    OutputStatus = InputStatus;
    mcp23017.WriteSingleReg(MCP23017_BNK0_OLATA, ~InputStatus, 0x00);
  }
#endif

#ifdef TOGGLE_OUTPUT
  //  This piece of code toggled the output bit of the portA when the state of the bit of the portB changes
  if ((InputStatus ^ OutputStatus) != 0) {
    OutputStatus = InputStatus;
    TempPortAValue = mcp23017.ReadSingleReg(MCP23017_BNK0_GPIOA, 0x00);
    mcp23017.WriteSingleReg(MCP23017_BNK0_OLATA, (TempPortAValue ^ ~InputStatus), 0x00);
  }
#endif
}

void DebouncingInput(void) {
  InputReaded = mcp23017.ReadSingleReg(MCP23017_BNK0_GPIOB, 0x00);
  if (InputReaded != InputStatus) {
    if (InputReaded != InputIvar) {
       InputIvar = InputReaded;
       LastDebouncingTime = millis();
    } else {
      if ((millis() - LastDebouncingTime) > DebouncingDelay) {
         InputIvar   = InputReaded;
         InputStatus = InputReaded;
      }
    }
  } else {
      InputIvar = InputStatus;
  }
}
