/*************************************************************************************************************
#  HISTORY
#
#  v1.0  -  Initial Release
#
#  =============================================================================
#  This is an example and explains how to use the MGC3130 library developed by Matteo Destro for Futura Group srl
#
#  www.Futurashop.it
#  www.open-electronics.org
#
#  BSD license, all text above must be included in any redistribution
#
#  The code for this demo is splitted into four file:
#
#  - MGC3130_Demo   -> Main file project
#  - DigitalInput   -> Contains code to manage digital inputs and the respective functions to manage it
#  - Digital Output -> Contains code to manage digital output and the respective functions to manage it
#  - Timersint      -> Contains code to manage Timer1 of ATMEGA328P. Timer1 generates an interrupt every 2 mSec.
#                      With this resolution (2 mSec) it is possible to manage the timer variables used in this demo.
#                      For example the TimeOut used to filter the unwanted variations on the digital inputs (debouncing)
#
#  - Arduino Uno Rev. 3
#  - Arduino Leonardo Rev. 3
#  - ArdUnoR3_RaspPiB+_GE_100x50 Rev. 1.0 (Parametrization file Elettrodo_Ard_Rasp_100x50.enz)
#  - Gestic_Elettrodo_100x50_10           (Parametrization file Elettrodo_100x50.enz)
#  - Gestic_Elettrodo_80x80_10            (Parametrization file Elettrodo_80x80.enz)
#  - Gestic_Elettrodo_140x90_10           (Parametrization file Elettrodo_140x90.enz)
#
#  Arduino Uno Rev. 3 (PinOut Arduino to MGC3130 Electrode if is used CN1 connector:
#
#  Line Reset:  Arduino pin 8  ->  Electrode pin 6
#  Line Ts:     Arduino pin 9  ->  Electrode pin 1
#  Line SDA:    Arduino pin 27 ->  Electrode pin 4
#  Line SCL:    Arduino pin 28 ->  Electrode pin 5
#
#  +3,3Vdc  ->  Electrode pin 2
#  GND      ->  Electrode pin 3
#
#  If is used "ArdUnoR3_RaspPiB+_GE_100x50 Rev. 1.0" it is possible to connect the ArduinoUno Rev. 3
#  directly to this hardware using the connection on the bottom layer. It is totally compatible with
#  ArduinoUno board R3 and Arduino Leonardo R3
#
#  Note: For this demo is necessary the FT1079K shield developed by Futura Electronica to report the
#        gesture on the digital output (RelÃ¨). In this demo we have used two FT1079K shield
#
#  The gestures reported on the digital output are:
#
#  - Tap South         -> First  FT1079 shield. For digital output it is used the PORTA of MCP23017. Hardware address 0x00, point 0
#  - Tap West          -> First  FT1079 shield. For digital output it is used the PORTA of MCP23017. Hardware address 0x00, point 1
#  - Tap North         -> First  FT1079 shield. For digital output it is used the PORTA of MCP23017. Hardware address 0x00, point 2
#  - Tap East          -> First  FT1079 shield. For digital output it is used the PORTA of MCP23017. Hardware address 0x00, point 3
#  - Tap Centre        -> First  FT1079 shield. For digital output it is used the PORTA of MCP23017. Hardware address 0x00, point 4
#  - Double Tap South  -> First  FT1079 shield. For digital output it is used the PORTA of MCP23017. Hardware address 0x00, point 5
#  - Double Tap West   -> First  FT1079 shield. For digital output it is used the PORTA of MCP23017. Hardware address 0x00, point 6
#  - Double Tap North  -> First  FT1079 shield. For digital output it is used the PORTA of MCP23017. Hardware address 0x00, point 7
#  - Double Tap East   -> Second FT1079 shield. For digital output it is used the PORTA of MCP23017. Hardware address 0x01, point 0
#  - Double Tap Centre -> Second FT1079 shield. For digital output it is used the PORTA of MCP23017. Hardware address 0x01, point 1
#  - West To East      -> Second FT1079 shield. For digital output it is used the PORTA of MCP23017. Hardware address 0x01, point 2
#  - East to West      -> Second FT1079 shield. For digital output it is used the PORTA of MCP23017. Hardware address 0x01, point 3
#  - South to North    -> Second FT1079 shield. For digital output it is used the PORTA of MCP23017. Hardware address 0x01, point 4
#  - North to South    -> Second FT1079 shield. For digital output it is used the PORTA of MCP23017. Hardware address 0x01, point 5
#  - ClockWise         -> Second FT1079 shield. For digital output it is used the PORTA of MCP23017. Hardware address 0x01, point 6
#  - CounterClockWise  -> Second FT1079 shield. For digital output it is used the PORTA of MCP23017. Hardware address 0x01, point 7
#
#  =============================================================================
#
#  INSTALLATION
#  The 3 library files (MGC3130.cpp, MGC3130.h and keywords.txt) in the MGC3130 folder should be placed in your Arduino Library folder.
#  Run the MGC3130_Demo.ino file from your Arduino IDE.
#
#  =============================================================================
#
#  SUPPORT
#
#  info@open-electronics.org
#
*************************************************************************************************************/

