#===========================================================================
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
#	REVISION	1.0.0	30/06/2015
#
#  ===========================================================================
#
#	REVISION	1.1.0	13/03/2016
#
#	- Fixed Bug print North to South gestic and South to North gestic
#
#	- Added new gesture recognitions:
#		*	Gesture Wave X
#		*	Gesture Wave Y
#		*	Gesture Hold
#		*	Gesture Presence
#		*	Gesture Double West To East 
#		*	Gesture Double East To Weast 
#		*	Gesture Double North To South 
#		*	Gesture Double South To North 
#
#  ===========================================================================
#
#  SUPPORT
#
#  info@open-electronics.org
#
#===========================================================================

from ctypes import *
from MGC3130_DefVar import *

import smbus
import time
import sys

#I2Cbus = smbus.SMBus(0)
I2Cbus = smbus.SMBus(1)

_Data          = c_ubyte * 256
_i2caddr       = 0x42
_Counter       = 0
_FirstTelegram = False
_GestInfo      = GestureInfo()
_TouchInfo     = TouchInfo()
_xyz           = Coordinates()
_GestOutput    = Gesture()

#===================================================================
#	Enable/Disable code to debug
EnablePrintMGC3130RawFirmwareInfo = True
EnablePrintMGC3130RawData         = True
EnablePrintMGC3130Gesture         = True
EnablePrintMGC3130xyz             = False
#===================================================================

#===================================================================
def MGC3130_SetAdd(Addr):
	global _Data
	_i2caddr = Addr
#===================================================================

#===================================================================
def MGC3130_Begin(GPIO, Ts, Rst):
	global _FirstTelegram
	print("MGC3130 initialization in progress...wait")
	GPIO.setup(Ts, GPIO.IN, pull_up_down=GPIO.PUD_UP)	# MGC3130 TS Line. Input mode
	#GPIO.add_event_detect(Ts, GPIO.FALLING, bouncetime=100)
	GPIO.setup(Rst, GPIO.OUT, initial=GPIO.HIGH)  		# MGC3130 RESET Line
	GPIO.output(Rst, GPIO.LOW)							# Reset MGC3130 Device
	time.sleep(0.250)									# Delay 250mSec
	GPIO.output(Rst, GPIO.HIGH)							# Remove Reset MGC3130 Device		 
	#time.sleep(0.250)									# Delay 250mSec
	#_FirstTelegram = True
	
	time.sleep(0.500)									# Delay 500mSec
	_FirstTelegram = False
	print("MGC3130 device is ready")
#===================================================================

#===================================================================
def MGC3130_ResetDevice(GPIO, Rst):
	GPIO.setup(Rst, GPIO.OUT, initial=GPIO.HIGH)  		# MGC3130 RESET Line
	GPIO.output(Rst, GPIO.LOW)
#===================================================================

#===================================================================
def MGC3130_ExitResetDevice(GPIO, Rst):
	GPIO.setup(Rst, GPIO.OUT, initial=GPIO.HIGH)  		# MGC3130 RESET Line
	GPIO.output(Rst, GPIO.HIGH)
#===================================================================
	
#===================================================================
def MGC3130_ReleaseTsLine(GPIO, Ts):
	#print("Release TS Line")
	GPIO.output(Ts, GPIO.HIGH)
	time.sleep(0.050)
	GPIO.setup(Ts, GPIO.IN, pull_up_down=GPIO.PUD_UP)	# MGC3130 TS Line. Input mode
#===================================================================

#===================================================================	
def MGC3130_GetTsLineStatus(GPIO, Ts):
	if (GPIO.input(Ts) == GPIO.LOW):
		#print("Get TS Line")
		time.sleep(0.050)
		GPIO.setup(Ts, GPIO.OUT)	# MGC3130 TS Line. Output mode
		GPIO.output(Ts, GPIO.LOW)
		return True
	else:
		return False
#===================================================================

