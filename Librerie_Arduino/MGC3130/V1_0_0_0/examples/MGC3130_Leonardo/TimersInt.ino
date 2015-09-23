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
}
//======================================================================