#include <Wire.h>
#include "MGC3130.h"
#include "MCP23017.h"
#include <EEPROM.h>

//======================================================================
MGC3130  mgc3130  = MGC3130();
MCP23017 mcp23017 = MCP23017();
//======================================================================

//======================================================================
//  State Machine defines
typedef void State;
typedef State (*Pstate)();

Pstate Input_Management;                  //  States machine used to manage the digital inputs (P1, P2, P3 and J1)
Pstate Led_Management;                    //  States machine used to manage led LD7. This led is used to recognize the pressure on P1, P2 and P3 buttons
Pstate Led_North;                         //  States machine used to manage led LD7. Used to recognize the pressure on the North electrode and manage the digital outputs on the I/O expanders (MCP23017)
Pstate Led_South;                         //  States machine used to manage led LD7. Used to recognize the pressure on the South electrode and manage the digital outputs on the I/O expanders (MCP23017)
Pstate Led_West;                          //  States machine used to manage led LD7. Used to recognize the pressure on the West electrode and manage the digital outputs on the I/O expanders (MCP23017)
Pstate Led_East;                          //  States machine used to manage led LD7. Used to recognize the pressure on the East electrode and manage the digital outputs on the I/O expanders (MCP23017)
Pstate Led_Centre;                        //  States machine used to manage led LD7. Used to recognize the pressure on the Centre electrode and manage the digital outputs on the I/O expanders (MCP23017)
Pstate Led_WestEast;                      //  States machine used to manage led LD7. Used to recognize the gesture "West to East" and manage the digital outputs on the I/O expander (MCP23017)
Pstate Led_EastWest;                      //  States machine used to manage led LD7. Used to recognize the gesture "East to West" and manage the digital outputs on the I/O expander (MCP23017)
Pstate Led_NorthSouth;                    //  States machine used to manage led LD7. Used to recognize the gesture "North to South" and manage the digital outputs on the I/O expander (MCP23017)
Pstate Led_SouthNorth;                    //  States machine used to manage led LD7. Used to recognize the gesture "South to North" and manage the digital outputs on the I/O expander (MCP23017)
Pstate Led_ClockWise;                     //  States machine used to manage led LD7. Used to recognize the gesture "ClockWise" and manage the digital outputs on the I/O expander (MCP23017)
Pstate Led_CounterClockWise;              //  States machine used to manage led LD7. Used to recognize the gesture "CounterClockWise" and manage the digital outputs on the I/O expander (MCP23017)
Pstate MonoStableTapNorth;	          //  States machine used to manage monostable command on the digital output when the user executes a "Tap gesture" on the North electrode
Pstate MonoStableTapSouth;                //  States machine used to manage monostable command on the digital output when the user executes a "Tap gesture" on the South electrode	
Pstate MonoStableTapWest;	          //  States machine used to manage monostable command on the digital output when the user executes a "Tap gesture" on the West electrode	
Pstate MonoStableTapEast;	          //  States machine used to manage monostable command on the digital output when the user executes a "Tap gesture" on the East electrode	
Pstate MonoStableTapCentre;               //  States machine used to manage monostable command on the digital output when the user executes a "Tap gesture" on the Centre electrode		
Pstate MonoStableDoubleTapNorth;          //  States machine used to manage monostable command on the digital output when the user executes a "Double Tap gesture" on the North electrode	
Pstate MonoStableDoubleTapSouth;          //  States machine used to manage monostable command on the digital output when the user executes a "Double Tap gesture" on the South electrode	
Pstate MonoStableDoubleTapWest;	          //  States machine used to manage monostable command on the digital output when the user executes a "Double Tap gesture" on the West electrode	
Pstate MonoStableDoubleTapEast;	          //  States machine used to manage monostable command on the digital output when the user executes a "Double Tap gesture" on the East electrode	
Pstate MonoStableDoubleTapCentre;         //  States machine used to manage monostable command on the digital output when the user executes a "Double Tap gesture" on the Centre electrode		
Pstate MonoStableGestureWestEast;         //  States machine used to manage monostable command on the digital output when the user executes a "West to East gesture"
Pstate MonoStableGestureEastWest;         //  States machine used to manage monostable command on the digital output when the user executes a "East to West gesture"
Pstate MonoStableGestureSouthNorth;       //  States machine used to manage monostable command on the digital output when the user executes a "North to South gesture"
Pstate MonoStableGestureNorthSouth;       //  States machine used to manage monostable command on the digital output when the user executes a "South to North gesture"
Pstate MonoStableGestureClockWise;        //  States machine used to manage monostable command on the digital output when the user executes a "ClockWise"
Pstate MonoStableGestureCounterClockWise; //  States machine used to manage monostable command on the digital output when the user executes a "CounterClockWise"
//======================================================================