#===================================================================
def MGC3130_GetEvent():
	global EnablePrintMGC3130RawFirmwareInfo
	global _Counter
	global _Data
	global _FirstTelegram
	global _GestInfo
	global _TouchInfo
	global _xyz
	
	_Counter = 0
		
	if (_FirstTelegram == True):
		_FirstTelegram = False
		_Data = I2Cbus.read_i2c_block_data(_i2caddr, 0x00, 0x20)
	else:
		_Data = I2Cbus.read_i2c_block_data(_i2caddr, 0x00, 0x1A)

	if (_Data[3] == ID_FW_VERSION):
		if (_Data[4] == 0xAA):
			if (EnablePrintMGC3130RawFirmwareInfo == True):
				MGC3130_PrintMGC3130RawFirmwareInfo()

			print("###################################################")
			print("Valid Library detected")
			print 'Hardware Rev: {}.{}' .format(_Data[5], _Data[6])
			print 'Library Loader Version: {}.{}' .format(_Data[9], _Data[8])
			#===========================================
			sys.stdout.write("Gestic Library Version: ")
			TempStr = ""
			for i in range(12, 17):
				if (i < len(_Data)):
					TempStr = TempStr + chr(_Data[i])
			sys.stdout.write(TempStr + "\n")
			#===========================================
			
			#===========================================
			sys.stdout.write("Platform: ")
			TempStr = ""
			for i in range(20, 31):
				if (i < len(_Data)):
					TempStr = TempStr + chr(_Data[i])
			sys.stdout.write(TempStr +"\n")
			#===========================================
			print("###################################################")
			print("\n")
			
		else:
			print("Invalid Library detected")
			
	elif (_Data[3] == ID_DATA_OUTPUT):
		#===========================================
		# Save Data into internal array
		for i in range(4):
			_GestInfo.GestInfoArray[i]   = _Data[i + 10]
			_TouchInfo.TouchInfoArray[i] = _Data[i + 14]
				
		_GestInfo.GestureInfoLong &= MASK_GESTURE_RAW
		_TouchInfo.TouchInfoLong  &= MASK_TOUCH_RAW	
		AirWheelInfo = _Data[18]
		for i in range(6):
			if (i < len(_Data)):
				_xyz.xyzArray[i] = _Data[i + 20]
		#===========================================
	else:
		print("Telegram not managed")
#===================================================================	

