//======================================================================
#define Fosc          (unsigned long)16000000    //  Clock 16MHz
#define PRET1         (unsigned long)256         //  Prescaler 256
#define MSEC          2                          //  Time Base: 2mSec
#define SLOWBASETIME  (0xFFFF - ((Fosc * MSEC) / (PRET1 * 1000)))
//======================================================================

//======================================================================
void SetupTimer(void) {   
  // initialize timer1 
  noInterrupts();           // disable all interrupts
  TCCR1A = 0;
  TCCR1B = 0;
  
  TCNT1   = SLOWBASETIME;
  TCCR1B |= 0x04;           // Prescaler 256
  TIMSK1 |= 0x01;           // enable oveflow timer interrupt
  interrupts();             // enable all interrupts 
}
//======================================================================

//======================================================================
ISR(TIMER1_OVF_vect)
{
  TCNT1 = SLOWBASETIME;    // preload timer

  if (DebouncingTimeOut > 0) { DebouncingTimeOut--; }
  if (TimeOutWait       > 0) { TimeOutWait--;       }
  if (TimeOutLed        > 0) { TimeOutLed--;        }

  if (TimeOutNorthGest  > 0) { TimeOutNorthGest--;  }
  if (TimeOutSouthGest  > 0) { TimeOutSouthGest--;  }
  if (TimeOutWestGest   > 0) { TimeOutWestGest--;   }
  if (TimeOutEastGest   > 0) { TimeOutEastGest--;   }
  if (TimeOutCentreGest > 0) { TimeOutCentreGest--; }

  if (TimeOutTapBistableSouth  > 0)   { TimeOutTapBistableSouth--;       }
  if (TimeOutTapBistableWest   > 0)   { TimeOutTapBistableWest--;        }
  if (TimeOutTapBistableNorth  > 0)   { TimeOutTapBistableNorth--;       }
  if (TimeOutTapBistableEast   > 0)   { TimeOutTapBistableEast--;        }
  if (TimeOutTapBistableCentre > 0)   { TimeOutTapBistableCentre--;      }

  if (TimeOutTapMonostableSouth  > 0) { TimeOutTapMonostableSouth--;      }
  if (TimeOutTapMonostableWest   > 0) { TimeOutTapMonostableWest--;       }
  if (TimeOutTapMonostableNorth  > 0) { TimeOutTapMonostableNorth--;      }
  if (TimeOutTapMonostableEast   > 0) { TimeOutTapMonostableEast--;       }
  if (TimeOutTapMonostableCentre > 0) { TimeOutTapMonostableCentre--;     }

  if (TimeOutDoubleTapBistableSouth  > 0)    { TimeOutDoubleTapBistableSouth--;     }
  if (TimeOutDoubleTapBistableWest   > 0)    { TimeOutDoubleTapBistableWest--;      }
  if (TimeOutDoubleTapBistableNorth  > 0)    { TimeOutDoubleTapBistableNorth--;     }
  if (TimeOutDoubleTapBistableEast   > 0)    { TimeOutDoubleTapBistableEast--;      }
  if (TimeOutDoubleTapBistableCentre > 0)    { TimeOutDoubleTapBistableCentre--;    }

  if (TimeOutDoubleTapMonostableSouth  > 0)  { TimeOutDoubleTapMonostableSouth--;   }
  if (TimeOutDoubleTapMonostableWest   > 0)  { TimeOutDoubleTapMonostableWest--;    }
  if (TimeOutDoubleTapMonostableNorth  > 0)  { TimeOutDoubleTapMonostableNorth--;   }
  if (TimeOutDoubleTapMonostableEast   > 0)  { TimeOutDoubleTapMonostableEast--;    }
  if (TimeOutDoubleTapMonostableCentre > 0)  { TimeOutDoubleTapMonostableCentre--;  }

  if (TimeOutWestEastBistable   > 0)         { TimeOutWestEastBistable--;           }
  if (TimeOutEastWestBistable   > 0)         { TimeOutEastWestBistable--;           }
  if (TimeOutSouthNorthBistable > 0)         { TimeOutSouthNorthBistable--;         }
  if (TimeOutNorthSouthBistable > 0)         { TimeOutNorthSouthBistable--;         }
 
  if (TimeOutWestEastMonostable   > 0)       { TimeOutWestEastMonostable--;         }
  if (TimeOutEastWestMonostable   > 0)       { TimeOutEastWestMonostable--;         }
  if (TimeOutSouthNorthMonostable > 0)       { TimeOutSouthNorthMonostable--;       }
  if (TimeOutNorthSouthMonostable > 0)       { TimeOutNorthSouthMonostable--;       }
  
  if (TimeOutClockWiseBistable        > 0)   { TimeOutClockWiseBistable--;          }
  if (TimeOutCounterClockWiseBistable > 0)   { TimeOutCounterClockWiseBistable--;   }

  if (TimeOutClockWiseMonostable        > 0) { TimeOutClockWiseMonostable--;        }
  if (TimeOutCounterClockWiseMonostable > 0) { TimeOutCounterClockWiseMonostable--; }
}
//======================================================================


