#!/usr/bin/env python
# -*- coding: utf-8 -*-
#
#  untitled.py
#  
#  Copyright 2015 root <root@RaspTest>
#  
#  This program is free software; you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation; either version 2 of the License, or
#  (at your option) any later version.
#  
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#  
#  You should have received a copy of the GNU General Public License
#  along with this program; if not, write to the Free Software
#  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
#  MA 02110-1301, USA.
#  
#====================================================================================================

import sys
sys.path.insert(0, "/usr/local/lib/python2.7/dist-packages/MGC3130")
import RPi.GPIO as GPIO
import MGC3130
import threading
import time

from pykeyboard import PyKeyboard

KeyPressed = PyKeyboard()

#==========================================================
#	Define pin

Addr_MGC3130       = 0x42	#  MGC3130 hardware address

MGC3130_SDA_LINE   = 2
MGC3130_SCL_LINE   = 3
MGC3130_TS_LINE    = 18
MGC3130_RESET_LINE = 17

JUMPER_J1 = 24
PULSE_P1  = 23
PULSE_P2  = 22
PULSE_P3  = 27

LED_DIAGNOSTIC = 25
#==========================================================

#==========================================================
#	Global Var
ActionInProgress = False

MAXZOOMIN = 30
ZoomStep  = 0
#==========================================================

GPIO.setmode(GPIO.BCM)  # set board mode to Broadcom

TEST_PIN = 21
GPIO.setup(TEST_PIN, GPIO.OUT, initial=GPIO.LOW)

GPIO.setup(JUMPER_J1, GPIO.IN, pull_up_down=GPIO.PUD_UP)		# Jumper
GPIO.setup(PULSE_P1, GPIO.IN, pull_up_down=GPIO.PUD_UP)			# Pulsante P1
GPIO.setup(PULSE_P2, GPIO.IN, pull_up_down=GPIO.PUD_UP)			# Pulsante P2
GPIO.setup(PULSE_P3, GPIO.IN, pull_up_down=GPIO.PUD_UP)			# Pulsante P3
GPIO.setup(LED_DIAGNOSTIC, GPIO.OUT, initial=GPIO.HIGH) 		# Led

GPIO.add_event_detect(PULSE_P1, GPIO.FALLING, bouncetime=100)
GPIO.add_event_detect(PULSE_P2, GPIO.FALLING, bouncetime=100)
GPIO.add_event_detect(PULSE_P3, GPIO.FALLING, bouncetime=100)

#===================================================================
#	SLOW BASE TIME 100mSec (Thread)      
def SlowBaseTime():
	global BaseTimeTimer
	
	GPIO.output(LED_DIAGNOSTIC, GPIO.HIGH)
	BaseTimeTimer.cancel()
#===================================================================

BaseTimeTimer = threading.Timer(1, SlowBaseTime)	
	
def main():
	global ZoomStep
	global MAXZOOMIN
	global ActionInProgress

	MGC3130.MGC3130_SetAdd(Addr_MGC3130)
	MGC3130.MGC3130_Begin(GPIO, MGC3130_TS_LINE, MGC3130_RESET_LINE)
	
	while True:
		#====================================
		if (MGC3130.MGC3130_GetTsLineStatus(GPIO, MGC3130_TS_LINE) == True):
			MGC3130.MGC3130_GetEvent()
			MGC3130.MGC3130_DecodeGesture()
			MGC3130.MGC3130_ReleaseTsLine(GPIO, MGC3130_TS_LINE)	
		#====================================
				
		#====================================
		if GPIO.event_detected(PULSE_P1):
			print "Activate gesture recognitions"
			ActionInProgress = True

		if GPIO.event_detected(PULSE_P2):
			print "Deactivate gesture recognitions"
			ActionInProgress = False			
									
		if GPIO.event_detected(PULSE_P3):
			print "P3 pulse intercepted"
			GPIO.cleanup()
			return 0
		#====================================

		#====================================
		if (ActionInProgress == True):
			if (MGC3130._GestOutput.Gesture64Bit.GestWestEast == 1):
				MGC3130._GestOutput.Gesture64Bit.GestWestEast = 0
				GPIO.output(LED_DIAGNOSTIC, GPIO.LOW)
				BaseTimeTimer = threading.Timer(1, SlowBaseTime)
				BaseTimeTimer.start()
				KeyPressed.tap_key(KeyPressed.right_key)
		
			if (MGC3130._GestOutput.Gesture64Bit.GestEastWest == 1):
				MGC3130._GestOutput.Gesture64Bit.GestEastWest = 0
				GPIO.output(LED_DIAGNOSTIC, GPIO.LOW)
				BaseTimeTimer = threading.Timer(1, SlowBaseTime)
				BaseTimeTimer.start()
				KeyPressed.tap_key(KeyPressed.left_key)
			
			if (MGC3130._GestOutput.Gesture64Bit.GestSouthNorth == 1):
				MGC3130._GestOutput.Gesture64Bit.GestSouthNorth = 0
				GPIO.output(LED_DIAGNOSTIC, GPIO.LOW)
				BaseTimeTimer = threading.Timer(1, SlowBaseTime)
				BaseTimeTimer.start()
				KeyPressed.tap_key(KeyPressed.up_key)
		
			if (MGC3130._GestOutput.Gesture64Bit.GestNorthSouth == 1):
				MGC3130._GestOutput.Gesture64Bit.GestNorthSouth = 0
				GPIO.output(LED_DIAGNOSTIC, GPIO.LOW)
				BaseTimeTimer = threading.Timer(1, SlowBaseTime)
				BaseTimeTimer.start()
				KeyPressed.tap_key(KeyPressed.down_key)

			if (MGC3130._GestOutput.Gesture64Bit.TouchCentre == 1):
				MGC3130._GestOutput.Gesture64Bit.TouchCentre = 0
				GPIO.output(LED_DIAGNOSTIC, GPIO.LOW)
				BaseTimeTimer = threading.Timer(1, SlowBaseTime)
				BaseTimeTimer.start()
				KeyPressed.tap_key(KeyPressed.function_keys[5])
								
			if (MGC3130._GestOutput.Gesture64Bit.TouchSouth == 1):
				MGC3130._GestOutput.Gesture64Bit.TouchSouth = 0
				GPIO.output(LED_DIAGNOSTIC, GPIO.LOW)
				BaseTimeTimer = threading.Timer(1, SlowBaseTime)
				BaseTimeTimer.start()
				if (ZoomStep > 0):
					ZoomStep -= 1
					KeyPressed.tap_key('minus')
				else:
					ZoomStep = 0
			
			if (MGC3130._GestOutput.Gesture64Bit.TouchNorth == 1):
				MGC3130._GestOutput.Gesture64Bit.TouchNorth = 0
				GPIO.output(LED_DIAGNOSTIC, GPIO.LOW)
				BaseTimeTimer = threading.Timer(1, SlowBaseTime)
				BaseTimeTimer.start()
				if (ZoomStep < MAXZOOMIN):
					ZoomStep += 1
					KeyPressed.tap_key('plus')
				else:
					ZoomStep = MAXZOOMIN

			if (MGC3130._GestOutput.Gesture64Bit.GestClockWise == 1):
				MGC3130._GestOutput.Gesture64Bit.GestClockWise = 0
				GPIO.output(LED_DIAGNOSTIC, GPIO.LOW)
				BaseTimeTimer = threading.Timer(1, SlowBaseTime)
				BaseTimeTimer.start()
				KeyPressed.press_key(KeyPressed.control_key)
				KeyPressed.tap_key('R')
				KeyPressed.release_key(KeyPressed.control_key)

			if (MGC3130._GestOutput.Gesture64Bit.GestCounterClockWise == 1):
				MGC3130._GestOutput.Gesture64Bit.GestCounterClockWise = 0
				GPIO.output(LED_DIAGNOSTIC, GPIO.LOW)
				BaseTimeTimer = threading.Timer(1, SlowBaseTime)
				BaseTimeTimer.start()
				KeyPressed.press_key(KeyPressed.shift_key)
				KeyPressed.press_key(KeyPressed.control_key)
				KeyPressed.tap_key('R')
				KeyPressed.release_key(KeyPressed.control_key)
				KeyPressed.release_key(KeyPressed.shift_key)
		#====================================
					
	GPIO.cleanup()	
	return 0

