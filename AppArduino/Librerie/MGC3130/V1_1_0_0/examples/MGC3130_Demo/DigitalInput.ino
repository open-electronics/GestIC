
#include <EEPROM.h>

//======================================================================
//  Sets digital inputs
void SetInputPin(void) {
  pinMode(Jumper1, INPUT);            // set pin to input
  pinMode(P1_Button, INPUT);          // set pin to input
  pinMode(P2_Button, INPUT);          // set pin to input
  pinMode(P3_Button, INPUT);          // set pin to input
  digitalWrite(Jumper1, HIGH);        // turn on pullup resistors
  digitalWrite(P1_Button, HIGH);      // turn on pullup resistors
  digitalWrite(P2_Button, HIGH);      // turn on pullup resistors
  digitalWrite(P3_Button, HIGH);      // turn on pullup resistors 
 
  DigInputStatus.Input = 0xFF;  
}
//======================================================================

//======================================================================
//  Debouncing digital inputs
void DebouncingInput(void) {
  DigInputReaded.In.P1_Button = digitalRead(P1_Button);
  DigInputReaded.In.P2_Button = digitalRead(P2_Button);
  DigInputReaded.In.P3_Button = digitalRead(P3_Button);
  DigInputReaded.In.Jumper1   = digitalRead(Jumper1);

  if (DigInputReaded.Input != DigInputStatus.Input) {
    if (DigInputReaded.Input != DigInputVar) {
       DigInputVar = DigInputReaded.Input;
       DebouncingTimeOut = T_50MSEC;
    } else {
      if (DebouncingTimeOut == 0) {
         DigInputVar = DigInputReaded.Input;
         DigInputStatus.Input = DigInputReaded.Input;
      }
    }
  } else {
      DigInputVar = DigInputStatus.Input;
  }
}
//======================================================================

//======================================================================
void Input_Idle(void) {
  if ((DigInputStatus.In.P1_Button != 1) && (DigInputStatus.In.P2_Button != 1) && (DigInputStatus.In.P3_Button != 1)) {
    TimeOutWait = T_2SEC;
    Input_Management = Input_WaitP1P2P3;
    return;
  }
  if ((DigInputStatus.In.P1_Button != 1) && (DigInputStatus.In.P2_Button != 1)) {
    TimeOutWait = T_2SEC;
    Input_Management = Input_WaitP1P2;
    return;
  }
  if ((DigInputStatus.In.P2_Button != 1) && (DigInputStatus.In.P3_Button != 1)) {
    TimeOutWait = T_2SEC;
    Input_Management = Input_WaitP2P3;
    return;
  }
  if ((DigInputStatus.In.P1_Button != 1) && (DigInputStatus.In.P3_Button != 1)) {
    TimeOutWait = T_2SEC;
    Input_Management = Input_WaitP1P3;
    return;
  }		
  if (DigInputStatus.In.P1_Button != 1) {
    TimeOutWait = T_2SEC;
    Input_Management = Input_WaitP1;	 
    return;
  }
  if (DigInputStatus.In.P2_Button != 1) {
    TimeOutWait = T_2SEC;
    Input_Management = Input_WaitP2;	 
    return;
  }
  if (DigInputStatus.In.P3_Button != 1) {
    TimeOutWait = T_2SEC;
    Input_Management = Input_WaitP3;
    return;
  }  
}
//======================================================================

//======================================================================
void Input_WaitP1(void) {
  if ((TimeOutWait == 0) && (DigInputStatus.In.P1_Button != 1)) {
    Input_Management = Input_ReleasedP1;
    Led_Management   = Led_P1Pressed;
  } else if ((TimeOutWait > 0) && (DigInputStatus.In.P1_Button != 0)) {
    Input_Management = Input_Idle;
  }
}

void Input_ReleasedP1(void) {
  if (DigInputStatus.In.P1_Button != 0) {
    MonoStableTime.ui = T_1SEC;
    EEPROM.write(0x0000, MonoStableTime.Int.uch);
    EEPROM.write(0x0001, MonoStableTime.Int.ucl);
    Input_Management = Input_Idle;
  }
}
//======================================================================

//======================================================================
void Input_WaitP2(void) {
  if ((TimeOutWait == 0) && (DigInputStatus.In.P2_Button != 1)) {
    Input_Management = Input_ReleasedP2;
    Led_Management   = Led_P2Pressed;
  } else if ((TimeOutWait > 0) && (DigInputStatus.In.P2_Button != 0)) {
    Input_Management = Input_Idle;
  }
}

void Input_ReleasedP2(void) {
  if (DigInputStatus.In.P2_Button != 0) {
    MonoStableTime.ui = T_5SEC;
    EEPROM.write(0x0000, MonoStableTime.Int.uch);
    EEPROM.write(0x0001, MonoStableTime.Int.ucl);
    Input_Management = Input_Idle;
  }
}
//======================================================================

