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
    //  Starts Keyboard and Mouse management  
    Keyboard.begin();
    Mouse.begin();
    KeybMouseEnable = true;
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
    Keyboard.end();
    Mouse.end();
    KeybMouseEnable = false;
    Input_Management = Input_Idle;
  }
}
//======================================================================

