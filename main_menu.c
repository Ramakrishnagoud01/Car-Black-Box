 #include <xc.h>
#include "main.h"

/* Menu strings displayed on the LCD */
char *menu[] = {
    "  VIEW LOG       ",
    "  CLEAR LOG     ",
    "  DOWNLOAD LOG  ",
    "  SET TIME      ",
    "  CHANGE PASSWD"
};

/* Menu position variables */
int pos1 = 0, pos2 = 0;

void main_menu(unsigned char key)
{
    /* Index of first item currently displayed on LCD */
    static int display;

    /* Selected menu state (-1 means no selection yet) */
    static int state = -1;

    /* Flag reserved for future extension (not used currently) */
    static int state_active = 0;

    /* Selection flag (not used directly here) */
    static unsigned char sel = 0;

    /* Used to refresh display only when needed */
    static int user_active = 0;

    /* Ensures menu initialization happens only once on entry */
    static unsigned char menu_once = 1;

 
    if(operation != 2)
    {
        menu_once = 1;
        return;
    }

    /* One-time initialization when entering main menu */
    if(menu_once)
    {
        state = -1;      
        pos1 = 0;         
        user_active = 0; 
        menu_once = 0;
    }

    /* -------- MENU NAVIGATION MODE -------- */
    if(state == -1)
    {
        /* SW1: Move menu selection down */
        if(key == SW1)
        {
            if(pos1 < 4)       // Max index is 4
            {
                user_active = 0;
                pos1++;
            }
        }

        /* SW2: Move menu selection up */
        if(key == SW2)
        {
            if(pos1 > 0)
            {
                user_active = 0;
                pos1--;
            }
        }

        /* Refresh display only when user_active is reset */
        if(user_active == 0)
        {
            /* Calculate which two menu items to display */
            display = (pos1 / 2) * 2;

            /* Prevent overflow for last menu items */
            if (display > 3)
            {
                display = 3;
            }

            /* Display two menu items on LCD */
            clcd_print(menu[display], LINE1(1));
            clcd_print(menu[display + 1], LINE2(1));

            /* Clear previous selection indicators */
            clcd_putch(' ', LINE1(0));
            clcd_putch(' ', LINE2(0));

            /* Place cursor arrow on selected item */
            if(pos1 == display)
            {
                clcd_putch('>', LINE1(0));
            }
            else
            {
                clcd_putch('>', LINE2(0));
            }

            user_active = 1;   // Mark display as updated
        }

        /* SW4: Confirm menu selection */
        if(key == SW4)
        {
            state = pos1;      // Store selected menu index
            state_active = 0;
            user_active = 0;
        }
    }

    /* -------- MENU SELECTION EXECUTION -------- */
    if(state != -1)
    {
        if(state == 0)
        {
            operation = 3;    // VIEW LOG
            state = -1;
            return;
        }
        else if(state == 1)
        {
            operation = 4;    // CLEAR LOG
            state = -1;
            return;
        }
        else if(state == 2)
        {
            operation = 5;    // DOWNLOAD LOG
            state = -1;
            return;
        }
        else if(state == 3)
        {
            operation = 6;    // SET TIME
            state = -1;
            return;
        }
        else if(state == 4)
        {
            operation = 7;    // CHANGE PASSWORD
            state = -1;
            return;
        }
    }
}