#===================================================================
def MGC3130_DecodeGesture():
	global EnablePrintMGC3130RawData
	global EnablePrintMGC3130Gesture
	global EnablePrintMGC3130xyz
	global _GestInfo
	global _TouchInfo
	global _GestOutput
	global LastTouch
	global LastGesture
	
	Mask = 0x00000001
	if (((_TouchInfo.TouchInfoLong ^ LastTouch) > 0) or ((_GestInfo.GestureInfoLong ^ LastGesture) > 0)):
		if (EnablePrintMGC3130RawData == True):
			MGC3130_PrintMGC3130RawData()
		_GestOutput.GestureLong = 0x00000000
		if ((_TouchInfo.TouchInfoLong ^ LastTouch) > 0):
			LastTouch = _TouchInfo.TouchInfoLong
			for i in range (15):
				if ((_TouchInfo.TouchInfoLong & Mask) > 0):
					_GestOutput.GestureLong |= Mask
				Mask = Mask << 1
		elif ((_GestInfo.GestureInfoLong ^ LastGesture) > 0):
			LastGesture = _GestInfo.GestureInfoLong
			if (_GestInfo.GestureInfo32Bit.GestureCode == NO_GESTURE):
				print("No Gesture")
			elif (_GestInfo.GestureInfo32Bit.GestureCode == GESTURE_GARBAGE):
				print("Garbage Gesture")
			elif (_GestInfo.GestureInfo32Bit.GestureCode == GESTURE_EDGE_WEST_EAST):
				_GestOutput.GestureLong |= GESTURE_MASK_EDGE_WEST_EAST
			elif (_GestInfo.GestureInfo32Bit.GestureCode == GESTURE_EDGE_EAST_WEST):
				_GestOutput.GestureLong |= GESTURE_MASK_EDGE_EAST_WEST
			elif (_GestInfo.GestureInfo32Bit.GestureCode == GESTURE_EDGE_SOUTH_NORTH):
				_GestOutput.GestureLong |= GESTURE_MASK_EDGE_SOUTH_NORTH
			elif (_GestInfo.GestureInfo32Bit.GestureCode == GESTURE_EDGE_NORTH_SOUTH):
				_GestOutput.GestureLong |= GESTURE_MASK_EDGE_NORTH_SOUTH
			elif (_GestInfo.GestureInfo32Bit.GestureCode == GESTURE_WEST_EAST):
				if (_GestInfo.GestureInfo32Bit.Edgeflick == 0):
					_GestOutput.GestureLong |= GESTURE_MASK_WEST_EAST
				else:
					_GestOutput.GestureLong |= GESTURE_MASK_EDGE_WEST_EAST
			elif (_GestInfo.GestureInfo32Bit.GestureCode == GESTURE_EAST_WEST):
				if (_GestInfo.GestureInfo32Bit.Edgeflick == 0):
					_GestOutput.GestureLong |= GESTURE_MASK_EAST_WEST
				else:
					_GestOutput.GestureLong |= GESTURE_MASK_EDGE_EAST_WEST
			elif (_GestInfo.GestureInfo32Bit.GestureCode == GESTURE_SOUTH_NORTH):
				if (_GestInfo.GestureInfo32Bit.Edgeflick == 0):
					_GestOutput.GestureLong |= GESTURE_MASK_SOUTH_NORTH
				else:
					_GestOutput.GestureLong |= GESTURE_MASK_EDGE_SOUTH_NORTH
			elif (_GestInfo.GestureInfo32Bit.GestureCode == GESTURE_NORTH_SOUTH):
				if (_GestInfo.GestureInfo32Bit.Edgeflick == 0):
					_GestOutput.GestureLong |= GESTURE_MASK_NORTH_SOUTH
				else:
					_GestOutput.GestureLong |= GESTURE_MASK_EDGE_NORTH_SOUTH
			elif (_GestInfo.GestureInfo32Bit.GestureCode == GESTURE_CLOCK_WISE):
				_GestOutput.GestureLong |= GESTURE_MASK_CLOCK_WISE
			elif (_GestInfo.GestureInfo32Bit.GestureCode == GESTURE_COUNTER_CLOCK_WISE):
				_GestOutput.GestureLong |= GESTURE_MASK_COUNTER_CLOCK_WISE
			elif (_GestInfo.GestureInfo32Bit.GestureCode == GESTURE_WAVE_X):
				_GestOutput.GestureLong |= GESTURE_MASK_WAVE_X
			elif (_GestInfo.GestureInfo32Bit.GestureCode == GESTURE_WAVE_Y):
				_GestOutput.GestureLong |= GESTURE_MASK_WAVE_Y
			elif (_GestInfo.GestureInfo32Bit.GestureCode == GESTURE_HOLD):
				_GestOutput.GestureLong |= GESTURE_MASK_HOLD
			elif (_GestInfo.GestureInfo32Bit.GestureCode == GESTURE_PRESENCE):
				_GestOutput.GestureLong |= GESTURE_MASK_PRESENCE
			elif (_GestInfo.GestureInfo32Bit.GestureCode == GESTURE_DOUBLE_WEST_EAST):
				_GestOutput.GestureLong |= GESTURE_MASK_DOUBLE_WEST_EAST		
			elif (_GestInfo.GestureInfo32Bit.GestureCode == GESTURE_DOUBLE_EAST_WEST):
				_GestOutput.GestureLong |= GESTURE_MASK_DOUBLE_EAST_WEST			
			elif (_GestInfo.GestureInfo32Bit.GestureCode == GESTURE_DOUBLE_SOUTH_NORTH):
				_GestOutput.GestureLong |= GESTURE_MASK_DOUBLE_SOUTH_NORTH			
			elif (_GestInfo.GestureInfo32Bit.GestureCode == GESTURE_DOUBLE_NORTH_SOUTH):	
				_GestOutput.GestureLong |= GESTURE_MASK_DOUBLE_NORTH_SOUTH
			
		_GestOutput.GestureLong &= ~(MASK_FILTER_GESTURE)
		
		if (EnablePrintMGC3130Gesture == True):
			MGC3130_PrintMGC3130Gesture()
		if (EnablePrintMGC3130xyz == True):
			MGC3130_PrintMGC3130xyz()
#===================================================================

