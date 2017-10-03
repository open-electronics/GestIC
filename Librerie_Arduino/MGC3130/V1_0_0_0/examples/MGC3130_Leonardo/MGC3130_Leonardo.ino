/*************************************************************************************************************
#  HISTORY
#
#  v1.0  -  Initial Release
#
#  =============================================================================
#  This is an example and explains how to use the MGC3130 library developed by Matteo Destro for Futura Group srl
#  To use this library it is necessary the Arduino Leonardo board and a software on the PC to interact
#  with this code. The software is a image viewer and it is downloadable at this link "http://www.faststone.org/FSViewerDownload.htm"
#
#  The Image Viewer must be configured to start with a shortcuts. In this example the shortcuts is "ctrl + alt + F1"
#
#  The code for this demo is splitted into four file:
#
#  - MGC3130_Leonardo -> Main file project
#  - DigitalInput     -> Contains code to manage digital inputs and the respective functions to manage it
#  - Gesture          -> Contains code to manage recognized gesture
#  - Timersint        -> Contains code to manage Timer1 of ATMEGA328P. Timer1 generates an interrupt every 2 mSec.
#                        With this resolution (2 mSec) it is possible to manage the timer variables used in this demo.
#                        For example the TimeOut used to filter the unwanted variations on the digital inputs (debouncing)
#
#  Arduino Uno Rev. 3 (PinOut Arduino to MGC3130 Electrode)
#
#  Line Reset:  Arduino pin 8  ->  Electrode pin 6
#  Line Ts:     Arduino pin 9  ->  Electrode pin 1
#  Line SDA:    Arduino pin 27 ->  Electrode pin 4
#  Line SCL:    Arduino pin 28 ->  Electrode pin 5
#  
#  +3,3Vdc  ->  Electrode pin 2
#  GND      ->  Electrode pin 3
#   
#  =============================================================================
#
#  =============================================================================
#  Key		         Hex value   Dec value
#  =============================================================================
#  KEY_LEFT_CTRL       |   0x80    |   128    | 
#  KEY_LEFT_SHIFT      |   0x81    |   129    |
#  KEY_LEFT_ALT        |   0x82    |   130    |
#  KEY_LEFT_GUI        |   0x83    |   131    |
#  KEY_RIGHT_CTRL      |   0x84    |   132    |
#  KEY_RIGHT_SHIFT     |   0x85    |   133    |
#  KEY_RIGHT_ALT       |   0x86    |   134    |
#  KEY_RIGHT_GUI       |   0x87    |   135    |
#  KEY_UP_ARROW        |   0xDA    |   218    |
#  KEY_DOWN_ARROW      |   0xD9    |   217    |
#  KEY_LEFT_ARROW      |   0xD8    |   216    |
#  KEY_RIGHT_ARROW     |   0xD7    |   215    |
#  KEY_BACKSPACE       |   0xB2    |   178    |
#  KEY_TAB 	       |   0xB3    |   179    |
#  KEY_RETURN 	       |   0xB0    |   176    |
#  KEY_ESC   	       |   0xB1    |   177    |
#  KEY_INSERT 	       |   0xD1    |   209    |
#  KEY_DELETE 	       |   0xD4    |   212    |
#  KEY_PAGE_UP 	       |   0xD3    |   211    |
#  KEY_PAGE_DOWN       |   0xD6    |   214    |
#  KEY_HOME 	       |   0xD2    |   210    |
#  KEY_END 	       |   0xD5    |   213    |
#  KEY_CAPS_LOCK       |   0xC1    |   193    |
#  KEY_F1 	       |   0xC2    |   194    |
#  KEY_F2 	       |   0xC3    |   195    |
#  KEY_F3 	       |   0xC4    |   196    |
#  KEY_F4 	       |   0xC5    |   197    |
#  KEY_F5 	       |   0xC6    |   198    |
#  KEY_F6 	       |   0xC7    |   199    |
#  KEY_F7 	       |   0xC8    |   200    |
#  KEY_F8 	       |   0xC9    |   201    |
#  KEY_F9 	       |   0xCA    |   202    |
#  KEY_F10 	       |   0xCB    |   203    |
#  KEY_F11 	       |   0xCC    |   204    |
#  KEY_F12 	       |   0xCD    |   205    |
#  =============================================================================
#
#  INSTALLATION
#
#  The 3 library files (MGC3130.cpp, MGC3130.h and keywords.txt) in the MGC3130 folder should be placed in your Arduino Library folder.
#  Run the MGC3130_Demo.ino file from your Arduino IDE.
#
#  =============================================================================
#
#  SUPPORT
#
#  info@open-electronics.org 
#
*********************************************************************************************************/