//======================================================================
void Input_WaitP3(void) {
  if ((TimeOutWait == 0) && (DigInputStatus.In.P3_Button != 1)) {
    Input_Management = Input_ReleasedP3;
    Led_Management   = Led_P3Pressed;
  } else if ((TimeOutWait > 0) && (DigInputStatus.In.P3_Button != 0)) {
    Input_Management = Input_Idle;
  }
}

void Input_ReleasedP3(void)
{
  if (DigInputStatus.In.P3_Button != 0) {
    MonoStableTime.ui = T_10SEC;
    EEPROM.write(0x0000, MonoStableTime.Int.uch);
    EEPROM.write(0x0001, MonoStableTime.Int.ucl);
    Input_Management = Input_Idle;
  }
}
//======================================================================

//======================================================================
void Input_WaitP1P2(void)
{
  if ((TimeOutWait == 0) && (DigInputStatus.In.P1_Button != 1) && (DigInputStatus.In.P2_Button != 1)) {
    Input_Management = Input_ReleasedP1P2;
    Led_Management   = Led_P1P2Pressed;
  } else if ((TimeOutWait > 0) && (DigInputStatus.In.P1_Button != 0) && (DigInputStatus.In.P2_Button != 0)) {
    Input_Management = Input_Idle;
  }
}

void Input_ReleasedP1P2(void)
{
  if ((DigInputStatus.In.P1_Button != 0) && (DigInputStatus.In.P2_Button != 0)) {
    MonoStableTime.ui = T_15SEC;
    EEPROM.write(0x0000, MonoStableTime.Int.uch);
    EEPROM.write(0x0001, MonoStableTime.Int.ucl);
    Input_Management = Input_Idle;
  }
}
//======================================================================

//======================================================================
void Input_WaitP2P3(void)
{
  if ((TimeOutWait == 0) && (DigInputStatus.In.P2_Button != 1) && (DigInputStatus.In.P3_Button != 1)) {
    Input_Management = Input_ReleasedP2P3;
    Led_Management   = Led_P2P3Pressed;
  } else if ((TimeOutWait > 0) && (DigInputStatus.In.P2_Button != 0) && (DigInputStatus.In.P3_Button != 0)) {
    Input_Management = Input_Idle;
  }
}

void Input_ReleasedP2P3(void)
{
  if ((DigInputStatus.In.P2_Button != 0) && (DigInputStatus.In.P3_Button != 0)) {
    MonoStableTime.ui = T_20SEC;
    EEPROM.write(0x0000, MonoStableTime.Int.uch);
    EEPROM.write(0x0001, MonoStableTime.Int.ucl);
    Input_Management = Input_Idle;
  }
}
//======================================================================

//======================================================================
void Input_WaitP1P3(void)
{
  if ((TimeOutWait == 0) && (DigInputStatus.In.P1_Button != 1) && (DigInputStatus.In.P3_Button != 1)) {
    Input_Management = Input_ReleasedP1P3;
    Led_Management   = Led_P1P3Pressed;
  } else if ((TimeOutWait > 0) && (DigInputStatus.In.P1_Button != 0) && (DigInputStatus.In.P3_Button != 0)) {
    Input_Management = Input_Idle;
  }	
}

void Input_ReleasedP1P3(void)
{
  if ((DigInputStatus.In.P1_Button != 0) && (DigInputStatus.In.P3_Button != 0)) {
    MonoStableTime.ui = T_25SEC;
    EEPROM.write(0x0000, MonoStableTime.Int.uch);
    EEPROM.write(0x0001, MonoStableTime.Int.ucl);
    Input_Management = Input_Idle;
  }
}
//======================================================================

//======================================================================
void Input_WaitP1P2P3(void)
{
  if ((TimeOutWait == 0) && (DigInputStatus.In.P1_Button != 1) && (DigInputStatus.In.P2_Button != 1) && (DigInputStatus.In.P3_Button != 1)) {
    Input_Management = Input_ReleasedP1P2P3;
    Led_Management   = Led_P1P2P3Pressed;
  } else if ((TimeOutWait > 0) && (DigInputStatus.In.P1_Button != 0) && (DigInputStatus.In.P2_Button != 0) && (DigInputStatus.In.P3_Button != 0)) {
    Input_Management = Input_Idle;
  }
}

void Input_ReleasedP1P2P3(void)
{
  if ((DigInputStatus.In.P1_Button != 0) && (DigInputStatus.In.P2_Button != 0) && (DigInputStatus.In.P3_Button != 0)) {
    MonoStableTime.ui = T_30SEC;
    EEPROM.write(0x0000, MonoStableTime.Int.uch);
    EEPROM.write(0x0001, MonoStableTime.Int.ucl);
    Input_Management = Input_Idle;
  }
}
//======================================================================


