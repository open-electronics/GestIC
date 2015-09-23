#==================================================================================
# ctypes type		C type						Python type
#==================================================================================
#   c_bool			_Bool						bool (1)
#----------------------------------------------------------------------------------
#   c_char			char						1-character string
#----------------------------------------------------------------------------------
#   c_wchar			wchar_t						1-character unicode string
#----------------------------------------------------------------------------------
#   c_byte			char						int/long
#----------------------------------------------------------------------------------
#   c_ubyte			unsigned char				int/long
#----------------------------------------------------------------------------------
#   c_short			short						int/long
#----------------------------------------------------------------------------------
#   c_ushort		unsigned short				int/long
#----------------------------------------------------------------------------------
#   c_int			int							int/long
#----------------------------------------------------------------------------------
#   c_uint			unsigned int				int/long
#----------------------------------------------------------------------------------
#   c_long			long						int/long
#----------------------------------------------------------------------------------
#   c_ulong			unsigned long				int/long
#----------------------------------------------------------------------------------
#   c_longlong		__int64 or long long		int/long
#----------------------------------------------------------------------------------
#   c_ulonglong		unsigned __int64 or 
#                   unsigned long long			int/long
#----------------------------------------------------------------------------------
#   c_float			float						float
#----------------------------------------------------------------------------------
#   c_double		double						float
#----------------------------------------------------------------------------------
#   c_longdouble	long double					float
#----------------------------------------------------------------------------------
#   c_char_p		char * (NUL terminated)		string or None
#----------------------------------------------------------------------------------
#   c_wchar_p		wchar_t * (NUL terminated)	unicode or None
#----------------------------------------------------------------------------------
#   c_void_p		void *	int/long or None	int/long or None
#==================================================================================

from ctypes import *

#===================================================================
#	MGC3130 CMD ID
ID_DATA_OUTPUT = 0x91
ID_FW_VERSION  = 0x83
#===================================================================

#===================================================================
MASK_GESTURE_RAW = 0x0001F0FF		# Filter mask to remove invalid data into gesture packet
MASK_TOUCH_RAW	 = 0x00007FFF		# Filter mask to remove invalid data into touch packet

MASK_FILTER_GESTURE	= 0x0000000	# To calculate exactly value of mask see below	
# B00000000000000000000000000000000				//	Set bit to "1" to mask Gesture and convert binary data into hexadecimal data
#  ||||||||||||||||||||||||||||||||
#  |||||||||||||||||||||||||||||||+------>		if "1" MASK Touch South
#  ||||||||||||||||||||||||||||||+------->		if "1" MASK Touch West
#  |||||||||||||||||||||||||||||+-------->		if "1" MASK Touch North
#  ||||||||||||||||||||||||||||+--------->		if "1" MASK Touch East
#  |||||||||||||||||||||||||||+---------->		if "1" MASK Touch Centre
#  ||||||||||||||||||||||||||+----------->		if "1" MASK Tap South
#  |||||||||||||||||||||||||+------------>		if "1" MASK Tap West
#  ||||||||||||||||||||||||+------------->		if "1" MASK Tap North
#  |||||||||||||||||||||||+-------------->		if "1" MASK Tap East
#  ||||||||||||||||||||||+--------------->		if "1" MASK Tap Centre
#  |||||||||||||||||||||+---------------->		if "1" MASK Double Tap South
#  ||||||||||||||||||||+----------------->		if "1" MASK Double Tap West
#  |||||||||||||||||||+------------------>		if "1" MASK Double Tap North
#  ||||||||||||||||||+------------------->		if "1" MASK Double Tap East
#  |||||||||||||||||+-------------------->		if "1" MASK Double Tap Centre
#  ||||||||||||||||+--------------------->		if "1" MASK Gesture West To East
#  |||||||||||||||+---------------------->		if "1" MASK Gesture East To West
#  ||||||||||||||+----------------------->		if "1" MASK Gesture South To North
#  |||||||||||||+------------------------>		if "1" MASK Gesture North To South
#  ||||||||||||+-------------------------> 	if "1" MASK Gesture Edge West To East
#  |||||||||||+-------------------------->		if "1" MASK Gesture Edge East To West
#  ||||||||||+--------------------------->		if "1" MASK Gesture Edge South To North
#  |||||||||+---------------------------->		if "1" MASK Gesture Edge North To South
#  ||||||||+----------------------------->		if "1" MASK Gesture Clock Wise
#  |||||||+------------------------------>		if "1" MASK Gesture Counter Clock Wise
#  ||||||+------------------------------->		if "1" MASK Gesture Complete Rotation
#  ++++++-------------------------------->		Free

