//======================================================================
//  Sets digital inputs
void SetOutputPin(void) {
  pinMode(Led, OUTPUT);       // Set pin to Output
  digitalWrite(Led, HIGH);    // Led OFF
}

void Led_Idle(void) {
  if (TimeOutLed == 0) {
    digitalWrite(Led, HIGH);  // Led OFF
  }
}

void Led_P1Pressed(void) {
  digitalWrite(Led, LOW);    // Led ON
  TimeOutLed = T_250MSEC;
  Led_Management = Led_Idle;
}

void Led_P2Pressed(void) {
  digitalWrite(Led, LOW);    // Led ON
  TimeOutLed = T_250MSEC;
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
//  State Machine associated at Centre Elettrode for touch recognition (Tap, Double Tap and Touch)
void CentreElectrode_Idle(void)
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
      CentreElectrode = CentreElectrode_Gesture;  
    }
  }
}

void CentreElectrode_Gesture(void)
{
  CentreData = SumTouch((uint16_t )&CentreTouch[0], (sizeof(CentreTouch) - 1));
  if (CentreData == GESTURE_DOUBLE_TAP_CENTRE) {
    digitalWrite(Led, LOW);  // Led ON
    TimeOutLed = T_1SEC;    
    Keyboard.press(0xB1);  // ESC
  } else if (CentreData == GESTURE_TAP_CENTRE) {
    digitalWrite(Led, LOW);  // Led ON
    TimeOutLed = T_500MSEC;
    Keyboard.press(0x80);  // CTRL
    Keyboard.press(0x82);  // ALT
    Keyboard.press(0xC2);  // F1
  }
  
//  if (CentreData == GESTURE_DOUBLE_TAP_CENTRE) {
//    digitalWrite(Led, LOW);  // Led ON
//    TimeOutLed = T_1SEC;
//  } else if (CentreData == GESTURE_TAP_CENTRE) {
//    digitalWrite(Led, LOW);  // Led ON
//    TimeOutLed = T_500MSEC;
//  } else if (CentreData == GESTURE_TOUCH_CENTRE) {
//    digitalWrite(Led, LOW);  // Led ON
//    TimeOutLed = T_250MSEC;
//  }
  CentreCounter = EraseTouch((uint16_t )&CentreTouch[0], (sizeof(CentreTouch) - 1));
  CentreElectrode = CentreElectrode_Idle;
}
//======================================================================

//======================================================================
//  State Machine associated at West Elettrode for touch recognition (Tap, Double Tap and Touch)
void WestElectrode_Idle(void)
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
      WestElectrode = WestElectrode_Gesture;  
    }
  }
}

void WestElectrode_Gesture(void)
{
  WestData = SumTouch((uint16_t )&WestTouch[0], (sizeof(WestTouch) - 1));
  if (WestData == GESTURE_TAP_WEST) {
    digitalWrite(Led, LOW);  // Led ON
    TimeOutLed = T_500MSEC;
    if (ActionInProgress == false) {
      //  This action enable the full screen visualization
      ActionInProgress = true;
      Keyboard.press(0xB0);  // ENTER
    } 
  }
  
//  if (WestData == GESTURE_DOUBLE_TAP_WEST) {
//    digitalWrite(Led, LOW);  // Led ON
//    TimeOutLed = T_1SEC;
//  } else if (WestData == GESTURE_TAP_WEST) {
//    digitalWrite(Led, LOW);  // Led ON
//    TimeOutLed = T_500MSEC;
//  } else if (WestData == GESTURE_TOUCH_WEST) {
//    digitalWrite(Led, LOW);  // Led ON
//    TimeOutLed = T_250MSEC;
//  }
  WestCounter = EraseTouch((uint16_t )&WestTouch[0], (sizeof(WestTouch) - 1));
  WestElectrode = WestElectrode_Idle;
}
//======================================================================

//======================================================================
//  State Machine associated at East Elettrode for touch recognition (Tap, Double Tap and Touch)
void EastElectrode_Idle(void)
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
      EastElectrode = EastElectrode_Gesture;  
    }
  }
}

void EastElectrode_Gesture(void)
{
  EastData = SumTouch((uint16_t )&EastTouch[0], (sizeof(EastTouch) - 1));
  if (EastData == GESTURE_TAP_EAST) {
    digitalWrite(Led, LOW);  // Led ON
    TimeOutLed = T_500MSEC;
    if (ActionInProgress == false) {
      //  This action disable the full screen visualization
      ActionInProgress = true;
      Keyboard.press(0xB1);  // ESC
    }   
  }  
  
//  if (EastData == GESTURE_DOUBLE_TAP_EAST) {
//    digitalWrite(Led, LOW);  // Led ON
//    TimeOutLed = T_1SEC;
//  } else if (EastData == GESTURE_TAP_EAST) {
//    digitalWrite(Led, LOW);  // Led ON
//    TimeOutLed = T_500MSEC;
//  } else if (EastData == GESTURE_TOUCH_EAST) {
//    digitalWrite(Led, LOW);  // Led ON
//  }
  EastCounter = EraseTouch((uint16_t )&EastTouch[0], (sizeof(EastTouch) - 1));
  EastElectrode = EastElectrode_Idle;
}
//======================================================================