//======================================================================
//  Define timer Constants and Variables
#define	TIMEBASE      2                //  Time Base: 2mSec

#define T_10MSEC      (10/TIMEBASE)    //  Defines a constant for a timeout of 10 mSec
#define T_20MSEC      (20/TIMEBASE)    //  Defines a constant for a timeout of 20 mSec
#define T_50MSEC      (50/TIMEBASE)    //  Defines a constant for a timeout of 50 mSec
#define T_100MSEC     (100/TIMEBASE)   //  Defines a constant for a timeout of 100 mSec
#define T_150MSEC     (150/TIMEBASE)   //  Defines a constant for a timeout of 150 mSec
#define T_200MSEC     (200/TIMEBASE)   //  Defines a constant for a timeout of 200 mSec
#define T_250MSEC     (250/TIMEBASE)   //  Defines a constant for a timeout of 250 mSec
#define T_500MSEC     (500/TIMEBASE)   //  Defines a constant for a timeout of 500 mSec
#define T_750MSEC     (750/TIMEBASE)   //  Defines a constant for a timeout of 750 mSec

#define T_1SEC	      (1000/TIMEBASE)  //  Defines a constant for a timeout of 1,000  Sec
#define T_1SEC250     (1250/TIMEBASE)  //  Defines a constant for a timeout of 1,250  Sec
#define T_1SEC500     (1500/TIMEBASE)  //  Defines a constant for a timeout of 1,500  Sec
#define T_1SEC750     (1750/TIMEBASE)  //  Defines a constant for a timeout of 1,750  Sec
#define T_2SEC	      (2000/TIMEBASE)  //  Defines a constant for a timeout of 2,000  Sec
#define T_2SEC5	      (2500/TIMEBASE)  //  Defines a constant for a timeout of 2,500  Sec
#define T_3SEC	      (3000/TIMEBASE)  //  Defines a constant for a timeout of 3,000  Sec
#define T_4SEC	      (4000/TIMEBASE)  //  Defines a constant for a timeout of 4,000  Sec
#define T_5SEC	      (5000/TIMEBASE)  //  Defines a constant for a timeout of 5,000  Sec
#define T_10SEC	      (10000/TIMEBASE) //  Defines a constant for a timeout of 10,000 Sec
#define T_15SEC	      (15000/TIMEBASE) //  Defines a constant for a timeout of 15,000 Sec
#define T_20SEC	      (20000/TIMEBASE) //  Defines a constant for a timeout of 20,000 Sec
#define T_25SEC	      (25000/TIMEBASE) //  Defines a constant for a timeout of 25,000 Sec
#define T_30SEC	      (30000/TIMEBASE) //  Defines a constant for a timeout of 30,000 Sec
#define T_60SEC	      (60000/TIMEBASE) //  Defines a constant for a timeout of 60,000 Sec

uint8_t  DebouncingTimeOut;    //  Debouncing TimeOut for digital inputs
uint16_t TimeOutWait;          //  TimeOut to manage pulse pressing
uint16_t TimeOutLed;           //  TimeOut to manage led pulse

union MonoStableTime {
  uint16_t	ui;
  struct {
    uint8_t  ucl;
    uint8_t  uch;
  } Int;
} MonoStableTime;