#include <Wire.h>
#include "MGC3130.h"
#include "MCP23017.h"

MGC3130  mgc3130  = MGC3130();

//======================================================================
//  State Machine defines
typedef void State;
typedef State (*Pstate)();

Pstate Input_Management;    //  States machine used to manage the digital inputs (P1, P2, P3 and J1)
Pstate Led_Management;      //  States machine used to manage led LD7. This led is used to recognize the pressure on P1, P2 and P3 buttons
Pstate NorthElectrode;      //  States machine used to manage led LD7. Used to recognize the pressure on the North electrode and manage the digital outputs on the I/O expanders (MCP23017)
Pstate SouthElectrode;      //  States machine used to manage led LD7. Used to recognize the pressure on the South electrode and manage the digital outputs on the I/O expanders (MCP23017)
Pstate WestElectrode;       //  States machine used to manage led LD7. Used to recognize the pressure on the West electrode and manage the digital outputs on the I/O expanders (MCP23017)
Pstate EastElectrode;       //  States machine used to manage led LD7. Used to recognize the pressure on the East electrode and manage the digital outputs on the I/O expanders (MCP23017)
Pstate CentreElectrode;     //  States machine used to manage led LD7. Used to recognize the pressure on the Centre electrode and manage the digital outputs on the I/O expanders (MCP23017)
//======================================================================

//======================================================================
//  Define timer Constants and Variables
#define	TIMEBASE      2                //  Time Base: 2mSec

#define T_50MSEC      (50/TIMEBASE)    //  Defines a constant for a timeout of 50 mSec
#define T_200MSEC     (200/TIMEBASE)   //  Defines a constant for a timeout of 200 mSec
#define T_250MSEC     (250/TIMEBASE)   //  Defines a constant for a timeout of 250 mSec
#define T_500MSEC     (500/TIMEBASE)   //  Defines a constant for a timeout of 500 mSec
#define T_1SEC	      (1000/TIMEBASE)  //  Defines a constant for a timeout of 1,000  Sec
#define T_2SEC	      (2000/TIMEBASE)  //  Defines a constant for a timeout of 2,000  Sec

uint8_t  DebouncingTimeOut;    //  Debouncing TimeOut for digital inputs
uint16_t TimeOutWait;          //  TimeOut to manage pulse pressing
uint16_t TimeOutLed;           //  TimeOut to manage led pulse

uint8_t  TimeOutNorthGest;                 //  Timeout used to read the complete sequence of gestures on the North Electrode. Usefull to decode between "Touch gesture", "Tap Gesture" or "Double Tap gesture"
uint8_t  TimeOutSouthGest;                 //  Timeout used to read the complete sequence of gestures on the South Electrode. Usefull to decode between "Touch gesture", "Tap Gesture" or "Double Tap gesture"
uint8_t  TimeOutWestGest;                  //  Timeout used to read the complete sequence of gestures on the West Electrode. Usefull to decode between "Touch gesture", "Tap Gesture" or "Double Tap gesture"
uint8_t  TimeOutEastGest;                  //  Timeout used to read the complete sequence of gestures on the East Electrode. Usefull to decode between "Touch gesture", "Tap Gesture" or "Double Tap gesture"
uint8_t  TimeOutCentreGest;                //  Timeout used to read the complete sequence of gestures on the Centre Electrode. Usefull to decode between "Touch gesture", "Tap Gesture" or "Double Tap gesture"
//======================================================================

//======================================================================
//  I/O Input pin define
uint8_t P3_Button = 6;         //  Digital input. Button P3. See schematic
uint8_t P2_Button = 5;         //  Digital input. Button P2. See schematic
uint8_t P1_Button = 4;         //  Digital input. Button P1. See schematic
uint8_t Jumper1   = 3;         //  Digital input. Jumper J1. See schematic

uint8_t Led = 7; //  Digital Output. Led LD7

uint8_t Reset_MGC3130 = 8;     //  MGC3130 Reset line
uint8_t Ts_MGC3130    = 9;     //  MGC3130 TS line
uint8_t Addr_MGC3130  = 0x42;  //  Indirizzo HW MGC3130 (See datasheet MGC3130)
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
uint8_t  WestTouch[8];        // `
uint8_t  WestCounter;         //  >--- Set of variables used to store gestures sequence. Usefull to recognize between Touch, Tap and Double Tap gesture. West electrode
uint8_t  WestData;            // /
uint8_t  EastTouch[8];        // `
uint8_t  EastCounter;         //  >--- Set of variables used to store gestures sequence. Usefull to recognize between Touch, Tap and Double Tap gesture. East electrode
uint8_t  EastData;            // /
uint8_t  CentreTouch[8];      // `
uint8_t  CentreCounter;       //  >--- Set of variables used to store gestures sequence. Usefull to recognize between Touch, Tap and Double Tap gesture. Centre electrode
uint8_t  CentreData;          // /