#===================================================================
def MGC3130_PrintMGC3130RawFirmwareInfo():
	global _Data
	
	RawInfoIndent      = "#####################################################################################\nRow Firmware Info from MGC3130 \n"
	HeaderInfo         = "Header: "
	PayloadInfo        = "Payload: "
	RawInfoCloseIndent ="\n#####################################################################################\n\n";
	
	if (_Data[3] == ID_FW_VERSION):
		sys.stdout.write(RawInfoIndent)
		#===========================================
		#	Header		
		sys.stdout.write(HeaderInfo)
		for i in range(4):
			MGC3130_SetHexPrintOutput(_Data[i])
		print("\n")
		#===========================================
	
		#===========================================
		#	Payload
		sys.stdout.write(PayloadInfo)
		MGC3130_SetHexPrintOutput(_Data[4])
		sys.stdout.write(" | ")
		#===========================================
		
		#===========================================
		#	HwRev
		for i in range(5, 7):
			MGC3130_SetHexPrintOutput(_Data[i])	
		sys.stdout.write(" | ")
		#===========================================
			
		#===========================================
		#	ParameterStartAddr
		MGC3130_SetHexPrintOutput(_Data[7])
		sys.stdout.write(" | ")
		#===========================================			
		
		#===========================================
		#	LibraryLoaderVersion
		for i in range(8, 11):
			MGC3130_SetHexPrintOutput(_Data[i])	
		sys.stdout.write(" | ")
		#===========================================		
		
		#===========================================
		#	FwStartAddr
		MGC3130_SetHexPrintOutput(_Data[11])
		sys.stdout.write(" | ")
		#===========================================
		
		#===========================================
		#	LibraryLoaderVersion
		for i in range(12, len(_Data)):
			MGC3130_SetHexPrintOutput(_Data[i])	
		sys.stdout.write(RawInfoCloseIndent)
		#===========================================			
					
def MGC3130_SetHexPrintOutput(Data):
	if (Data < 0x10):
		sys.stdout.write("0x" + format(Data, "02x") + " ")
	else:
		sys.stdout.write(hex(Data) + " ")

#===================================================================

#===================================================================
def MGC3130_PrintMGC3130RawData():
	global _Data
	
	RawDataIndent      = "#####################################################################################\nRow data from MGC3130 \n"
	HeaderRawData      = "Header: "
	PayloadRawData     = "Payload: "
	RawDataCloseIndent ="\n#####################################################################################\n\n";
	
	if (_Data[3] == ID_DATA_OUTPUT):
		sys.stdout.write(RawDataIndent)
		#===========================================
		#	Header		
		sys.stdout.write(HeaderRawData)
		for i in range(4):
			MGC3130_SetHexPrintOutput(_Data[i])
		print("\n")
		#===========================================
		
		#===========================================
		#	Payload
		#	DataOutputConfigMask
		sys.stdout.write(PayloadRawData)
		for i in range(4, 6):
			MGC3130_SetHexPrintOutput(_Data[i])
		sys.stdout.write(" | ")
		#===========================================		
	
		#===========================================
		#	TimeStamp
		MGC3130_SetHexPrintOutput(_Data[6])
		sys.stdout.write(" | ")
		#===========================================		
		
		#===========================================
		#	SystemInfo
		MGC3130_SetHexPrintOutput(_Data[7])
		sys.stdout.write(" | ")
		#===========================================		
		
		#===========================================
		#	DSPStatus
		for i in range(8, 10):
			MGC3130_SetHexPrintOutput(_Data[i])
		sys.stdout.write(" | ")
		#===========================================		
		
		#===========================================
		#	GestureInfo
		for i in range(10, 14):
			MGC3130_SetHexPrintOutput(_Data[i])
		sys.stdout.write(" | ")
		#===========================================	
		
		#===========================================
		#	TouchInfo
		for i in range(14, 18):
			MGC3130_SetHexPrintOutput(_Data[i])
		sys.stdout.write(" | ")
		#===========================================
		
		#===========================================
		#	AirWheelInfo
		for i in range(18, 20):
			MGC3130_SetHexPrintOutput(_Data[i])
		sys.stdout.write(" | ")
		#===========================================
		
		#===========================================
		#	xyzPosition
		for i in range(20, 26):
			MGC3130_SetHexPrintOutput(_Data[i])
		sys.stdout.write(" | ")
		#===========================================
		
		sys.stdout.write(RawDataCloseIndent)				
#===================================================================

