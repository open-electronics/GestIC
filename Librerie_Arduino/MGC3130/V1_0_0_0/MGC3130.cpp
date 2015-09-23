/*  ===========================================================================
#  This is the library for MGC3130. 
#  
#  MGC3130 is a device that lets you control your hardware projects in a whole new way.  
#  MGC3130 detects hands movements and converts these  in gestures. These gestures
#  permit to manage a device without touch it. 
#
#  Written by Matteo Destro for Futura Group srl
#  www.Futurashop.it
#  www.open-electronics.org
#
#  BSD license, all text above must be included in any redistribution
#  ===========================================================================
#
#	REVISION	1.0.0	22/03/2015
#
#  ===========================================================================
#
#  INSTALLATION
#  The 3 library files (MGC3130.cpp, MGC3130.h and keywords.txt) in the MGC3130 folder should be placed in your Arduino Library folder.
#  Run the MGC3130_Demo.ino file from your Arduino IDE.
#
#  SUPPORT
#
#  info@open-electronics.org
#
#  ===========================================================================*/

#include "MGC3130.h"
#include <Wire.h>
#include <avr/pgmspace.h>

#ifdef __AVR__
 #define WIRE Wire
#endif

const char MGC3130Init[]  PROGMEM = "MGC3130 initialization in progress...wait";
const char MGC3130Ready[] PROGMEM = "MGC3130 device is ready";

void MGC3130::SetSerial(uint8_t Baud, uint8_t Config) {
	Serial.begin(19200, Config);
	delay(1000);
}

void MGC3130::SetAdd(uint8_t Addr) {
	_i2caddr = Addr;
}

void MGC3130::ResetDevice(uint8_t Rst) {
	pinMode(Rst, OUTPUT);    	// Set Reset line as Output
	digitalWrite(Rst, LOW);		// Reset MGC3130 device for 250 mSec
}

void MGC3130::ExitResetDevice(uint8_t Rst) {
	pinMode(Rst, OUTPUT);    	// Set Reset line as Output
	digitalWrite(Rst, HIGH);
}

void MGC3130::Begin(uint8_t Ts, uint8_t Rst) {
	ReadStringFLASH((uint8_t *)MGC3130Init, strlen(MGC3130Init), TRUE);	// Print "MGC3130 initialization in progress...wait"
	//FirstStartPacket = TRUE;
	WIRE.begin(_i2caddr);		// Initialize I2c with hardware address
	pinMode(Ts, INPUT);    		// Set TS line as Input
	pinMode(Rst, OUTPUT);    	// Set Reset line as Output
	digitalWrite(Rst, LOW);		// Reset MGC3130 device for 250 mSec
	delay(250);					// Delay
	digitalWrite(Rst, HIGH);
	delay(250);					// Delay
	ReadStringFLASH((uint8_t *)MGC3130Ready, strlen(MGC3130Ready), TRUE);	// Print "MGC3130 device is ready"
}

void MGC3130::ReleaseTsLine(uint8_t Ts) {
    digitalWrite(Ts, HIGH);		//	Set TS line as Input
    pinMode(Ts, INPUT);			//	Set TS level
}

boolean MGC3130::GetTsLineStatus(uint8_t Ts) {
	if (digitalRead(Ts) == 0) {
		//	MGC3130 put TS line LOW. Data is available
		pinMode(Ts, OUTPUT);	//	Set TS line as Output
		digitalWrite(Ts, LOW);	//	Set TS level
		return TRUE;			//  Return TRUE;
	}	
	return FALSE;				//	Return FALSE
}

void MGC3130::GetEvent(void) {
	int  Counter = 0;
	char c;
	// if (FirstStartPacket == TRUE) {
		// FirstStartPacket = FALSE;
		// WIRE.requestFrom((uint8_t)_i2caddr, (uint8_t)0x84);
	// } else {
		// WIRE.requestFrom((uint8_t)_i2caddr, (uint8_t)0x1A);
	// }
	WIRE.requestFrom((uint8_t)_i2caddr, (uint8_t)0x1A);
    while(WIRE.available())	{
		data[Counter++] = WIRE.read();
    }
	
	switch (data[3])
	{
		case ID_FW_VERSION:		
#ifdef PRINT_RAW_FW_INFO	
	PrintMGC3130RawFirmwareInfo();
#endif
			if (data[4] == 0xAA) {
				// Valid Gestic Library available
				Serial.print("FW Version: ");
				for (int i = 0; i < 120; i++) {
					c = char(data[i + 10]);
					Serial.print(c);
				}
				Serial.print("\n");
			}
			break;
			
		case ID_DATA_OUTPUT:		
			// ----------------------------------------
			// Save Data into internal array
			for (int i = 0; i < 4; i++) {
				GestureInfo.GestArray[i] = data[i + 10];
				TouchInfo.TouchArray[i]  = data[i + 14];
			}		
			GestureInfo.Gesture &= MASK_GESTURE_RAW;
			TouchInfo.Touch     &= MASK_TOUCH_RAW;
			AirWheelInfo = data[18];
			for (int i = 0; i < 6; i++) {
				xyzPosition.xyzArray[i] = data[i + 20];
			}
			// ----------------------------------------
			break;
						
		default:
			break;
	}
}