uint8_t  TimeOutNorthGest;                 //  Timeout used to read the complete sequence of gestures on the North Electrode. Usefull to decode between "Touch gesture", "Tap Gesture" or "Double Tap gesture"
uint8_t  TimeOutSouthGest;                 //  Timeout used to read the complete sequence of gestures on the South Electrode. Usefull to decode between "Touch gesture", "Tap Gesture" or "Double Tap gesture"
uint8_t  TimeOutWestGest;                  //  Timeout used to read the complete sequence of gestures on the West Electrode. Usefull to decode between "Touch gesture", "Tap Gesture" or "Double Tap gesture"
uint8_t  TimeOutEastGest;                  //  Timeout used to read the complete sequence of gestures on the East Electrode. Usefull to decode between "Touch gesture", "Tap Gesture" or "Double Tap gesture"
uint8_t  TimeOutCentreGest;                //  Timeout used to read the complete sequence of gestures on the Centre Electrode. Usefull to decode between "Touch gesture", "Tap Gesture" or "Double Tap gesture"

uint8_t  TimeOutTapBistableNorth;          //  TimeOut for bistable command on the digital outputs. Associated at North Electrode. Tap Gesture
uint8_t  TimeOutTapBistableSouth;          //  TimeOut for bistable command on the digital outputs. Associated at South Electrode. Tap Gesture
uint8_t  TimeOutTapBistableWest;           //  TimeOut for bistable command on the digital outputs. Associated at West Electrode. Tap Gesture
uint8_t  TimeOutTapBistableEast;           //  TimeOut for bistable command on the digital outputs. Associated at East Electrode. Tap Gesture
uint8_t  TimeOutTapBistableCentre;         //  TimeOut for bistable command on the digital outputs. Associated at Centre Electrode. Tap Gesture

uint16_t TimeOutTapMonostableNorth;        //  TimeOut for monostable command on the digital outputs. Associated at North Electrode. Tap Gesture
uint16_t TimeOutTapMonostableSouth;        //  TimeOut for monostable command on the digital outputs. Associated at South Electrode. Tap Gesture
uint16_t TimeOutTapMonostableWest;         //  TimeOut for monostable command on the digital outputs. Associated at West Electrode. Tap Gesture
uint16_t TimeOutTapMonostableEast;         //  TimeOut for monostable command on the digital outputs. Associated at East Electrode. Tap Gesture
uint16_t TimeOutTapMonostableCentre;       //  TimeOut for monostable command on the digital outputs. Associated at Centre Electrode. Tap Gesture

uint8_t  TimeOutDoubleTapBistableNorth;    //  TimeOut for bistable command on the digital outputs. Associated at North Electrode. Double Tap Gesture
uint8_t  TimeOutDoubleTapBistableSouth;    //  TimeOut for bistable command on the digital outputs. Associated at South Electrode. Double Tap Gesture
uint8_t  TimeOutDoubleTapBistableWest;     //  TimeOut for bistable command on the digital outputs. Associated at West Electrode. Double Tap Gesture
uint8_t  TimeOutDoubleTapBistableEast;     //  TimeOut for bistable command on the digital outputs. Associated at East Electrode. Double Tap Gesture
uint8_t  TimeOutDoubleTapBistableCentre;   //  TimeOut for bistable command on the digital outputs. Associated at Centre Electrode. Double Tap Gesture

uint16_t TimeOutDoubleTapMonostableNorth;  //  TimeOut for monostable command on the digital outputs. Associated at North Electrode. Double Tap Gesture
uint16_t TimeOutDoubleTapMonostableSouth;  //  TimeOut for monostable command on the digital outputs. Associated at South Electrode. Double Tap Gesture
uint16_t TimeOutDoubleTapMonostableWest;   //  TimeOut for monostable command on the digital outputs. Associated at West Electrode. Double Tap Gesture
uint16_t TimeOutDoubleTapMonostableEast;   //  TimeOut for monostable command on the digital outputs. Associated at East Electrode. Double Tap Gesture
uint16_t TimeOutDoubleTapMonostableCentre; //  TimeOut for monostable command on the digital outputs. Associated at Centre Electrode. Double Tap Gesture

uint8_t  TimeOutWestEastBistable;          //  TimeOut for bistable command on the digital outputs. Associated at West to East Gesture
uint8_t  TimeOutEastWestBistable;          //  TimeOut for bistable command on the digital outputs. Associated at East to West Gesture
uint8_t  TimeOutSouthNorthBistable;        //  TimeOut for bistable command on the digital outputs. Associated at North to South Gesture
uint8_t  TimeOutNorthSouthBistable;        //  TimeOut for bistable command on the digital outputs. Associated at South to North Gesture

uint16_t TimeOutWestEastMonostable;        //  TimeOut for monostable command on the digital outputs. Associated at West to East Gesture
uint16_t TimeOutEastWestMonostable;        //  TimeOut for monostable command on the digital outputs. Associated at East to West Gesture
uint16_t TimeOutSouthNorthMonostable;      //  TimeOut for monostable command on the digital outputs. Associated at North to South Gesture
uint16_t TimeOutNorthSouthMonostable;      //  TimeOut for monostable command on the digital outputs. Associated at South to North Gesture