#define TOUCH_CODE          1    //  Code, for touch gesture, used to fill the array gesture storage. See variables above
#define TAP_CODE            2    //  Code, for tap gesture, used to fill the array gesture storage. See variables above
#define DOUBLE_TAP_CODE     3    //  Code, for double tap gesture, used to fill the array gesture storage. See variables above

//============================================
//  Use these constants if no filter is set on the gestures that has been read from MGC3130
#define GESTURE_DOUBLE_TAP_WEST   10   // `
#define GESTURE_DOUBLE_TAP_EAST   10   //  >--- The "double tap gesture" is composed by a sequence of gestures. So the sum of gesture codes that have been read returns the value indicated
#define GESTURE_DOUBLE_TAP_CENTRE 10   // /
#define GESTURE_TAP_WEST          3    // `
#define GESTURE_TAP_EAST          3    //  >--- The "tap gesture" is composed by a sequence of gestures. So the sum of gesture codes that have been read returns the value indicated
#define GESTURE_TAP_CENTRE        3    // /
#define GESTURE_TOUCH_WEST        1    // `
#define GESTURE_TOUCH_EAST        1    //  >--- The "Touch Gesture"
#define GESTURE_TOUCH_CENTRE      1    // /
//============================================

//============================================
//  Use these constants if Touch filter is set on the gestures that has been read from MGC3130
//#define GESTURE_DOUBLE_TAP_WEST   7    // `
//#define GESTURE_DOUBLE_TAP_EAST   7    //  >--- The "double tap gesture" is composed by a sequence of gestures. So the sum of gesture codes that have been read returns the value indicated
//#define GESTURE_DOUBLE_TAP_CENTRE 7    // /
//#define GESTURE_TAP_WEST          2    // `
//#define GESTURE_TAP_EAST          2    //  >--- The "tap gesture" is composed by a sequence of gestures. So the sum of gesture codes that have been read returns the value indicated
//#define GESTURE_TAP_CENTRE        2    // /
//#define GESTURE_TOUCH_WEST        1    // `
//#define GESTURE_TOUCH_EAST        1    //  >--- The "Touch Gesture"
//#define GESTURE_TOUCH_CENTRE      1    // /
//============================================

//============================================
//  Use these constants if Tap filter is set on the gestures that has been read from MGC3130
//#define GESTURE_DOUBLE_TAP_WEST   5    // `
//#define GESTURE_DOUBLE_TAP_EAST   5    //  >--- The "double tap gesture" is composed by a sequence of gestures. So the sum of gesture codes that have been read returns the value indicated
//#define GESTURE_DOUBLE_TAP_CENTRE 5    // /
//#define GESTURE_TAP_WEST          3    // `
//#define GESTURE_TAP_EAST          3    //  >--- The "tap gesture" is composed by a sequence of gestures. So the sum of gesture codes that have been read returns the value indicated
//#define GESTURE_TAP_CENTRE        3    // /
//#define GESTURE_TOUCH_WEST        1    // `
//#define GESTURE_TOUCH_EAST        1    //  >--- The "Touch Gesture"
//#define GESTURE_TOUCH_CENTRE      1    // /
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
char    inChar;
boolean KeybMouseEnable;
boolean ActionInProgress;

byte    event;
String  output_string = "";
byte    ZoomStep;
//======================================================================

//======================================================================
void setup() {
  mgc3130.SetSerial(19200, SERIAL_8N1);
  mgc3130.SetAdd(Addr_MGC3130);
  mgc3130.Begin(Ts_MGC3130, Reset_MGC3130);
  
  SetInputPin();      //  Initialized Input pins
  SetOutputPin();     //  Initializes Output pins 
  SetupTimer();       //  Initializes Interrupt Timer 1 to manage TimeOut variables

  Input_Management = Input_Idle;              //  Initializes states machine used to manage the digital inputs (P1, P2, P3 and J1)
  Led_Management   = Led_Idle;                //  Initializes states machine used to manage led LD7. This led is used to recognize the pressure on P1, P2 and P3 buttons
  WestElectrode    = WestElectrode_Idle;      //  Initializes states machine used to manage led LD7. Used to recognize the pressure on the West electrode
  EastElectrode    = EastElectrode_Idle;      //  Initializes states machine used to manage led LD7. Used to recognize the pressure on the East electrode
  CentreElectrode  = CentreElectrode_Idle;    //  Initializes states machine used to manage led LD7. Used to recognize the pressure on the Centre electrode
  
  ZoomStep = 0;
}
//======================================================================

