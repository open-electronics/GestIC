//======================================================================
//  Sets digital inputs
void SetOutputPin(void) {
  pinMode(Led, OUTPUT);       // Set pin to Output
  digitalWrite(Led, HIGH);    // Led OFF
}
//======================================================================

void Led_Idle(void) {
  if (TimeOutLed == 0) {
    digitalWrite(Led, HIGH);  // Led OFF
  }
}

//======================================================================
void Led_P1Pressed(void) {
  digitalWrite(Led, LOW);    // Led ON
  TimeOutLed = T_250MSEC;
  Led_Management = Led_Idle;
}

void Led_P2Pressed(void) {
  digitalWrite(Led, LOW);    // Led ON
  TimeOutLed = T_500MSEC;
  Led_Management = Led_Idle;
}

void Led_P3Pressed(void) {
  digitalWrite(Led, LOW);    // Led ON
  TimeOutLed = T_750MSEC;
  Led_Management = Led_Idle;
}

void Led_P1P2Pressed(void) {
  digitalWrite(Led, LOW);    // Led ON
  TimeOutLed = T_1SEC;
  Led_Management = Led_Idle;
}

void Led_P2P3Pressed(void) {
  digitalWrite(Led, LOW);    // Led ON
  TimeOutLed = T_1SEC250;
  Led_Management = Led_Idle;
}

void Led_P1P3Pressed(void) {
  digitalWrite(Led, LOW);    // Led ON
  TimeOutLed = T_1SEC500;
  Led_Management = Led_Idle;
}

void Led_P1P2P3Pressed(void) {
  digitalWrite(Led, LOW);    // Led ON
  TimeOutLed = T_1SEC750;
  Led_Management = Led_Idle;
}
//======================================================================

//======================================================================
uint8_t SumTouch(uint16_t PointerArray, uint8_t ArrayLenght)
{
  uint8_t Counter;
  uint8_t Data;
  
  Counter = 0;
  Data = 0;
  do {
    Data += *(uint8_t *)PointerArray++;
  } while (Counter++ < ArrayLenght);
  return(Data);
}
//======================================================================

//======================================================================
uint8_t EraseTouch(uint16_t PointerArray, uint8_t ArrayLenght)
{
  uint8_t Counter;
  
  Counter = 0;
  do {
    *(uint8_t *)PointerArray++ = 0;
  } while (Counter++ < ArrayLenght);
  return(0);
}
//======================================================================

//======================================================================
//  Led State Machine associated at North Elettrode for touch recognition (Tap, Double Tap and Touch)
void LedNorth_Idle(void)
{
  if (mgc3130.GestureOutput.Bit.DoubleTapNorth != 0) {    // Double Tap North
    mgc3130.GestureOutput.Bit.DoubleTapNorth = 0;
    TimeOutNorthGest = T_200MSEC;
    NorthTouch[NorthCounter++] = DOUBLE_TAP_CODE;
  }
  if (mgc3130.GestureOutput.Bit.TapNorth != 0) {          // Tap North
    mgc3130.GestureOutput.Bit.TapNorth = 0;
    TimeOutNorthGest = T_200MSEC;
    NorthTouch[NorthCounter++] = TAP_CODE;
  }
  if (mgc3130.GestureOutput.Bit.TouchNorth != 0) {        // Touch North
    mgc3130.GestureOutput.Bit.TouchNorth = 0;
    TimeOutNorthGest = T_200MSEC;
    NorthTouch[NorthCounter++] = TOUCH_CODE;
  }
  
  if (TimeOutNorthGest == 0) {
    if (NorthCounter > 0) {
      NorthCounter = 0;
      Led_North = LedNorth_Gesture;  
    }
  }
}

void LedNorth_Gesture(void)
{
  NorthData = SumTouch((uint16_t )&NorthTouch[0], (sizeof(NorthTouch) - 1));
  if (NorthData == GESTURE_DOUBLE_TAP_NORTH) {
    digitalWrite(Led, LOW);  // Led ON
    TimeOutLed = T_1SEC;
    Gesture_Readed |= GESTURE_MASK_DOUBLE_TAP_NORTH; 
    BiMonoGestureOutput(GESTURE_MASK_DOUBLE_TAP_NORTH);
  } else if (NorthData == GESTURE_TAP_NORTH) {
    digitalWrite(Led, LOW);  // Led ON
    TimeOutLed = T_500MSEC;
    Gesture_Readed |= GESTURE_MASK_TAP_NORTH;
    BiMonoGestureOutput(GESTURE_MASK_TAP_NORTH);
  } else if (NorthData == GESTURE_TOUCH_NORTH) {
    digitalWrite(Led, LOW);  // Led ON
    TimeOutLed = T_250MSEC;
    Gesture_Readed |= GESTURE_MASK_TOUCH_NORTH; 
  }
  NorthCounter = EraseTouch((uint16_t )&NorthTouch[0], (sizeof(NorthTouch) - 1));
  Led_North = LedNorth_Idle;
}
//======================================================================