uint8_t  TimeOutClockWiseBistable;         //  TimeOut for bistable command on the digital outputs. ClockWise Gesture
uint8_t  TimeOutCounterClockWiseBistable;  //  TimeOut for bistable command on the digital outputs. CounterClockWise Gesture

uint16_t TimeOutClockWiseMonostable;       //  TimeOut for monostable command on the digital outputs. Associated at ClockWise Gesture
uint16_t TimeOutCounterClockWiseMonostable;//  TimeOut for monostable command on the digital outputs. Associated at CounterClockWise Gesture
//======================================================================

//======================================================================
//  I/O Input pin define
uint8_t P3_Button = 6;         //  Digital input. Button P3. See schematic
uint8_t P2_Button = 5;         //  Digital input. Button P2. See schematic
uint8_t P1_Button = 4;         //  Digital input. Button P1. See schematic
uint8_t Jumper1   = 3;         //  Digital input. Jumper J1. See schematic

uint8_t Led       = 7;         //  Digital Output. Led LD7

uint8_t Reset_MGC3130 = 8;     //  MGC3130 Reset line
uint8_t Ts_MGC3130    = 9;     //  MGC3130 TS line
uint8_t Addr_MGC3130  = 0x42;  //  MGC3130 hardware address
//======================================================================

//======================================================================
//  Some variables and constants used to recognize the touch gestures readed by MGC3130 device
//	Sequence for Tap gesture
//	Touch -> Tap
//
//	Sequence for Double Tap gesture
//	Touch -> Tap -> Touch -> DoubleTap -> Touch -> Tap
//
uint32_t Gesture_Readed;      //  Used to store the recognized gesture
uint8_t  NorthTouch[8];       // \
uint8_t  NorthCounter;        //  >--- Set of variables used to store gestures sequence. Usefull to recognize between Touch, Tap and Double Tapgesture. North electrode
uint8_t  NorthData;           // /
uint8_t  SouthTouch[8];       // \
uint8_t  SouthCounter;        //  >--- Set of variables used to store gestures sequence. Usefull to recognize between Touch, Tap and Double Tap gesture. South electrode
uint8_t  SouthData;           // /
uint8_t  WestTouch[8];        // \
uint8_t  WestCounter;         //  >--- Set of variables used to store gestures sequence. Usefull to recognize between Touch, Tap and Double Tap gesture. West electrode
uint8_t  WestData;            // /
uint8_t  EastTouch[8];        // \
uint8_t  EastCounter;         //  >--- Set of variables used to store gestures sequence. Usefull to recognize between Touch, Tap and Double Tap gesture. East electrode
uint8_t  EastData;            // /
uint8_t  CentreTouch[8];      // \
uint8_t  CentreCounter;       //  >--- Set of variables used to store gestures sequence. Usefull to recognize between Touch, Tap and Double Tap gesture. Centre electrode
uint8_t  CentreData;          // /

#define TOUCH_CODE          1    //  Code, for touch gesture, used to fill the array gesture storage. See variables above
#define TAP_CODE            2    //  Code, for tap gesture, used to fill the array gesture storage. See variables above
#define DOUBLE_TAP_CODE     3    //  Code, for double tap gesture, used to fill the array gesture storage. See variables above

//============================================
//  Use these constants if no filter is set on the gestures that has been read from MGC3130
#define GESTURE_DOUBLE_TAP_NORTH  10   // \  
#define GESTURE_DOUBLE_TAP_SOUTH  10   //  |
#define GESTURE_DOUBLE_TAP_WEST   10   //   >--- The "double tap gesture" is composed by a sequence of gestures. So the sum of gesture codes that have been read returns the value indicated
#define GESTURE_DOUBLE_TAP_EAST   10   //  |
#define GESTURE_DOUBLE_TAP_CENTRE 10   // /
#define GESTURE_TAP_NORTH         3    // \ 
#define GESTURE_TAP_SOUTH         3    //  |
#define GESTURE_TAP_WEST          3    //   >--- The "tap gesture" is composed by a sequence of gestures. So the sum of gesture codes that have been read returns the value indicated
#define GESTURE_TAP_EAST          3    //  |
#define GESTURE_TAP_CENTRE        3    // /
#define GESTURE_TOUCH_NORTH       1    // \  
#define GESTURE_TOUCH_SOUTH       1    //  |
#define GESTURE_TOUCH_WEST        1    //   >--- The "Touch Gesture"
#define GESTURE_TOUCH_EAST        1    //  |
#define GESTURE_TOUCH_CENTRE      1    // /
//============================================