#===================================================================
def MGC3130_PrintMGC3130Gesture():
	global _GestOutput
	
	if (_GestOutput.Gesture64Bit.TouchSouth):
		print ("Touch South")
		print("\n")
	if (_GestOutput.Gesture64Bit.TouchWest):
		print ("Touch West")
		print("\n")
	if (_GestOutput.Gesture64Bit.TouchNorth):
		print ("Touch North")
		print("\n")
	if (_GestOutput.Gesture64Bit.TouchEast):
		print ("Touch East")		
		print("\n")
	if (_GestOutput.Gesture64Bit.TouchCentre):		   	 
		print ("Touch Centre")
		print("\n")
		
	if (_GestOutput.Gesture64Bit.TapSouth):
		print ("Tap South")
		print("\n")
	if (_GestOutput.Gesture64Bit.TapWest):
		print ("Tap West")
		print("\n")
	if (_GestOutput.Gesture64Bit.TapNorth):
		print ("Tap North")
		print("\n")
	if (_GestOutput.Gesture64Bit.TapEast):
		print ("Tap East")		
		print("\n")
	if (_GestOutput.Gesture64Bit.TapCentre):		   	 
		print ("Tap Centre")		
		print("\n")
		
	if (_GestOutput.Gesture64Bit.DoubleTapSouth):
		print ("Double Tap South")
		print("\n")
	if (_GestOutput.Gesture64Bit.DoubleTapWest):
		print ("Double Tap West")
		print("\n")
	if (_GestOutput.Gesture64Bit.DoubleTapNorth):
		print ("Double Tap North")
		print("\n")
	if (_GestOutput.Gesture64Bit.DoubleTapEast):
		print ("Double Tap East")		
		print("\n")
	if (_GestOutput.Gesture64Bit.DoubleTapCentre):		   	 
		print ("Double Tap Centre")	
		print("\n")
		
	if (_GestOutput.Gesture64Bit.GestWestEast):
		print ("Gesture Flick West to East")
		print("\n")
	if (_GestOutput.Gesture64Bit.GestEastWest):
		print ("Gesture Flick East to West")
		print("\n")
	if (_GestOutput.Gesture64Bit.GestSouthNorth):
		print ("Gesture Flick South to North")
		print("\n")
	if (_GestOutput.Gesture64Bit.GestNorthSouth):
		print ("Gesture Flick North to South")			
		print("\n")
		
	if (_GestOutput.Gesture64Bit.EdgeGestWestEast):
		print ("Gesture Flick Edge West to East")
		print("\n")
	if (_GestOutput.Gesture64Bit.EdgeGestEastWest):
		print ("Gesture Flick Edge East to West")
		print("\n")
	if (_GestOutput.Gesture64Bit.EdgeGestSouthNorth):
		print ("Gesture Flick Edge South to North")
		print("\n")
	if (_GestOutput.Gesture64Bit.EdgeGestNorthSouth):
		print ("Gesture Flick Edge North to South")
		print("\n")
		
	if (_GestOutput.Gesture64Bit.GestClockWise):
		print ("Gesture Clock Wise")
		print("\n")
	if (_GestOutput.Gesture64Bit.GestCounterClockWise):
		print ("Gesture Counter Clock Wise")		
		print("\n")
		
	if (_GestOutput.Gesture64Bit.GestWaveX):
		print ("Gesture Wave X")
		print("\n")
	if (_GestOutput.Gesture64Bit.GestWaveY):
		print ("Gesture Wave Y")		
		print("\n")
	if (_GestOutput.Gesture64Bit.GestHold):
		print ("Gesture Hold")
		print("\n")
	if (_GestOutput.Gesture64Bit.GestPresence):
		print ("Gesture Presence")		
		print("\n")
		
	if (_GestOutput.Gesture64Bit.DoubleGestWestEast):
		print ("Gesture Double West to East")
		print("\n")
	if (_GestOutput.Gesture64Bit.DoubleGestEastWest):
		print ("Gesture Double East to West")		
		print("\n")
	if (_GestOutput.Gesture64Bit.DoubleSouthNorth):
		print ("Gesture Double South to North")
		print("\n")
	if (_GestOutput.Gesture64Bit.DoubleGestNorthSouth):
		print ("Gesture Double North to South")		
		print("\n")
				
#===================================================================

#===================================================================
def MGC3130_PrintMGC3130xyz():
	global _xyz
	global Last_X
	global Last_Y
	global Last_Z
	
	if (Last_X != _xyz.xyz.x):
		Last_X = _xyz.xyz.x
		sys.stdout.write("The X coordinate is: ")
		print(_xyz.xInt)
	if (Last_Y != _xyz.xyz.y):
		Last_Y = _xyz.xyz.y
		sys.stdout.write("The Y coordinate is: ")
		print(_xyz.yInt)
	if (Last_Z != _xyz.xyz.z):
		Last_Z = _xyz.xyz.z
		sys.stdout.write("The Z coordinate is: ")
		print(_xyz.zInt)
	print("\n")
#===================================================================