if __name__ == '__main__':
	main()

#====================================================================================================
#	TTY Function Keys
#===============================
#	backspace_key   	-> 'BackSpace'
#	tab_key				-> 'Tab'
#   linefeed_key    	-> 'Linefeed'
#   clear_key       	-> 'Clear'
#   return_key      	-> 'Return'
#   enter_key       	-> 'Enter'
#   pause_key       	-> 'Pause'
#   scroll_lock_key 	-> 'Scroll_Lock'
#   sys_req_key     	-> 'Sys_Req'
#   escape_key     	 	-> 'Escape'
#   delete_key 	    	-> 'Delete'
#===============================
#   #Modifier Keys
#===============================
#	shift_l_key 		-> 'Shift_L'
#   shift_r_key 		-> 'Shift_R'
#   shift_key 			-> Default Shift is left Shift
#   alt_l_key 			-> 'Alt_L'
#   alt_r_key 			-> 'Alt_R'
#   alt_key 			-> Default Alt is left Alt
#   control_l_key 		-> 'Control_L'
#   control_r_key 		-> 'Control_R'
#   control_key	 		-> Default Ctrl is left Ctrl
#   caps_lock_key 		-> 'Caps_Lock'
#   capital_key 		-> Some may know it as Capital
#   shift_lock_key 		-> 'Shift_Lock'
#   meta_l_key 			-> 'Meta_L'
#   meta_r_key 			-> 'Meta_R'
#   super_l_key 		-> 'Super_L'
#   windows_l_key 		-> Cross-support; also it's printed there
#   super_r_key 		-> 'Super_R'
#   windows_r_key 		-> Cross-support; also it's printed there
#   hyper_l_key 		-> 'Hyper_L'
#   hyper_r_key 		-> 'Hyper_R'
#===============================
#	Cursor Control and Motion
#===============================
#   home_key 			-> 'Home'
#   up_key 				-> 'Up'
#   down_key 			-> 'Down'
#   left_key 			-> 'Left'
#   right_key	 		-> 'Right'
#   end_key 			-> 'End'
#   begin_key 			-> 'Begin'
#   page_up_key 		-> 'Page_Up'
#   page_down_key 		-> 'Page_Down'
#   prior_key 			-> 'Prior'
#   next_key 			-> 'Next'
#===============================
#	Misc Functions
#===============================
#	select_key 			-> 'Select'
#   print_key 			-> 'Print'
#   print_screen_key	-> Seems to be the same thing
#   snapshot_key 		-> Another name for printscreen
#   execute_key 		-> 'Execute'
#   insert_key 			-> 'Insert'
#   undo_key 			-> 'Undo'
#   redo_key 			-> 'Redo'
#   menu_key 			-> Menu'
#   apps_key 			-> Windows...
#   find_key 			-> 'Find'
#   cancel_key 			-> 'Cancel'
#   help_key 			-> 'Help'
#   break_key 			-> 'Break'
#   mode_switch_key 	-> 'Mode_switch'
#   script_switch_key 	-> 'script_switch'
#   num_lock_key 		-> 'Num_Lock'
#====================================================================================================