//======================================================================
//  Led State Machine associated at South Elettrode for touch recognition (Tap, Double Tap and Touch)
void LedSouth_Idle(void)
{
  if (mgc3130.GestureOutput.Bit.DoubleTapSouth != 0) {    // Double Tap South 
    mgc3130.GestureOutput.Bit.DoubleTapSouth = 0;
    TimeOutSouthGest = T_200MSEC;  //T_20MSEC;
    SouthTouch[SouthCounter++] = DOUBLE_TAP_CODE; 
  }
  if (mgc3130.GestureOutput.Bit.TapSouth != 0) {          // Tap South  
    mgc3130.GestureOutput.Bit.TapSouth = 0;
    TimeOutSouthGest = T_200MSEC;  //T_20MSEC;
    SouthTouch[SouthCounter++] = TAP_CODE; 
  }
  if (mgc3130.GestureOutput.Bit.TouchSouth != 0) {        // Touch South
    mgc3130.GestureOutput.Bit.TouchSouth = 0;
    TimeOutSouthGest = T_200MSEC;  //T_20MSEC;
    SouthTouch[SouthCounter++] = TOUCH_CODE;
  }
  
  if (TimeOutSouthGest == 0) {
    if (SouthCounter > 0) {
      SouthCounter = 0;
      Led_South = LedSouth_Gesture;  
    }
  }
}

void LedSouth_Gesture(void)
{
  SouthData = SumTouch((uint16_t )&SouthTouch[0], (sizeof(SouthTouch) - 1));
  if (SouthData == GESTURE_DOUBLE_TAP_SOUTH) {
    digitalWrite(Led, LOW);  // Led ON
    TimeOutLed = T_1SEC;
    Gesture_Readed |= GESTURE_MASK_DOUBLE_TAP_SOUTH; 
    BiMonoGestureOutput(GESTURE_MASK_DOUBLE_TAP_SOUTH);
  } else if (SouthData == GESTURE_TAP_SOUTH) {
    digitalWrite(Led, LOW);  // Led ON
    TimeOutLed = T_500MSEC;
    Gesture_Readed |= GESTURE_MASK_TAP_SOUTH;
    BiMonoGestureOutput(GESTURE_MASK_TAP_SOUTH);
  } else if (SouthData == GESTURE_TOUCH_SOUTH) {
    digitalWrite(Led, LOW);  // Led ON
    TimeOutLed = T_250MSEC;
    Gesture_Readed |= GESTURE_MASK_TOUCH_SOUTH; 
  }
  SouthCounter = EraseTouch((uint16_t )&SouthTouch[0], (sizeof(SouthTouch) - 1));
  Led_South = LedSouth_Idle;
}
//======================================================================

//======================================================================
//  Led State Machine associated at West Elettrode for touch recognition (Tap, Double Tap and Touch)
void LedWest_Idle(void)
{
  if (mgc3130.GestureOutput.Bit.DoubleTapWest != 0) {    // Double Tap West   
    mgc3130.GestureOutput.Bit.DoubleTapWest = 0;
    TimeOutWestGest = T_200MSEC;  //T_20MSEC;
    WestTouch[WestCounter++] = DOUBLE_TAP_CODE; 
  }
  if (mgc3130.GestureOutput.Bit.TapWest != 0) {          // Tap West   
    mgc3130.GestureOutput.Bit.TapWest = 0;
    TimeOutWestGest = T_200MSEC;  //T_20MSEC;
    WestTouch[WestCounter++] = TAP_CODE; 
  }
  if (mgc3130.GestureOutput.Bit.TouchWest != 0) {        // Touch West
    mgc3130.GestureOutput.Bit.TouchWest = 0;
    TimeOutWestGest = T_200MSEC;  //T_20MSEC;
    WestTouch[WestCounter++] = TOUCH_CODE;
  }
  
  if (TimeOutWestGest == 0) {
    if (WestCounter > 0) {
      WestCounter = 0;
      Led_West = LedWest_Gesture;  
    }
  }
}

void LedWest_Gesture(void)
{
  WestData = SumTouch((uint16_t )&WestTouch[0], (sizeof(WestTouch) - 1));
  if (WestData == GESTURE_DOUBLE_TAP_WEST) {
    digitalWrite(Led, LOW);  // Led ON
    TimeOutLed = T_1SEC;
    Gesture_Readed |= GESTURE_MASK_DOUBLE_TAP_WEST; 
    BiMonoGestureOutput(GESTURE_MASK_DOUBLE_TAP_WEST);
  } else if (WestData == GESTURE_TAP_WEST) {
    digitalWrite(Led, LOW);  // Led ON
    TimeOutLed = T_500MSEC;
    Gesture_Readed |= GESTURE_MASK_TAP_WEST;
    BiMonoGestureOutput(GESTURE_MASK_TAP_WEST);
  } else if (WestData == GESTURE_TOUCH_WEST) {
    digitalWrite(Led, LOW);  // Led ON
    TimeOutLed = T_250MSEC;
    Gesture_Readed |= GESTURE_MASK_TOUCH_WEST; 
  }
  WestCounter = EraseTouch((uint16_t )&WestTouch[0], (sizeof(WestTouch) - 1));
  Led_West = LedWest_Idle;
}
//======================================================================