void MGC3130::DecodeGesture(void) {
	uint32_t Mask = 0x00000001;

	if (((TouchInfo.Touch ^ LastTouch) > 0) || ((GestureInfo.Gesture ^ LastGesture) > 0) ) {
#ifdef PRINT_RAW_DATA
	PrintMGC3130RawData();
#endif
		GestureOutput.Gesture = 0;
		if ((TouchInfo.Touch ^ LastTouch) > 0) {
			LastTouch = TouchInfo.Touch;
			for (int i = 0; i < 15; i++) {
				if ((TouchInfo.Touch & Mask) > 0) {
					GestureOutput.Gesture |= Mask; 
				}
				Mask = Mask << 1;
			}
		} else if ((GestureInfo.Gesture ^ LastGesture) > 0) {
			LastGesture = GestureInfo.Gesture;
			switch (GestureInfo.Bit.GestureCode)
			{
				case NO_GESTURE:
				case GESTURE_GARBAGE:
					break;
				case GESTURE_WEST_EAST:
					if (GestureInfo.Bit.EdgeFlick == 0) {
						GestureOutput.Gesture |= GESTURE_MASK_WEST_EAST;
					} else {
						GestureOutput.Gesture |= GESTURE_MASK_EDGE_WEST_EAST;
					}
					break;
				case GESTURE_EAST_WEST:
					if (GestureInfo.Bit.EdgeFlick == 0) {
						GestureOutput.Gesture |= GESTURE_MASK_EAST_WEST;
					} else {
						GestureOutput.Gesture |= GESTURE_MASK_EDGE_EAST_WEST;
					}
					break;
				case GESTURE_SOUTH_NORTH:
					if (GestureInfo.Bit.EdgeFlick == 0) {
						GestureOutput.Gesture |= GESTURE_MASK_SOUTH_NORTH;
					} else {
						GestureOutput.Gesture |= GESTURE_MASK_EDGE_SOUTH_NORTH;
					}
					break;
				case GESTURE_NORTH_SOUTH:
					if (GestureInfo.Bit.EdgeFlick == 0) {
						GestureOutput.Gesture |= GESTURE_MASK_NORTH_SOUTH;
					} else {
						GestureOutput.Gesture |= GESTURE_MASK_EDGE_NORTH_SOUTH;
					}
					break;
				case GESTURE_CLOCK_WISE:
					GestureOutput.Gesture |= GESTURE_MASK_CLOCK_WISE;
					break;
				case GESTURE_COUNTER_CLOCK_WISE:
					GestureOutput.Gesture |= GESTURE_MASK_COUNTER_CLOCK_WISE;
					break;
				default:
					break;
			}
		}
		//	Remove not desired Touch or Gesture Info. See MASK_FILTER_GESTURE into MGC3130.h file for details
		GestureOutput.Gesture &= ~(MASK_FILTER_GESTURE);
#ifdef PRINT_GESTURE_DATA
	PrintMGC3130Gesture();
#endif
#ifdef PRINT_XYZ
	PrintMGC3130xyz();
#endif
	}
}