//======================================================================
void loop() {
  DebouncingInput();                   //  Debouncing digital inputs (P1, P2, P3 e J1)
  Input_Management();                  //  State Machine to manage Digital input
  Led_Management();
  
  if (mgc3130.GetTsLineStatus(Ts_MGC3130) == 0) {
      mgc3130.GetEvent();              //  Start read data from MGC3130
      mgc3130.DecodeGesture();         //  Decode Gesture
      mgc3130.ReleaseTsLine(Ts_MGC3130);  //  Release TS Line
  }
 
  //-------------------------------------------------------------- 
  if (Serial.available() > 0) {
    inChar = Serial.read();
    if (inChar == 'S') {
      //  Starts Keyboard and Mouse management  
      Keyboard.begin();
      Mouse.begin();
      KeybMouseEnable = true;
    }
    if (inChar == 'P') {
      //  Stop Keyboard and Mouse management
      Keyboard.end();
      Mouse.end();
      KeybMouseEnable = false;
    }   
  }
  //--------------------------------------------------------------
  
  //--------------------------------------------------------------
  if (KeybMouseEnable == true) {
    Led_Management();
    CentreElectrode();  //  Open software
    WestElectrode();    //  Open image
    EastElectrode();    //  Close image
    
    if (mgc3130.GestureOutput.Bit.GestWestEast == 1) {
      mgc3130.GestureOutput.Bit.GestWestEast = 0;
      if (ActionInProgress == false) {
        //  Visualize the next image
        ActionInProgress = true;
        Keyboard.press(0xD7);  // RIGHT ARROW
      }
    }
    if (mgc3130.GestureOutput.Bit.GestEastWest == 1) {
      mgc3130.GestureOutput.Bit.GestEastWest = 0;
      if (ActionInProgress == false) {
        //  Visualize the previous image
        ActionInProgress = true;
        Keyboard.press(0xD8);  // LEFT ARROW
      }
    }
    if (mgc3130.GestureOutput.Bit.GestNorthSouth == 1) {
      mgc3130.GestureOutput.Bit.GestNorthSouth = 0;
      if (ActionInProgress == false) {
        //  Visualize the next image
        ActionInProgress = true;
        Keyboard.press(0xD9);  // DOWN ARROW
      }
    }
    if (mgc3130.GestureOutput.Bit.GestSouthNorth == 1) {
      mgc3130.GestureOutput.Bit.GestSouthNorth = 0;
      if (ActionInProgress == false) {
        //  Visualize the previous image
        ActionInProgress = true;
        Keyboard.press(0xDA);  // UP ARROW
      }
    }
    if (mgc3130.GestureOutput.Bit.EdgeGestEastWest == 1) {
      mgc3130.GestureOutput.Bit.EdgeGestEastWest = 0;
      if (ActionInProgress == false) {
        ActionInProgress = true;
        if (ZoomStep > 2) {
          ZoomStep -= 1;
        } else {
          ZoomStep = 1;
        }
        Keyboard.press(0x30 + ZoomStep);  // ZOOM -
      }
    }
    if (mgc3130.GestureOutput.Bit.EdgeGestWestEast == 1) {
      mgc3130.GestureOutput.Bit.EdgeGestWestEast = 0;
      if (ActionInProgress == false) {
        ActionInProgress = true;
        if (ZoomStep < 9) {
          ZoomStep += 1;
        } else {
          ZoomStep = 9;
        }
        Keyboard.press(0x30 + ZoomStep);  // ZOOM +
      }
    }
    if (mgc3130.GestureOutput.Bit.GestClockWise == 1) {
      mgc3130.GestureOutput.Bit.GestClockWise = 0;
      if (ActionInProgress == false) {
        ActionInProgress = true;
        Keyboard.press(0x72);  // RIGHT rotation
      }
    }
    if (mgc3130.GestureOutput.Bit.GestCounterClockWise == 1) {
      mgc3130.GestureOutput.Bit.GestCounterClockWise = 0;
      if (ActionInProgress == false) {
        ActionInProgress = true;
        Keyboard.press(0x6C);  // LEFT rotation
      }
    }
    
    if (mgc3130.GestureOutput.Gesture == 0) {
      ActionInProgress = false;
      Keyboard.releaseAll();
    }
  }
  //-------------------------------------------------------------- 
}
//======================================================================