//============================================
//  Use these constants if Touch filter is set on the gestures that has been read from MGC3130
//#define GESTURE_DOUBLE_TAP_NORTH  7   // \ 
//#define GESTURE_DOUBLE_TAP_SOUTH  7   //  |
//#define GESTURE_DOUBLE_TAP_WEST   7   //   >--- The "double tap gesture" is composed by a sequence of gestures. So the sum of gesture codes that have been read returns the value indicated
//#define GESTURE_DOUBLE_TAP_EAST   7   //  |
//#define GESTURE_DOUBLE_TAP_CENTRE 7   // /
//#define GESTURE_TAP_NORTH         2   // \ 
//#define GESTURE_TAP_SOUTH         2   //  |
//#define GESTURE_TAP_WEST          2   //   >--- The "tap gesture" is composed by a sequence of gestures. So the sum of gesture codes that have been read returns the value indicated
//#define GESTURE_TAP_EAST          2   //  |
//#define GESTURE_TAP_CENTRE        2   // /
//#define GESTURE_TOUCH_NORTH       1   // \ 
//#define GESTURE_TOUCH_SOUTH       1   //  |
//#define GESTURE_TOUCH_WEST        1   //   >--- The "Touch Gesture" is filtered
//#define GESTURE_TOUCH_EAST        1   //  |
//#define GESTURE_TOUCH_CENTRE      1   // /
//============================================

//============================================
//  Use these constants if Tap filter is set on the gestures that has been read from MGC3130
//#define GESTURE_DOUBLE_TAP_NORTH   5   // \
//#define GESTURE_DOUBLE_TAP_SOUTH   5   //  |
//#define GESTURE_DOUBLE_TAP_WEST    5   //   >--- The "double tap gesture" is composed by a sequence of gestures. So the sum of gesture codes that have been read returns the value indicated
//#define GESTURE_DOUBLE_TAP_EAST    5   //  |
//#define GESTURE_DOUBLE_TAP_CENTRE  5   // /
//#define GESTURE_TAP_NORTH          3   // \ 
//#define GESTURE_TAP_SOUTH          3   //  |
//#define GESTURE_TAP_WEST           3   //   >--- The "Touch Gesture" is filtered
//#define GESTURE_TAP_EAST           3   //  |
//#define GESTURE_TAP_CENTRE         3   // /
//#define GESTURE_TOUCH_NORTH        1   // \ 
//#define GESTURE_TOUCH_SOUTH        1   //  |
//#define GESTURE_TOUCH_WEST         1   //   >--- The "Touch Gesture"
//#define GESTURE_TOUCH_EAST         1   //  |
//#define GESTURE_TOUCH_CENTRE       1   // /
//============================================
//======================================================================

//======================================================================
//  Variables to manage the input pin (Debouncing)
union  DigInputStatus {
  uint8_t  Input;
  struct {
    uint8_t  P3_Button  : 1; //  Bit 0
    uint8_t  P2_Button  : 1; //  Bit 1
    uint8_t  P1_Button  : 1; //  Bit 2
    uint8_t  Jumper1    : 1; //  Bit 3
    uint8_t  Free       : 4;
  } In;
} DigInputStatus;

union  DigInputReaded {
  uint8_t  Input;
  struct {
    uint8_t  P3_Button  : 1; //  Bit 0
    uint8_t  P2_Button  : 1; //  Bit 1
    uint8_t  P1_Button  : 1; //  Bit 2
    uint8_t  Jumper1    : 1; //  Bit 3
    uint8_t  Free       : 4;
  } In;
} DigInputReaded;

uint8_t DigInputVar;
//======================================================================

