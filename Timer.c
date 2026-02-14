 #include <xc.h>

#include "main.h"
void init_timer0()
{
    //setting the internal clock source
    T0CS = 0;
    //Assigning the prescaler
    PSA = 1;
   
    TMR0 = 6;
    //Enabling the timer0
    TMR0IE = 1;
    //Timer flag
    TMR0IF = 0;
    
    
    
}