//======================================================================
//  Led State Machine associated at East Elettrode for touch recognition (Tap, Double Tap and Touch)
void LedEast_Idle(void)
{
  if (mgc3130.GestureOutput.Bit.DoubleTapEast != 0) {    // Double Tap East    
    mgc3130.GestureOutput.Bit.DoubleTapEast = 0;
    TimeOutEastGest = T_200MSEC;  //T_20MSEC;
    EastTouch[EastCounter++] = DOUBLE_TAP_CODE; 
  }
  if (mgc3130.GestureOutput.Bit.TapEast != 0) {          // Tap East   
    mgc3130.GestureOutput.Bit.TapEast = 0;
    TimeOutEastGest = T_200MSEC;  //T_20MSEC;
    EastTouch[EastCounter++] = TAP_CODE; 
  }
  if (mgc3130.GestureOutput.Bit.TouchEast != 0) {        // Touch East
    mgc3130.GestureOutput.Bit.TouchEast = 0;
    TimeOutEastGest = T_200MSEC;  //T_20MSEC;
    EastTouch[EastCounter++] = TOUCH_CODE;
  }
  
  if (TimeOutEastGest == 0) {
    if (EastCounter > 0) {
      EastCounter = 0;
      Led_East = LedEast_Gesture;  
    }
  }
}

void LedEast_Gesture(void)
{
  EastData = SumTouch((uint16_t )&EastTouch[0], (sizeof(EastTouch) - 1));
  if (EastData == GESTURE_DOUBLE_TAP_EAST) {
    digitalWrite(Led, LOW);  // Led ON
    TimeOutLed = T_1SEC;
    Gesture_Readed |= GESTURE_MASK_DOUBLE_TAP_EAST; 
    BiMonoGestureOutput(GESTURE_MASK_DOUBLE_TAP_EAST);
  } else if (EastData == GESTURE_TAP_EAST) {
    digitalWrite(Led, LOW);  // Led ON
    TimeOutLed = T_500MSEC;
    Gesture_Readed |= GESTURE_MASK_TAP_EAST;
    BiMonoGestureOutput(GESTURE_MASK_TAP_EAST);
  } else if (EastData == GESTURE_TOUCH_EAST) {
    digitalWrite(Led, LOW);  // Led ON
    TimeOutLed = T_250MSEC;
    Gesture_Readed |= GESTURE_MASK_TOUCH_EAST; 
  }
  EastCounter = EraseTouch((uint16_t )&EastTouch[0], (sizeof(EastTouch) - 1));
  Led_East = LedEast_Idle;
}
//======================================================================

//======================================================================
//  Led State Machine associated at Centre Elettrode for touch recognition (Tap, Double Tap and Touch)
void LedCentre_Idle(void)
{
  if (mgc3130.GestureOutput.Bit.DoubleTapCentre != 0) {    // Double Tap Centre   
    mgc3130.GestureOutput.Bit.DoubleTapCentre = 0;
    TimeOutCentreGest = T_200MSEC;  //T_20MSEC;
    CentreTouch[CentreCounter++] = DOUBLE_TAP_CODE; 
  }
  if (mgc3130.GestureOutput.Bit.TapCentre != 0) {          // Tap Centre  
    mgc3130.GestureOutput.Bit.TapCentre = 0;
    TimeOutCentreGest = T_200MSEC;  //T_20MSEC;
    CentreTouch[CentreCounter++] = TAP_CODE; 
  }
  if (mgc3130.GestureOutput.Bit.TouchCentre != 0) {        // Touch Centre
    mgc3130.GestureOutput.Bit.TouchCentre = 0;
    TimeOutCentreGest = T_200MSEC;  //T_20MSEC;
    CentreTouch[CentreCounter++] = TOUCH_CODE;
  }
  
  if (TimeOutCentreGest == 0) {
    if (CentreCounter > 0) {
      CentreCounter = 0;
      Led_Centre = LedCentre_Gesture;  
    }
  }
}

void LedCentre_Gesture(void)
{
  CentreData = SumTouch((uint16_t )&CentreTouch[0], (sizeof(CentreTouch) - 1));
  if (CentreData == GESTURE_DOUBLE_TAP_CENTRE) {
    digitalWrite(Led, LOW);  // Led ON
    TimeOutLed = T_1SEC;
    Gesture_Readed |= GESTURE_MASK_DOUBLE_TAP_CENTRE; 
    BiMonoGestureOutput(GESTURE_MASK_DOUBLE_TAP_CENTRE);
  } else if (CentreData == GESTURE_TAP_CENTRE) {
    digitalWrite(Led, LOW);  // Led ON
    TimeOutLed = T_500MSEC;
    Gesture_Readed |= GESTURE_MASK_TAP_CENTRE;
    BiMonoGestureOutput(GESTURE_MASK_TAP_CENTRE);
  } else if (CentreData == GESTURE_TOUCH_CENTRE) {
    digitalWrite(Led, LOW);  // Led ON
    TimeOutLed = T_250MSEC;
    Gesture_Readed |= GESTURE_MASK_TOUCH_CENTRE; 
  }
  CentreCounter = EraseTouch((uint16_t )&CentreTouch[0], (sizeof(CentreTouch) - 1));
  Led_Centre = LedCentre_Idle;
}
//======================================================================