//======================================================================
void setup() {
  mgc3130.ResetDevice(Reset_MGC3130);            //  hold MGC3130 in reset mode
  
  MonoStableTime.Int.uch = EEPROM.read(0x0000);  //  Reads Monostable TimeOut by EEPROM. Two bytes
  MonoStableTime.Int.ucl = EEPROM.read(0x0001);
     
  mcp23017.Begin();                              //  \
  mcp23017.SetAllRegBank0(0x00);                 //   |
  mcp23017.ClearReg(MCP23017_BNK0_OLATA, 0x00);  //    >--- Initializes I/O expander board
  mcp23017.SetAllRegBank0(0x01);                 //   |
  mcp23017.ClearReg(MCP23017_BNK0_OLATA, 0x01);  //  /

  mgc3130.SetSerial(19200, SERIAL_8N1);          //  Initializes serial interface  
  mgc3130.SetAdd(Addr_MGC3130);                  //  Set MGC3130 address
  mgc3130.Begin(Ts_MGC3130, Reset_MGC3130);      //  Initializes TS and Reset lines

  SetInputPin();      //  Initialized Input pins
  SetOutputPin();     //  Initializes Output pins 
  SetupTimer();       //  Initializes Interrupt Timer 1 to manage TimeOut variables

  Input_Management                  = Input_Idle;                              //  Initializes states machine used to manage the digital inputs (P1, P2, P3 and J1)
  Led_Management                    = Led_Idle;                                //  Initializes states machine used to manage led LD7. This led is used to recognize the pressure on P1, P2 and P3 buttons
  Led_North                         = LedNorth_Idle;                           //  Initializes states machine used to manage led LD7. Used to recognize the pressure on the North electrode and manage the digital outputs on the I/O expanders (MCP23017)
  Led_South                         = LedSouth_Idle;                           //  Initializes states machine used to manage led LD7. Used to recognize the pressure on the South electrode and manage the digital outputs on the I/O expanders (MCP23017)
  Led_West                          = LedWest_Idle;                            //  Initializes states machine used to manage led LD7. Used to recognize the pressure on the West electrode and manage the digital outputs on the I/O expanders (MCP23017)
  Led_East                          = LedEast_Idle;                            //  Initializes states machine used to manage led LD7. Used to recognize the pressure on the East electrode and manage the digital outputs on the I/O expanders (MCP23017)
  Led_Centre                        = LedCentre_Idle;                          //  Initializes states machine used to manage led LD7. Used to recognize the pressure on the Centre electrode and manage the digital outputs on the I/O expanders (MCP23017)
  Led_WestEast                      = LedWestEast_Idle;                        //  Initializes states machine used to manage led LD7. Used to recognize the gesture "West to East" and manage the digital outputs on the I/O expander (MCP23017)
  Led_EastWest                      = LedEastWest_Idle;                        //  Initializes states machine used to manage led LD7. Used to recognize the gesture "East to West" and manage the digital outputs on the I/O expander (MCP23017)
  Led_NorthSouth                    = LedNorthSouth_Idle;                      //  Initializes states machine used to manage led LD7. Used to recognize the gesture "North to South" and manage the digital outputs on the I/O expander (MCP23017)
  Led_SouthNorth                    = LedSouthNorth_Idle;                      //  Initializes states machine used to manage led LD7. Used to recognize the gesture "South to North" and manage the digital outputs on the I/O expander (MCP23017)
  Led_ClockWise                     = LedClockWise_Idle;                       //  Initializes states machine used to manage led LD7. Used to recognize the gesture "ClockWise" and manage the digital outputs on the I/O expander (MCP23017)
  Led_CounterClockWise              = LedCounterClockWise_Idle;                //  Initializes states machine used to manage led LD7. Used to recognize the gesture "CounterClockWise" and manage the digital outputs on the I/O expander (MCP23017)
  MonoStableTapNorth                = MonoStableTapNorth_Idle;                 //  Initializes states machine used to manage monostable command on the digital output when the user executes a "Tap gesture" on the North electrode
  MonoStableTapSouth                = MonoStableTapSouth_Idle;                 //  Initializes states machine used to manage monostable command on the digital output when the user executes a "Tap gesture" on the South electrode	
  MonoStableTapWest                 = MonoStableTapWest_Idle;                  //  Initializes states machine used to manage monostable command on the digital output when the user executes a "Tap gesture" on the West electrode	
  MonoStableTapEast                 = MonoStableTapEast_Idle;                  //  Initializes states machine used to manage monostable command on the digital output when the user executes a "Tap gesture" on the East electrode	
  MonoStableTapCentre               = MonoStableTapCentre_Idle;                //  Initializes states machine used to manage monostable command on the digital output when the user executes a "Tap gesture" on the Centre electrode		
  MonoStableDoubleTapNorth          = MonoStableDoubleTapNorth_Idle;	       //  Initializes states machine used to manage monostable command on the digital output when the user executes a "Double Tap gesture" on the North electrode	
  MonoStableDoubleTapSouth          = MonoStableDoubleTapSouth_Idle;           //  Initializes states machine used to manage monostable command on the digital output when the user executes a "Double Tap gesture" on the South electrode	
  MonoStableDoubleTapWest           = MonoStableDoubleTapWest_Idle;            //  Initializes states machine used to manage monostable command on the digital output when the user executes a "Double Tap gesture" on the West electrode	
  MonoStableDoubleTapEast           = MonoStableDoubleTapEast_Idle;            //  Initializes states machine used to manage monostable command on the digital output when the user executes a "Double Tap gesture" on the East electrode	
  MonoStableDoubleTapCentre         = MonoStableDoubleTapCentre_Idle;          //  Initializes states machine used to manage monostable command on the digital output when the user executes a "Double Tap gesture" on the Centre electrode		
  MonoStableGestureWestEast         = MonoStableGestureWestEast_Idle;          //  Initializes states machine used to manage monostable command on the digital output when the user executes a "West to East gesture"
  MonoStableGestureEastWest         = MonoStableGestureEastWest_Idle;          //  Initializes states machine used to manage monostable command on the digital output when the user executes a "East to West gesture"
  MonoStableGestureSouthNorth       = MonoStableGestureSouthNorth_Idle;        //  Initializes states machine used to manage monostable command on the digital output when the user executes a "North to South gesture"
  MonoStableGestureNorthSouth       = MonoStableGestureNorthSouth_Idle;        //  Initializes states machine used to manage monostable command on the digital output when the user executes a "South to North gesture" 
  MonoStableGestureClockWise        = MonoStableGestureClockWise_Idle;         //  Initializes states machine used to manage monostable command on the digital output when the user executes a "ClockWise gesture" 
  MonoStableGestureCounterClockWise = MonoStableGestureCounterClockWise_Idle;  //  Initializes states machine used to manage monostable command on the digital output when the user executes a "CounterClockWise gesture"
}
//======================================================================