#ifdef PRINT_RAW_FW_INFO
	const char RawInfoIndent[]      PROGMEM = "#####################################################################################\nRow Firmware Info from MGC3130 \n";
	const char HeaderInfo[]         PROGMEM = "Header: ";
	const char PayloadInfo[]        PROGMEM = "Payload: ";
	const char RawInfoCloseIndent[] PROGMEM = "\n#####################################################################################\n\n";

	void MGC3130::PrintMGC3130RawFirmwareInfo(void) { 
		if (data[3] = ID_FW_VERSION) {
			ReadStringFLASH((uint8_t *)RawInfoIndent, strlen(RawInfoIndent), FALSE);
			//----------------------------------------
			//	Header
			ReadStringFLASH((uint8_t *)HeaderInfo, strlen(HeaderInfo), FALSE);
			for (int i = 0; i < 4; i++) {
				SetHexPrintOutput(data[i]);
			}
			Serial.print("\n");
			//----------------------------------------
			
			//----------------------------------------
			//	Payload
			ReadStringFLASH((uint8_t *)PayloadInfo, strlen(PayloadInfo), FALSE);
			//----------------------------------------
			//	Firmware Valid
			SetHexPrintOutput(data[4]);
			Serial.print(" | ");
			//----------------------------------------

			//----------------------------------------
			//	HwRev
			for (int i = 5; i < 7; i++) {
				SetHexPrintOutput(data[i]);
			}
			Serial.print(" | ");
			//----------------------------------------

			//----------------------------------------
			//	ParameterStartAddr
			SetHexPrintOutput(data[7]);
			Serial.print(" | ");
			//----------------------------------------

			//----------------------------------------
			//	LibraryLoaderVersion
			for (int i = 8; i < 11; i++) {
				SetHexPrintOutput(data[i]);
			}
			Serial.print(" | ");
			//----------------------------------------

			//----------------------------------------
			//	FwStartAddr
			SetHexPrintOutput(data[11]);
			Serial.print(" | ");
			//----------------------------------------		

			//----------------------------------------
			//	FWVersion
			for (int i = 12; i < 132; i++) {
				SetHexPrintOutput(data[i]);
			}
			ReadStringFLASH((uint8_t *)RawInfoCloseIndent, strlen(RawInfoCloseIndent), FALSE);
			//----------------------------------------
		}
	}
#endif

#ifdef PRINT_RAW_DATA
	const char RawDataIndent[]      PROGMEM = "#####################################################################################\nRow data from MGC3130 \n";
	const char HeaderRawData[]      PROGMEM = "Header: ";
	const char PayloadRawData[]     PROGMEM = "Payload: ";
	const char RawDataCloseIndent[] PROGMEM = "\n#####################################################################################\n\n";
	
	void MGC3130::PrintMGC3130RawData(void) { 
		if (data[3] = ID_DATA_OUTPUT) {
			ReadStringFLASH((uint8_t *)RawDataIndent, strlen(RawDataIndent), FALSE);
			//----------------------------------------
			//	Header
			ReadStringFLASH((uint8_t *)HeaderRawData, strlen(HeaderRawData), FALSE);
			for (int i = 0; i < 4; i++) {
				SetHexPrintOutput(data[i]);
			}
			Serial.print("\n");
			//----------------------------------------
			
			//----------------------------------------
			//	Payload
			ReadStringFLASH((uint8_t *)PayloadRawData, strlen(PayloadRawData), FALSE);	
			//----------------------------------------
			//	DataOutputConfigMask
			for (int i = 4; i < 6; i++) {
				SetHexPrintOutput(data[i]);
			}
			Serial.print(" | ");
			//----------------------------------------
			
			//----------------------------------------
			//	TimeStamp
			SetHexPrintOutput(data[6]);		
			Serial.print(" | ");
			//----------------------------------------
			
			//----------------------------------------
			//	SystemInfo
			SetHexPrintOutput(data[7]);		
			Serial.print(" | ");		
			//----------------------------------------
			
			//----------------------------------------
			//	DSPStatus
			for (int i = 8; i < 10; i++) {
				SetHexPrintOutput(data[i]);
			}
			Serial.print(" | ");
			//----------------------------------------

			//----------------------------------------
			//	GestureInfo
			for (int i = 10; i < 14; i++) {
				SetHexPrintOutput(data[i]);
			}
			Serial.print(" | ");
			//----------------------------------------
			
			//----------------------------------------
			//	TouchInfo
			for (int i = 14; i < 18; i++) {
				SetHexPrintOutput(data[i]);
			}
			Serial.print(" | ");
			//----------------------------------------
			
			//----------------------------------------
			//	AirWheelInfo
			for (int i = 18; i < 20; i++) {
				SetHexPrintOutput(data[i]);
			}
			Serial.print(" | ");
			//----------------------------------------	
			
			//----------------------------------------
			//	xyzPosition
			for (int i = 20; i < 26; i++) {
				SetHexPrintOutput(data[i]);
			}
			ReadStringFLASH((uint8_t *)RawDataCloseIndent, strlen(RawDataCloseIndent), FALSE);
			//----------------------------------------
		}	
	}
#endif