//======================================================================
//  Led State Machine associated at gesture West to Easr
void LedWestEast_Idle(void)
{
  if (mgc3130.GestureOutput.Bit.EdgeGestWestEast != 0) {
    digitalWrite(Led, LOW);  // Led ON
    TimeOutLed = T_100MSEC;	
    Led_WestEast = LedWestEastEdge_Gesture;
    Gesture_Readed |= GESTURE_MASK_EDGE_WEST_EAST;
    return;
  }
  if (mgc3130.GestureOutput.Bit.GestWestEast != 0) {
    digitalWrite(Led, LOW);  // Led ON
    TimeOutLed = T_1SEC;
    Led_WestEast = LedWestEast_Gesture;
    Gesture_Readed |= GESTURE_MASK_WEST_EAST;
    BiMonoGestureOutput(GESTURE_MASK_WEST_EAST);
  }
}

void LedWestEast_Gesture(void)
{
  if (mgc3130.GestureOutput.Bit.GestWestEast != 1) {
    Gesture_Readed &= ~GESTURE_MASK_WEST_EAST;
    Led_WestEast = LedWestEast_Idle;
  }  
}  

void LedWestEastEdge_Gesture(void)
{
  if (mgc3130.GestureOutput.Bit.EdgeGestWestEast != 1) {
    Gesture_Readed &= ~GESTURE_MASK_EDGE_WEST_EAST;
    Led_WestEast = LedWestEast_Idle;
  }
}
//======================================================================

//======================================================================
//  Led State Machine associated at gesture Easr to West
void LedEastWest_Idle(void)
{
  if (mgc3130.GestureOutput.Bit.EdgeGestEastWest != 0) {
    digitalWrite(Led, LOW);  // Led ON
    TimeOutLed = T_100MSEC;	
    Led_EastWest = LedEastWestEdge_Gesture;
    Gesture_Readed |= GESTURE_MASK_EDGE_EAST_WEST;
    return;
  }
  if (mgc3130.GestureOutput.Bit.GestEastWest != 0) {
    digitalWrite(Led, LOW);  // Led ON
    TimeOutLed = T_1SEC;
    Led_EastWest = LedEastWest_Gesture;
    Gesture_Readed |= GESTURE_MASK_EAST_WEST;
    BiMonoGestureOutput(GESTURE_MASK_EAST_WEST);
  }
}

void LedEastWest_Gesture(void)
{
  if (mgc3130.GestureOutput.Bit.GestEastWest != 1) {
    Gesture_Readed &= ~GESTURE_MASK_EAST_WEST;
    Led_EastWest = LedEastWest_Idle;
  }  
}  

void LedEastWestEdge_Gesture(void)
{
  if (mgc3130.GestureOutput.Bit.EdgeGestEastWest != 1) {
    Gesture_Readed &= ~GESTURE_MASK_EDGE_EAST_WEST;
    Led_EastWest = LedEastWest_Idle;
  }
}
//======================================================================

//======================================================================
//  Led State Machine associated at gesture North to South
void LedNorthSouth_Idle(void)
{
  if (mgc3130.GestureOutput.Bit.EdgeGestNorthSouth != 0) {
    digitalWrite(Led, LOW);  // Led ON
    TimeOutLed = T_100MSEC;	
    Led_NorthSouth = LedNorthSouthEdge_Gesture;
    Gesture_Readed |= GESTURE_MASK_EDGE_NORTH_SOUTH;
    return;
  }
  if (mgc3130.GestureOutput.Bit.GestNorthSouth != 0) {
    digitalWrite(Led, LOW);  // Led ON
    TimeOutLed = T_1SEC;
    Led_NorthSouth = LedNorthSouth_Gesture;
    Gesture_Readed |= GESTURE_MASK_NORTH_SOUTH;
    BiMonoGestureOutput(GESTURE_MASK_NORTH_SOUTH);
  }
}

void LedNorthSouth_Gesture(void)
{
  if (mgc3130.GestureOutput.Bit.GestNorthSouth != 1) {
    Gesture_Readed &= ~GESTURE_MASK_NORTH_SOUTH;
    Led_NorthSouth = LedNorthSouth_Idle;
  }  
}  

void LedNorthSouthEdge_Gesture(void)
{
  if (mgc3130.GestureOutput.Bit.EdgeGestNorthSouth != 1) {
    Gesture_Readed &= ~GESTURE_MASK_EDGE_NORTH_SOUTH;
    Led_NorthSouth = LedNorthSouth_Idle;
  }
}
//======================================================================

//======================================================================
//  Led State Machine associated at gesture South to North
void LedSouthNorth_Idle(void)
{
  if (mgc3130.GestureOutput.Bit.EdgeGestSouthNorth != 0) {
    digitalWrite(Led, LOW);  // Led ON
    TimeOutLed = T_100MSEC;	
    Led_SouthNorth = LedSouthNorthEdge_Gesture;
    Gesture_Readed |= GESTURE_MASK_EDGE_SOUTH_NORTH;
    return;
  }
  if (mgc3130.GestureOutput.Bit.GestSouthNorth != 0) {
    digitalWrite(Led, LOW);  // Led ON
    TimeOutLed = T_1SEC;
    Led_SouthNorth = LedSouthNorth_Gesture;
    Gesture_Readed |= GESTURE_MASK_SOUTH_NORTH;
    BiMonoGestureOutput(GESTURE_MASK_SOUTH_NORTH);
  }
}