//======================================================================
void loop() {
  DebouncingInput();                   //  Debouncing digital inputs (P1, P2, P3 e J1)
  Input_Management();                  //  State Machine to manage Digital input
  Led_Management();                    //  State Machine to manage Led
  Led_North();                         //  State Machine to manage Led and output about gestic recognized
  Led_South();                         //  State Machine to manage Led and output about gestic recognized
  Led_West();                          //  State Machine to manage Led and output about gestic recognized
  Led_East();                          //  State Machine to manage Led and output about gestic recognized
  Led_Centre();                        //  State Machine to manage Led and output about gestic recognized
  Led_WestEast();                      //  State Machine to manage Led and output about gestic recognized
  Led_EastWest();                      //  State Machine to manage Led and output about gestic recognized
  Led_NorthSouth();                    //  State Machine to manage Led and output about gestic recognized
  Led_SouthNorth();                    //  State Machine to manage Led and output about gestic recognized
  Led_ClockWise();                     //  State Machine to manage Led and output about gestic recognized
  Led_CounterClockWise();              //  State Machine to manage Led and output about gestic recognized
  MonoStableTapNorth();                //  State Machine to manage Led and output about gestic recognized            
  MonoStableTapSouth();	               //  State Machine to manage Led and output about gestic recognized
  MonoStableTapWest();                 //  State Machine to manage Led and output about gestic recognized
  MonoStableTapEast();                 //  State Machine to manage Led and output about gestic recognized
  MonoStableTapCentre();               //  State Machine to manage Led and output about gestic recognized
  MonoStableDoubleTapNorth();          //  State Machine to manage Led and output about gestic recognized
  MonoStableDoubleTapSouth();          //  State Machine to manage Led and output about gestic recognized
  MonoStableDoubleTapWest();           //  State Machine to manage Led and output about gestic recognized
  MonoStableDoubleTapEast();           //  State Machine to manage Led and output about gestic recognized
  MonoStableDoubleTapCentre();         //  State Machine to manage Led and output about gestic recognized
  MonoStableGestureWestEast();         //  State Machine to manage Led and output about gestic recognized
  MonoStableGestureEastWest();         //  State Machine to manage Led and output about gestic recognized
  MonoStableGestureSouthNorth();       //  State Machine to manage Led and output about gestic recognized
  MonoStableGestureNorthSouth();       //  State Machine to manage Led and output about gestic recognized
  MonoStableGestureClockWise();        //  State Machine to manage Led and output about gestic recognized
  MonoStableGestureCounterClockWise(); //  State Machine to manage Led and output about gestic recognized

  if (mgc3130.GetTsLineStatus(Ts_MGC3130) == 0) {
    mgc3130.GetEvent();                  //  Start read data from MGC3130
    mgc3130.DecodeGesture();             //  Decode Gesture
    mgc3130.ReleaseTsLine(Ts_MGC3130);   //  Release TS Line
  }
}
//======================================================================