#===================================================================
#	Use this MASK constant to create application code for gestic gesture decode 
#===================================================================
#	TOUCH/GESTURE OUTPUT MASK
GESTURE_MASK_TOUCH_SOUTH		 = 0x00000001
GESTURE_MASK_TOUCH_WEST			 = 0x00000002
GESTURE_MASK_TOUCH_NORTH		 = 0x00000004
GESTURE_MASK_TOUCH_EAST			 = 0x00000008
GESTURE_MASK_TOUCH_CENTRE		 = 0x00000010
GESTURE_MASK_TAP_SOUTH			 = 0x00000020
GESTURE_MASK_TAP_WEST			 = 0x00000040
GESTURE_MASK_TAP_NORTH			 = 0x00000080
GESTURE_MASK_TAP_EAST			 = 0x00000100
GESTURE_MASK_TAP_CENTRE			 = 0x00000200
GESTURE_MASK_DOUBLE_TAP_SOUTH	 = 0x00000400
GESTURE_MASK_DOUBLE_TAP_WEST	 = 0x00000800
GESTURE_MASK_DOUBLE_TAP_NORTH	 = 0x00001000
GESTURE_MASK_DOUBLE_TAP_EAST	 = 0x00002000
GESTURE_MASK_DOUBLE_TAP_CENTRE	 = 0x00004000
GESTURE_MASK_WEST_EAST			 = 0x00008000
GESTURE_MASK_EAST_WEST			 = 0x00010000
GESTURE_MASK_SOUTH_NORTH		 = 0x00020000
GESTURE_MASK_NORTH_SOUTH		 = 0x00040000
GESTURE_MASK_EDGE_WEST_EAST		 = 0x00080000
GESTURE_MASK_EDGE_EAST_WEST		 = 0x00100000
GESTURE_MASK_EDGE_SOUTH_NORTH	 = 0x00200000
GESTURE_MASK_EDGE_NORTH_SOUTH	 = 0x00400000
GESTURE_MASK_CLOCK_WISE		     = 0x00800000
GESTURE_MASK_COUNTER_CLOCK_WISE  = 0x01000000
GESTURE_MASK_COMPLETE_ROTATION	 = 0x02000000
#===================================================================

#===================================================================
#	GESTURE INPUT CODE MASK
NO_GESTURE				   = 0x00
GESTURE_GARBAGE			   = 0x01
GESTURE_WEST_EAST		   = 0x02
GESTURE_EAST_WEST		   = 0x03
GESTURE_SOUTH_NORTH		   = 0x04
GESTURE_NORTH_SOUTH		   = 0x05
GESTURE_CLOCK_WISE		   = 0x06
GESTURE_COUNTER_CLOCK_WISE = 0x07
#===================================================================

#===================================================================
#	Sequence for Tap gesture
#	Touch -> Tap
#
#	Sequence for Double Tap gesture
#	Touch -> Tap -> Touch -> DoubleTap -> Touch -> Tap	
#===================================================================

#===================================================================
#	AirWheel Variable
AirWheelInfo = 0x00
#===================================================================
		
#===================================================================
#	Gesture Private Structure
LastGesture = 0x00000000

class GestureInfoBit(Structure):
	_fields_ = [("GestureCode",       c_uint32, 8),	            
			    ("Reserved",          c_uint32, 4),
		   	    ("GestureType",       c_uint32, 4),
			    ("Edgeflick",         c_uint32, 1),
			    ("Reserved2",         c_uint32, 14),
			    ("GestureInProgress", c_uint32, 1)]

class GestureInfoByte(Structure):	
	_fields_ = [("Byte0", c_uint8),
				("Byte1", c_uint8),
				("Byte2", c_uint8),
				("Byte3", c_uint8)]
	
class GestureInfo(Union):
	_fields_ = [("GestureInfo32Bit", GestureInfoBit),
				("GestureInfoByte",  GestureInfoByte),
	            ("GestureInfoLong",  c_uint32),
	            ("GestInfoArray",    c_ubyte * 4)]            
#===================================================================

#===================================================================
#	Touch Private Structure 
LastTouch = 0x00000000 
 