void LedSouthNorth_Gesture(void)
{
  if (mgc3130.GestureOutput.Bit.GestSouthNorth != 1) {
    Gesture_Readed &= ~GESTURE_MASK_SOUTH_NORTH;
    Led_SouthNorth = LedSouthNorth_Idle;
  }  
}  

void LedSouthNorthEdge_Gesture(void)
{
  if (mgc3130.GestureOutput.Bit.EdgeGestSouthNorth != 1) {
    Gesture_Readed &= ~GESTURE_MASK_EDGE_SOUTH_NORTH;
    Led_SouthNorth = LedSouthNorth_Idle;
  }
}
//======================================================================

//======================================================================
void LedClockWise_Idle(void)
{
  if (mgc3130.GestureOutput.Bit.GestClockWise != 0) {
    digitalWrite(Led, LOW);  // Led ON
    TimeOutLed = T_100MSEC;	
    Led_ClockWise = LedClockWise_Gesture;
    Gesture_Readed |= GESTURE_MASK_CLOCK_WISE;
    BiMonoGestureOutput(GESTURE_MASK_CLOCK_WISE);
    return;
  }
}

void LedClockWise_Gesture(void)
{
  if (mgc3130.GestureOutput.Bit.GestClockWise != 1) {
    Gesture_Readed &= ~GESTURE_MASK_CLOCK_WISE;
    Led_ClockWise = LedClockWise_Idle; 
  }  
}
//======================================================================

//======================================================================
void LedCounterClockWise_Idle(void)
{
  if (mgc3130.GestureOutput.Bit.GestCounterClockWise != 0) {
    digitalWrite(Led, LOW);  // Led ON
    TimeOutLed = T_100MSEC;	
    Led_CounterClockWise = LedCounterClockWise_Gesture;
    Gesture_Readed |= GESTURE_MASK_COUNTER_CLOCK_WISE;
    BiMonoGestureOutput(GESTURE_MASK_COUNTER_CLOCK_WISE);
    return;
  }
}

void LedCounterClockWise_Gesture(void)
{
  if (mgc3130.GestureOutput.Bit.GestCounterClockWise != 1) {
    Gesture_Readed &= ~GESTURE_MASK_COUNTER_CLOCK_WISE;
    Led_CounterClockWise = LedCounterClockWise_Idle; 
  }  
}
//======================================================================

