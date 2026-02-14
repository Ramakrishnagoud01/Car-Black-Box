 #include <xc.h>

/* Time variables updated every second */
volatile extern unsigned int sec, new_sec;

/* Timer0 Interrupt Service Routine */
void __interrupt() isr(void)
{
    static unsigned int count = 0;   // Interrupt counter

    if (TMR0IF == 1)                 // Timer0 interrupt flag
    {
        TMR0IF = 0;                  // Clear interrupt flag
        TMR0 = TMR0 + 8;             // Reload Timer0

        count++;

        /* Generate 1-second time base */
        if (count == 20000)
        {
            sec++;                   // Increment seconds
            new_sec--;               // Decrement secondary counter
            count = 0;

            if (sec > 59)          
            {
                sec = 0;
                new_sec = 59;
            }
        }
    }
}
