/*************************************************************************************************************
#  This is an example and shows how to manage the interrupt on the input port of the MCP23017 device.
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

#define DEBOUNCING_IDLE      0
#define DEBOUNCING_PROGRESS  1

MCP23017 mcp23017 = MCP23017();

int MCP23017IntPin = 2;  //  Interrupt MCP23017 pin

byte InputReaded;        //  Input state of the portB read from the MCP23017 device
byte InputVar;           //  Saves the variation of the input
byte InputStatus;        //  The stable state after debouncing
byte OutputStatus;
byte TempPortAValue;

byte IntReaded;          //  Input state of the interrupt pin read from the MCP23017 device
byte IntVar;             //  Saves the variation of the interrupt pin
byte IntStatus;          //  The stable state after debouncing of interrupt pin

byte DebouncingState;    //  Debouncing State Machine

long LastDebouncingTime = 0;
long DebouncingDelay    = 100;    //  Debouncing time 100ms

long IntLastDebouncingTime = 0;
long IntDebouncingDelay    = 2;  //  Debouncing time 50ms

void setup() {  
  pinMode(MCP23017IntPin, INPUT); 
  IntStatus   = 0xFF;  //  Set default value of interrupt status
  InputStatus = 0xFF;  //  Set default value of input status
  DebouncingState = DEBOUNCING_IDLE;
  
  //----------------------------------------------
  mcp23017.Begin();
  //  To set the registers of the MCP23017 use this function. In the file MCP23017.h set the "#define"
  //  preprocessor to set a data for any single register
  mcp23017.SetAllRegBank0(0x00);
  //----------------------------------------------
  
  //----------------------------------------------
  //  Set Interrupt function for portB input
  LoadRegisterForInterrupt();
  mcp23017.SetIntPortBBank0(mcp23017.IoconReg.IoconByte, mcp23017.Intcon.IntconByte, mcp23017.DefVal.DefValByte, mcp23017.GpInten.GpIntenByte, 0x00);
  //----------------------------------------------

  //----------------------------------------------  
  //  Reset all bit of the PORTA output (This is an optional configuration for this example)
  mcp23017.ClearReg(MCP23017_BNK0_OLATA, 0x00);  
  //----------------------------------------------
}

void loop() {
  DebouncingIntPin();    //  Debouncing interrupt pin. Active Low
  SM_DebouncingInput();  //  Debouncing State Machine
  
  //  This piece of code toggled the output bit of the portA when the state of the bit of the portB changes
  if ((InputStatus ^ OutputStatus) != 0) {
    OutputStatus = InputStatus;
    TempPortAValue = mcp23017.ReadSingleReg(MCP23017_BNK0_GPIOA, 0x00);
    mcp23017.WriteSingleReg(MCP23017_BNK0_OLATA, (TempPortAValue ^ ~InputStatus), 0x00);
  }
}

//---------------------------------------------------
//  This function prepares the data to set interrupt function
void LoadRegisterForInterrupt(void) {
  //  INTCON Register sets to  0x00
  //  DEFVAL Register sets to  0x00
  //  GPINTEN Register sets to 0xFF
  mcp23017.IoconReg.IoconByte = mcp23017.ReadSingleReg(MCP23017_BNK0_IOCONB, 0x00);
  mcp23017.IoconReg.Bit.IntPol = 0;  //  Int pin active Low
  mcp23017.IoconReg.Bit.Odr    = 1;  //  Int pin configured as open drain
  mcp23017.IoconReg.Bit.Mirror = 0;  //  Int A and Int B are associated at port A and B respectively
  
  mcp23017.GpInten.GpIntenByte = 0xFF;  //  All pin of the portB are configured for interrupt
  mcp23017.DefVal.DefValByte   = 0xFF;  //  This value, in this example, is negligible because the intcon byte is set to test the change respect the previous pin value
  mcp23017.Intcon.IntconByte   = 0x00;  //  Test change respect the previous pin value
}
//---------------------------------------------------

//---------------------------------------------------
//  Debouncing interrupt pin
void DebouncingIntPin(void) {
  IntReaded = digitalRead(MCP23017IntPin);
  if (IntReaded != IntStatus) {
    if (IntReaded != IntVar) {
       IntVar = IntReaded;
       IntLastDebouncingTime = millis();
    } else {
      if ((millis() - IntLastDebouncingTime) > IntDebouncingDelay) {
         IntVar    = IntReaded;
         IntStatus = IntReaded;         
      }
    }
  } else {
      IntVar = IntStatus;
  } 
}
//---------------------------------------------------

//---------------------------------------------------
void SM_DebouncingInput(void) {
  switch (DebouncingState)
  {
    case DEBOUNCING_IDLE:
      //  Idle state. Waits until Interrupt occurred
      if (IntStatus == 0) {
        DebouncingState++;
      }
      break;
    
    case DEBOUNCING_PROGRESS:
      //  Debouncing MCP23017 PortB input
      DebouncingInput();
      break;
      
    default:
      break;
  }  
}
//---------------------------------------------------

//---------------------------------------------------
//  Debouncing portB input
void DebouncingInput(void) {
  InputReaded = mcp23017.ReadSingleReg(MCP23017_BNK0_GPIOB, 0x00);
  if (InputReaded != InputStatus) {
    if (InputReaded != InputVar) {
       InputVar = InputReaded;
       LastDebouncingTime = millis();
    } else {
      if ((millis() - LastDebouncingTime) > DebouncingDelay) {
         InputVar    = InputReaded;
         InputStatus = InputReaded;
         DebouncingState = DEBOUNCING_IDLE;
      }
    }
  } else {
      InputVar = InputStatus;
      DebouncingState = DEBOUNCING_IDLE;
  }
}
//---------------------------------------------------