//======================================================================
void BiMonoGestureOutput(uint32_t Mask)
{
  uint32_t MaskGesture;
  uint32_t TempGestureReaded;

  TempGestureReaded = (Gesture_Readed & Mask);
  MaskGesture = 0x00000001;
  do {
    switch (TempGestureReaded & MaskGesture)
    {
      case GESTURE_MASK_TAP_SOUTH:
        if (DigInputStatus.In.Jumper1 != 0) {
	  //	BiStable
	  if (TimeOutTapBistableSouth == 0) {
            mcp23017.ToggleSingleBit(MCP23017_BNK0_OLATA, 0, 0x00);
	    TimeOutTapBistableSouth = T_500MSEC;	
	  }
        } else {
          // MonoStable
	  mcp23017.SetSingleBit(MCP23017_BNK0_OLATA, 0, 0x00);
	  TimeOutTapMonostableSouth = MonoStableTime.ui;
	  MonoStableTapSouth = MonoStableTapSouth_Wait;
	}
        break;
      case GESTURE_MASK_TAP_WEST:
        if (DigInputStatus.In.Jumper1 != 0) {
          //	BiStable
	  if (TimeOutTapBistableWest == 0) {
	    mcp23017.ToggleSingleBit(MCP23017_BNK0_OLATA, 1, 0x00);
	    TimeOutTapBistableWest = T_500MSEC;	
	  }
	} else {
	  // MonoStable
	  mcp23017.SetSingleBit(MCP23017_BNK0_OLATA, 1, 0x00);
	  TimeOutTapMonostableWest = MonoStableTime.ui;
	  MonoStableTapWest = MonoStableTapWest_Wait;
	}					
	break;
      case GESTURE_MASK_TAP_NORTH:
        if (DigInputStatus.In.Jumper1 != 0) {
          //	BiStable
	  if (TimeOutTapBistableNorth == 0) {
	    mcp23017.ToggleSingleBit(MCP23017_BNK0_OLATA, 2, 0x00);
	    TimeOutTapBistableNorth = T_500MSEC;	
          }
	} else {
	  // MonoStable
	  mcp23017.SetSingleBit(MCP23017_BNK0_OLATA, 2, 0x00);
	  TimeOutTapMonostableNorth = MonoStableTime.ui;
	  MonoStableTapNorth = MonoStableTapNorth_Wait;
	}			
	break;
      case GESTURE_MASK_TAP_EAST:
        if (DigInputStatus.In.Jumper1 != 0) {
	  //	BiStable
	  if (TimeOutTapBistableEast == 0) {
	    mcp23017.ToggleSingleBit(MCP23017_BNK0_OLATA, 3, 0x00);
	    TimeOutTapBistableEast = T_500MSEC;	
	  }
	} else {
	  // MonoStable
	  mcp23017.SetSingleBit(MCP23017_BNK0_OLATA, 3, 0x00);
	  TimeOutTapMonostableEast = MonoStableTime.ui;
	  MonoStableTapEast = MonoStableTapEast_Wait;
	}					
	break;
      case GESTURE_MASK_TAP_CENTRE:
        if (DigInputStatus.In.Jumper1 != 0) {
	  //	BiStable
	  if (TimeOutTapBistableCentre == 0) {
	    mcp23017.ToggleSingleBit(MCP23017_BNK0_OLATA, 4, 0x00);
	    TimeOutTapBistableCentre = T_500MSEC;	
	  }
	} else {
	  // MonoStable
	  mcp23017.SetSingleBit(MCP23017_BNK0_OLATA, 4, 0x00);
	  TimeOutTapMonostableCentre = MonoStableTime.ui;
	  MonoStableTapCentre = MonoStableTapCentre_Wait;
	}					
	break;
      case GESTURE_MASK_DOUBLE_TAP_SOUTH:
        if (DigInputStatus.In.Jumper1 != 0) {
	  //	BiStable
	  if (TimeOutDoubleTapBistableSouth == 0) {
	    mcp23017.ToggleSingleBit(MCP23017_BNK0_OLATA, 5, 0x00);
	    TimeOutDoubleTapBistableSouth = T_500MSEC;
          }
	} else {
	  // MonoStable
	  mcp23017.SetSingleBit(MCP23017_BNK0_OLATA, 5, 0x00);
	  TimeOutDoubleTapMonostableSouth = MonoStableTime.ui;
	  MonoStableDoubleTapSouth = MonoStableDoubleTapSouth_Wait;
	}
	break;
      case GESTURE_MASK_DOUBLE_TAP_WEST:
        if (DigInputStatus.In.Jumper1 != 0) {
	  //	BiStable
	  if (TimeOutDoubleTapBistableWest == 0) {
	    mcp23017.ToggleSingleBit(MCP23017_BNK0_OLATA, 6, 0x00);
	    TimeOutDoubleTapBistableWest = T_500MSEC;	
	  }
	} else {
	  // MonoStable
	  mcp23017.SetSingleBit(MCP23017_BNK0_OLATA, 6, 0x00);
	  TimeOutDoubleTapMonostableWest = MonoStableTime.ui;
	  MonoStableDoubleTapWest = MonoStableDoubleTapWest_Wait;
	}
	break;
      case GESTURE_MASK_DOUBLE_TAP_NORTH:
        if (DigInputStatus.In.Jumper1 != 0) {
	  //	BiStable
	  if (TimeOutDoubleTapBistableNorth == 0) {
	    mcp23017.ToggleSingleBit(MCP23017_BNK0_OLATA, 7, 0x00);
	    TimeOutDoubleTapBistableNorth = T_500MSEC;	
	  }
	} else {
	  // MonoStable
	  mcp23017.SetSingleBit(MCP23017_BNK0_OLATA, 7, 0x00);
	  TimeOutDoubleTapMonostableNorth = MonoStableTime.ui;
	  MonoStableDoubleTapNorth = MonoStableDoubleTapNorth_Wait;
	}
	break;
      case GESTURE_MASK_DOUBLE_TAP_EAST:
        if (DigInputStatus.In.Jumper1 != 0) {
	  //	BiStable
	  if (TimeOutDoubleTapBistableEast == 0) {
	    mcp23017.ToggleSingleBit(MCP23017_BNK0_OLATA, 0, 0x01);
	    TimeOutDoubleTapBistableEast = T_500MSEC;	
	  }
        } else {
	  // MonoStable
	  mcp23017.SetSingleBit(MCP23017_BNK0_OLATA, 0, 0x01);
	  TimeOutDoubleTapMonostableEast = MonoStableTime.ui;
	  MonoStableDoubleTapEast = MonoStableDoubleTapEast_Wait;
	}
	break;
      case GESTURE_MASK_DOUBLE_TAP_CENTRE:
        if (DigInputStatus.In.Jumper1 != 0) {
	  //	BiStable
	  if (TimeOutDoubleTapBistableCentre == 0) {
	    mcp23017.ToggleSingleBit(MCP23017_BNK0_OLATA, 1, 0x01);
	    TimeOutDoubleTapBistableCentre = T_500MSEC;	
	  }
	} else {
	  // MonoStable
	  mcp23017.SetSingleBit(MCP23017_BNK0_OLATA, 1, 0x01);
	  TimeOutDoubleTapMonostableCentre = MonoStableTime.ui;
	  MonoStableDoubleTapCentre = MonoStableDoubleTapCentre_Wait;
	}
	break;
      case GESTURE_MASK_WEST_EAST:
        if (DigInputStatus.In.Jumper1 != 0) {
	  //	BiStable
	  if (TimeOutWestEastBistable == 0) {
	    mcp23017.ToggleSingleBit(MCP23017_BNK0_OLATA, 2, 0x01);
	    TimeOutWestEastBistable = T_500MSEC;	
          }
	} else {
	  // MonoStable
	  mcp23017.SetSingleBit(MCP23017_BNK0_OLATA, 2, 0x01);
	  TimeOutWestEastMonostable = MonoStableTime.ui;
	  MonoStableGestureWestEast = MonoStableGestureWestEast_Wait;	
	}					
	break;
      case GESTURE_MASK_EAST_WEST:
        if (DigInputStatus.In.Jumper1 != 0) {
	  //	BiStable
	  if (TimeOutEastWestBistable == 0) {
	    mcp23017.ToggleSingleBit(MCP23017_BNK0_OLATA, 3, 0x01);
	    TimeOutEastWestBistable = T_500MSEC;	
	  }
	} else {
	  // MonoStable
	  mcp23017.SetSingleBit(MCP23017_BNK0_OLATA, 3, 0x01);
	  TimeOutEastWestMonostable = MonoStableTime.ui;
	  MonoStableGestureEastWest = MonoStableGestureEastWest_Wait;	
	}
	break;
      case GESTURE_MASK_SOUTH_NORTH:
        if (DigInputStatus.In.Jumper1 != 0) {
	  //	BiStable
	  if (TimeOutSouthNorthBistable == 0) {
	    mcp23017.ToggleSingleBit(MCP23017_BNK0_OLATA, 4, 0x01);
	    TimeOutSouthNorthBistable = T_500MSEC;	
	  }
	} else {
	  // MonoStable
	  mcp23017.SetSingleBit(MCP23017_BNK0_OLATA, 4, 0x01);
	  TimeOutSouthNorthMonostable = MonoStableTime.ui;
	  MonoStableGestureSouthNorth = MonoStableGestureSouthNorth_Wait;
	}
	break;
      case GESTURE_MASK_NORTH_SOUTH:
        if (DigInputStatus.In.Jumper1 != 0) {
	  //	BiStable
	  if (TimeOutNorthSouthBistable == 0) {
	    mcp23017.ToggleSingleBit(MCP23017_BNK0_OLATA, 5, 0x01);
	    TimeOutNorthSouthBistable = T_500MSEC;	
	  }
	} else {
	  // MonoStable
	  mcp23017.SetSingleBit(MCP23017_BNK0_OLATA, 5, 0x01);
	  TimeOutNorthSouthMonostable = MonoStableTime.ui;
	   MonoStableGestureNorthSouth = MonoStableGestureNorthSouth_Wait;
	}
	break;
      case GESTURE_MASK_CLOCK_WISE:
        if (DigInputStatus.In.Jumper1 != 0) {
	  //	BiStable
	  if (TimeOutClockWiseBistable == 0) {
	    mcp23017.ToggleSingleBit(MCP23017_BNK0_OLATA, 6, 0x01);
	    TimeOutClockWiseBistable = T_500MSEC;
          }
	} else {
	  // MonoStable
	  mcp23017.ToggleSingleBit(MCP23017_BNK0_OLATA, 6, 0x01);
	  TimeOutClockWiseMonostable = MonoStableTime.ui;
	  MonoStableGestureClockWise = MonoStableGestureClockWise_Wait;
	}
	break;
      case GESTURE_MASK_COUNTER_CLOCK_WISE:
        if (DigInputStatus.In.Jumper1 != 0) {
	  //	BiStable
	  if (TimeOutCounterClockWiseBistable == 0) {
	    mcp23017.ToggleSingleBit(MCP23017_BNK0_OLATA, 7, 0x01);
	    TimeOutCounterClockWiseBistable = T_500MSEC;	
	  }
	} else {
	  // MonoStable
	  mcp23017.ToggleSingleBit(MCP23017_BNK0_OLATA, 7, 0x01);
	  TimeOutCounterClockWiseMonostable = MonoStableTime.ui;
	  MonoStableGestureCounterClockWise = MonoStableGestureCounterClockWise_Wait;
	}
	break;
    }
    MaskGesture <<= 1;
  } while (MaskGesture != 0x0000);	
}
//======================================================================