#ifdef PRINT_GESTURE_DATA
	const char TouchSouth[]  PROGMEM = "Touch South";
	const char TouchWest[]   PROGMEM = "Touch West";
	const char TouchNorth[]  PROGMEM = "Touch North";
	const char TouchEast[]   PROGMEM = "Touch East";
	const char TouchCentre[] PROGMEM = "Touch Centre";

	const char TapSouth[]  PROGMEM = "Tap South";
	const char TapWest[]   PROGMEM = "Tap West";
	const char TapNorth[]  PROGMEM = "Tap North";
	const char TapEast[]   PROGMEM = "Tap East";
	const char TapCentre[] PROGMEM = "Tap Centre";

	const char DoubleTapSouth[]  PROGMEM = "Double Tap South";
	const char DoubleTapWest[]   PROGMEM = "Double Tap West";
	const char DoubleTapNorth[]  PROGMEM = "Double Tap North";
	const char DoubleTapEast[]   PROGMEM = "Double Tap East";
	const char DoubleTapCentre[] PROGMEM = "Double Tap Centre";
	
	const char GestureWestToEast[]   PROGMEM = "Gesture West to East";
	const char GestureEastToWest[]   PROGMEM = "Gesture East to West";
	const char GestureNorthToSouth[] PROGMEM = "Gesture North to South";
	const char GestureSouthToNorth[] PROGMEM = "Gesture South to North";
	
	const char GestureEdgeWestToEast[]   PROGMEM = "Gesture Edge West to East";
	const char GestureEdgeEastToWest[]   PROGMEM = "Gesture Edge East to West";
	const char GestureEdgeNorthToSouth[] PROGMEM = "Gesture Edge North to South";
	const char GestureEdgeSouthToNorth[] PROGMEM = "Gesture Edge South to North";	
	
	const char GestureClockWise[]        PROGMEM = "Gesture Clock Wise";	
	const char GestureCounterClockWise[] PROGMEM = "Gesture Counter Clock Wise";	
	
	void MGC3130::PrintMGC3130Gesture(void) {
		//----------------------------------------
		if (GestureOutput.Bit.TouchSouth > 0) {
			ReadStringFLASH((uint8_t *)TouchSouth, strlen(TouchSouth), TRUE);
		}
		if (GestureOutput.Bit.TouchWest > 0) {
			ReadStringFLASH((uint8_t *)TouchWest, strlen(TouchWest), TRUE);
		}
		if (GestureOutput.Bit.TouchNorth > 0) {
			ReadStringFLASH((uint8_t *)TouchNorth, strlen(TouchNorth), TRUE);
		}
		if (GestureOutput.Bit.TouchEast > 0) {
			ReadStringFLASH((uint8_t *)TouchEast, strlen(TouchEast), TRUE);
		}
		if (GestureOutput.Bit.TouchCentre > 0) {
			ReadStringFLASH((uint8_t *)TouchCentre, strlen(TouchCentre), TRUE);
		}	
		//----------------------------------------

		//----------------------------------------
		if (GestureOutput.Bit.TapSouth > 0) {
			ReadStringFLASH((uint8_t *)TapSouth, strlen(TapSouth), TRUE);
		}
		if (GestureOutput.Bit.TapWest > 0) {
			ReadStringFLASH((uint8_t *)TapWest, strlen(TapWest), TRUE);
		}
		if (GestureOutput.Bit.TapNorth > 0) {
			ReadStringFLASH((uint8_t *)TapNorth, strlen(TapNorth), TRUE);
		}
		if (GestureOutput.Bit.TapEast > 0) {
			ReadStringFLASH((uint8_t *)TapEast, strlen(TapEast), TRUE);
		}
		if (GestureOutput.Bit.TapCentre > 0) {
			ReadStringFLASH((uint8_t *)TapCentre, strlen(TapCentre), TRUE);
		}
		//----------------------------------------
		
		//----------------------------------------
		if (GestureOutput.Bit.DoubleTapSouth > 0) {
			ReadStringFLASH((uint8_t *)DoubleTapSouth, strlen(DoubleTapSouth), TRUE);
		}
		if (GestureOutput.Bit.DoubleTapWest > 0) {
			ReadStringFLASH((uint8_t *)DoubleTapWest, strlen(DoubleTapWest), TRUE);
		}
		if (GestureOutput.Bit.DoubleTapNorth > 0) {
			ReadStringFLASH((uint8_t *)DoubleTapNorth, strlen(DoubleTapNorth), TRUE);
		}
		if (GestureOutput.Bit.DoubleTapEast > 0) {
			ReadStringFLASH((uint8_t *)DoubleTapEast, strlen(DoubleTapEast), TRUE);
		}
		if (GestureOutput.Bit.DoubleTapCentre > 0) {
			ReadStringFLASH((uint8_t *)DoubleTapCentre, strlen(DoubleTapCentre), TRUE);
		}
		//----------------------------------------

		//----------------------------------------	
		if (GestureOutput.Bit.GestWestEast > 0) {
			ReadStringFLASH((uint8_t *)GestureWestToEast, strlen(GestureWestToEast), TRUE);
		}
		if (GestureOutput.Bit.GestEastWest > 0) {
			ReadStringFLASH((uint8_t *)GestureEastToWest, strlen(GestureEastToWest), TRUE);
		}	
		if (GestureOutput.Bit.GestSouthNorth > 0) {
			ReadStringFLASH((uint8_t *)GestureNorthToSouth, strlen(GestureNorthToSouth), TRUE);
		}
		if (GestureOutput.Bit.GestNorthSouth > 0) {
			ReadStringFLASH((uint8_t *)GestureSouthToNorth, strlen(GestureSouthToNorth), TRUE);
		}
		if (GestureOutput.Bit.EdgeGestWestEast > 0) {
			ReadStringFLASH((uint8_t *)GestureEdgeWestToEast, strlen(GestureEdgeWestToEast), TRUE);
		}
		if (GestureOutput.Bit.EdgeGestEastWest > 0) {
			ReadStringFLASH((uint8_t *)GestureEdgeEastToWest, strlen(GestureEdgeEastToWest), TRUE);
		}
		if (GestureOutput.Bit.EdgeGestSouthNorth > 0) {
			ReadStringFLASH((uint8_t *)GestureEdgeNorthToSouth, strlen(GestureEdgeNorthToSouth), TRUE);
		}
		if (GestureOutput.Bit.EdgeGestNorthSouth > 0) {
			ReadStringFLASH((uint8_t *)GestureEdgeSouthToNorth, strlen(GestureEdgeSouthToNorth), TRUE);
		}
		if (GestureOutput.Bit.GestClockWise > 0) {
			ReadStringFLASH((uint8_t *)GestureClockWise, strlen(GestureClockWise), TRUE);
		}
		if (GestureOutput.Bit.GestCounterClockWise > 0) {
			ReadStringFLASH((uint8_t *)GestureCounterClockWise, strlen(GestureCounterClockWise), TRUE);
		}
		//----------------------------------------	
	}
