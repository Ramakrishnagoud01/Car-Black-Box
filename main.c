 #pragma config WDTE = OFF      // Disable Watchdog Timer

#include <xc.h>
#include "main.h"

/*
 * Global operation variable
 * Controls the current screen/state of the system
 *
 * operation values:
 * 0 -> Dashboard
 * 1 -> Login screen
 * 2 -> Main menu
 * 3 -> View log
 * 4 -> Clear log
 * 5 -> Download log
 * 6 -> Set time
 * 7 -> Change password
 */
unsigned char operation = 0;

/* Initialize all peripherals used in the project */
void init_config()
{
    init_adc();               // Initialize ADC for speed sensing
    init_clcd();              // Initialize character LCD
    init_digital_keypad();    // Initialize keypad
    init_i2c(100000);         // Initialize I2C (100 kHz)
    init_uart(9600);          // Initialize UART for log download
    init_ds1307();            // Initialize RTC
    init_timer0();            // Initialize Timer0 for 1-second timing
    GIE = 1;                  // Enable global interrupts
}

void main(void)
{
    init_config();            // System initialization
    unsigned char key;        // Stores current key state

    while(1)
    {
        /* Read keypad input */
        key = read_digital_keypad(STATE);

        /* ---------- EXIT FLAGS HANDLING ---------- */
        /* These flags are set by individual modules to return control */

        /* Exit from view log ? go to main menu */
        if(exit_view_log)
        {
            operation = 2;    // MAIN MENU
            exit_view_log = 0;
        }

        /* Exit from clear log ? go to main menu */
        if(exit_clear_log)
        {
            operation = 2;    // MAIN MENU
            exit_clear_log = 0;
        }

        /* Exit from download log ? go to main menu */
        if(exit_down_log)
        {
            operation = 2;    // MAIN MENU
            exit_down_log = 0;
        }

        /* Exit from change password ? go to dashboard */
        if(exit_ch_pass)
        {
            operation = 0;    // DASHBOARD
            exit_ch_pass = 0;
        }

        /* ---------- STATE TRANSITION ---------- */
        /* From dashboard, SW4 moves to login screen */
        if(key == SW4 && operation == 0)
        {
            operation = 1;    // LOGIN
        }

        /* ---------- STATE MACHINE ---------- */
        /* Call the function corresponding to the current operation */

        if(operation == 0)
            dashboard(key);

        else if(operation == 1)
            login_screen(key);

        else if(operation == 2)
            main_menu(key);

        else if(operation == 3)
            view_log(key);

        else if(operation == 4)
            clear_log();

        else if(operation == 5)
            down_log();

        else if(operation == 6)
            set_time(key);

        else if(operation == 7)
            change_pass(key);
    }
}

        