//======================================================================
void MonoStableTapNorth_Idle(void) { }
void MonoStableTapNorth_Wait(void)
{
  if (TimeOutTapMonostableNorth == 0) {
    mcp23017.ResetSingleBit(MCP23017_BNK0_OLATA, 2, 0x00);
    MonoStableTapNorth = MonoStableTapNorth_Idle;
  }
}

void MonoStableDoubleTapNorth_Idle(void) { }
void MonoStableDoubleTapNorth_Wait(void)
{
  if (TimeOutDoubleTapMonostableNorth == 0) {
    mcp23017.ResetSingleBit(MCP23017_BNK0_OLATA, 7, 0x00);
    MonoStableDoubleTapNorth = MonoStableDoubleTapNorth_Idle;	
  }
}
//======================================================================

//======================================================================
void MonoStableTapSouth_Idle(void) { }
void MonoStableTapSouth_Wait(void)
{
  if (TimeOutTapMonostableSouth == 0) {
    mcp23017.ResetSingleBit(MCP23017_BNK0_OLATA, 0, 0x00);
    MonoStableTapSouth = MonoStableTapSouth_Idle;
  }
}

void MonoStableDoubleTapSouth_Idle(void) { }
void MonoStableDoubleTapSouth_Wait(void)
{
  if (TimeOutDoubleTapMonostableSouth == 0) {
    mcp23017.ResetSingleBit(MCP23017_BNK0_OLATA, 5, 0x00);
    MonoStableDoubleTapSouth = MonoStableDoubleTapSouth_Idle;	
  }
}
//======================================================================