class TouchInfoBit(Structure):
	_fields_ = [("TouchSouth",           c_uint32, 1),            
			    ("TouchWest",            c_uint32, 1),
		   	    ("TouchNorth",           c_uint32, 1),
			    ("TouchEast",            c_uint32, 1),
			    ("TouchCentre",          c_uint32, 1),
			    ("TapSouth",             c_uint32, 1),
			    ("TapWest",              c_uint32, 1),
			    ("TapNorth",             c_uint32, 1),
			    ("TapEast",              c_uint32, 1),	            
			    ("TapCentre",            c_uint32, 1),
			    ("DoubleTapSouth",       c_uint32, 1),
			    ("DoubleTapWest",        c_uint32, 1),
			    ("DoubleTapNorth",       c_uint32, 1),
			    ("DoubleTapEast",        c_uint32, 1),
			    ("DoubleTapCentre",      c_uint32, 1),	            
			    ("Free",                 c_uint32, 17)]

class TouchInfoByte(Structure):	
	_fields_ = [("Byte0", c_uint8),
				("Byte1", c_uint8),
				("Byte2", c_uint8),
				("Byte3", c_uint8)]
				
class TouchInfo(Union):
	_fields_ = [("TouchInfo32Bit", TouchInfoBit),
				("TouchInfoByte",  TouchInfoByte),
	            ("TouchInfoLong",  c_uint32),
	            ("TouchInfoArray", c_ubyte * 4)]
#===================================================================
 
#===================================================================
#	Gesture Public Structure
class GestureBit(Structure):
	_fields_ = [("TouchSouth",           c_uint32, 1),	            
			    ("TouchWest",            c_uint32, 1),
		   	    ("TouchNorth",           c_uint32, 1),
			    ("TouchEast",            c_uint32, 1),
			    ("TouchCentre",          c_uint32, 1),
			    ("TapSouth",             c_uint32, 1),
			    ("TapWest",              c_uint32, 1),
			    ("TapNorth",             c_uint32, 1),
			    ("TapEast",              c_uint32, 1),	            
			    ("TapCentre",            c_uint32, 1),
			    ("DoubleTapSouth",       c_uint32, 1),
			    ("DoubleTapWest",        c_uint32, 1),
			    ("DoubleTapNorth",       c_uint32, 1),
			    ("DoubleTapEast",        c_uint32, 1),
			    ("DoubleTapCentre",      c_uint32, 1),
			    ("GestWestEast",         c_uint32, 1),
			    ("GestEastWest",         c_uint32, 1),	            
			    ("GestSouthNorth",       c_uint32, 1),
			    ("GestNorthSouth",       c_uint32, 1),
			    ("EdgeGestWestEast",     c_uint32, 1),
			    ("EdgeGestEastWest",     c_uint32, 1),
			    ("EdgeGestSouthNorth",   c_uint32, 1),
			    ("EdgeGestNorthSouth",   c_uint32, 1),
			    ("GestClockWise",        c_uint32, 1),
			    ("GestCounterClockWise", c_uint32, 1),	            
			    ("Free",                 c_uint32, 7)]

class GestureByte(Structure):	
	_fields_ = [("Byte0", c_uint8),
				("Byte1", c_uint8),
				("Byte2", c_uint8),
				("Byte3", c_uint8)]
				
class Gesture(Union):
	_fields_ = [("Gesture32Bit", GestureBit),
				("GestureByte",  GestureByte),
	            ("GestureLong",  c_uint32),
	            ("GestArray",    c_ubyte * 4)]
		 
#===================================================================

#===================================================================
#	X, Y, Z coordinates Public Class
Last_X = 0x0000 
Last_Y = 0x0000 
Last_Z = 0x0000 

class x_SplitByte(Structure):
	_fields_ = [("Byte0", c_ubyte),
				("Byte1", c_ubyte)]

class y_SplitByte(Structure):
	_fields_ = [("Byte0", c_ubyte),
				("Byte1", c_ubyte)]

class z_SplitByte(Structure):
	_fields_ = [("Byte0", c_ubyte),
				("Byte1", c_ubyte)]
				
class xyz_Coordinates(Structure):
	_fields_ = [("x", x_SplitByte),
				("y", y_SplitByte),
				("z", z_SplitByte)]

class Coordinates(Union):
    _fields_ = [("xyz",      xyz_Coordinates),
                ("xInt",     c_uint16),
                ("yInt",     c_uint16),
                ("zInt",     c_uint16),
                ("xyzArray", c_ubyte * 6)]
#=================================================================== 