#endif

#ifdef PRINT_XYZ
	const char X[] PROGMEM = "The X coordinate is: ";
	const char Y[] PROGMEM = "The Y coordinate is: ";
	const char Z[] PROGMEM = "The Z coordinate is: ";

	void MGC3130::PrintMGC3130xyz(void) {
		if (Previous_x_pos != xyzPosition.xyzWord.x_pos) {
			Previous_x_pos = xyzPosition.xyzWord.x_pos;
			ReadStringFLASH((uint8_t *)X, strlen(X), TRUE);
			Serial.println(xyzPosition.xyzWord.x_pos, DEC);
		}
		if (Previous_y_pos != xyzPosition.xyzWord.y_pos) {
			Previous_y_pos = xyzPosition.xyzWord.y_pos;
			ReadStringFLASH((uint8_t *)Y, strlen(Y), TRUE);
			Serial.println(xyzPosition.xyzWord.y_pos, DEC);
		}
		if (Previous_z_pos != xyzPosition.xyzWord.z_pos) {
			Previous_z_pos = xyzPosition.xyzWord.z_pos;		
			ReadStringFLASH((uint8_t *)Z, strlen(Z), TRUE);
			Serial.println(xyzPosition.xyzWord.z_pos, DEC);		
		}
	}
#endif

//----------------------------------------
//	This function is used to read the string data by flash and print the data read on the screen
//	It is useful to save SRAM memory instead that used the "println" function with a string type parameter
//	The string type parameters are saved into FLASH memory using the "PROGMEM" function
void MGC3130::ReadStringFLASH(uint8_t *FlashPointer, uint8_t Lenght, boolean PrintCR) {
	uint8_t k;
	char myChar;
	for (k = 0; k < Lenght; k++) {
		myChar = pgm_read_byte_near(FlashPointer + k);
		Serial.print(myChar);
	}
	if (PrintCR == TRUE) {
		Serial.print("\n");
	}
}
//----------------------------------------

void MGC3130::SetHexPrintOutput(uint8_t Data) {
	if (Data < 0x10) {
		Serial.print(0, HEX);
		Serial.print(Data, HEX);
	} else {
		Serial.print(Data, HEX);
	}
}