//======================================================================
void MonoStableTapWest_Idle(void) { }
void MonoStableTapWest_Wait(void)
{
  if (TimeOutTapMonostableWest == 0) {
    mcp23017.ResetSingleBit(MCP23017_BNK0_OLATA, 1, 0x00);
    MonoStableTapWest = MonoStableTapWest_Idle;
  }
}

void MonoStableDoubleTapWest_Idle(void) { }
void MonoStableDoubleTapWest_Wait(void)
{
  if (TimeOutDoubleTapMonostableWest == 0) {
    mcp23017.ResetSingleBit(MCP23017_BNK0_OLATA, 6, 0x00);
    MonoStableDoubleTapWest = MonoStableDoubleTapWest_Idle;	
  }
}
//======================================================================

//======================================================================
void MonoStableTapEast_Idle(void) { }
void MonoStableTapEast_Wait(void)
{
  if (TimeOutTapMonostableEast == 0) {
    mcp23017.ResetSingleBit(MCP23017_BNK0_OLATA, 3, 0x00);
    MonoStableTapEast = MonoStableTapEast_Idle;
  }
}
	
void MonoStableDoubleTapEast_Idle(void) { }
void MonoStableDoubleTapEast_Wait(void)
{
  if (TimeOutDoubleTapMonostableEast == 0) {
    mcp23017.ResetSingleBit(MCP23017_BNK0_OLATA, 0, 0x01);
    MonoStableDoubleTapEast = MonoStableDoubleTapEast_Idle;	
  }
}
//======================================================================

//======================================================================
void MonoStableTapCentre_Idle(void) { }
void MonoStableTapCentre_Wait(void)
{
  if (TimeOutTapMonostableCentre == 0) {
    mcp23017.ResetSingleBit(MCP23017_BNK0_OLATA, 4, 0x00);
    MonoStableTapCentre = MonoStableTapCentre_Idle;
  }
}

void MonoStableDoubleTapCentre_Idle(void) { }
void MonoStableDoubleTapCentre_Wait(void)
{
  if (TimeOutDoubleTapMonostableCentre == 0) {
    mcp23017.ResetSingleBit(MCP23017_BNK0_OLATA, 1, 0x01);
    MonoStableDoubleTapCentre = MonoStableDoubleTapCentre_Idle;
  }
}
//======================================================================

//======================================================================
void MonoStableGestureWestEast_Idle(void) { }
void MonoStableGestureWestEast_Wait(void)
{
  if (TimeOutWestEastMonostable == 0) {
    mcp23017.ResetSingleBit(MCP23017_BNK0_OLATA, 2, 0x01);
    MonoStableGestureWestEast = MonoStableGestureWestEast_Idle;
  }
}

void MonoStableGestureEastWest_Idle(void) { }
void MonoStableGestureEastWest_Wait(void)
{
  if (TimeOutEastWestMonostable == 0) {
    mcp23017.ResetSingleBit(MCP23017_BNK0_OLATA, 3, 0x01);
    MonoStableGestureEastWest = MonoStableGestureEastWest_Idle;
  }	
}

void MonoStableGestureSouthNorth_Idle(void) { }
void MonoStableGestureSouthNorth_Wait(void)
{
  if (TimeOutSouthNorthMonostable == 0) {
    mcp23017.ResetSingleBit(MCP23017_BNK0_OLATA, 4, 0x01);
    MonoStableGestureSouthNorth = MonoStableGestureSouthNorth_Idle;
  }	
}

void MonoStableGestureNorthSouth_Idle(void) { }
void MonoStableGestureNorthSouth_Wait(void)
{
  if (TimeOutNorthSouthMonostable == 0) {
    mcp23017.ResetSingleBit(MCP23017_BNK0_OLATA, 5, 0x01);
    MonoStableGestureNorthSouth = MonoStableGestureNorthSouth_Idle;
  }	
}
//======================================================================

//======================================================================
void MonoStableGestureClockWise_Idle(void) { }
void MonoStableGestureClockWise_Wait(void)
{
  if (TimeOutClockWiseMonostable == 0) {
    mcp23017.ResetSingleBit(MCP23017_BNK0_OLATA, 6, 0x01);
    MonoStableGestureClockWise = MonoStableGestureClockWise_Idle;
  }	
}
//======================================================================

//======================================================================
void MonoStableGestureCounterClockWise_Idle(void) { }
void MonoStableGestureCounterClockWise_Wait(void)
{
  if (TimeOutCounterClockWiseMonostable == 0) {
    mcp23017.ResetSingleBit(MCP23017_BNK0_OLATA, 7, 0x01);
    MonoStableGestureCounterClockWise = MonoStableGestureCounterClockWise_Idle;
  }	
}
//======================================================================


