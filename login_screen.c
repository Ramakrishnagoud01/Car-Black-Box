 #include <xc.h>
#include "main.h"

/* System time counters (updated in ISR) */
volatile unsigned int sec = 0;
volatile unsigned int new_sec = 59;

/* Default and user passwords */
char pass[5] = "1010";
char user_pass[5];

void login_screen(unsigned char key)
{
    /* Password index */
    static unsigned int i = 0;

    /* Login attempts */
    static int attempt = 3;

    /* Block timing */
    static unsigned int block_start_sec = 0;

    /* User activity flag */
    static int user_active = 0;

    /* Timing helpers */
    static unsigned int prev_sec = 0;
    static unsigned int idle_prev_sec = 0;

    /* State flag: 0-login, 2-blocked, 3-idle */
    static int flag = 0;

    /* Idle counter */
    static unsigned int idle_count = 0;

    /* LCD clear string */
    static char str[17] = "                ";

    /* -------- LOGIN STATE -------- */
    if (flag == 0)
    {
        clcd_print(" ENTER PASSWORD ", LINE1(0));

        if (i == 0)
            clcd_print(str, LINE2(0));

        /* Password entry */
        if (i < 4)
        {
            /* Cursor blink */
            if (sec != prev_sec)
            {
                prev_sec = sec;
                clcd_putch((sec % 2) ? ' ' : 0xFF, LINE2(i + 3));
            }

            /* Idle timeout check */
            if (user_active == 0)
            {
                if (sec != idle_prev_sec)
                {
                    idle_prev_sec = sec;
                    idle_count++;
                }

                if (idle_count >= 10)
                {
                    flag = 3;
                    idle_count = 0;
                }
            }

            /* Read password bits */
            if (key == SW1 || key == SW2)
            {
                user_active = 1;
                idle_count = 0;

                clcd_putch('*', LINE2(i + 3));
                user_pass[i++] = (key == SW1) ? '0' : '1';
            }
        }
        else
        {
            user_pass[i] = '\0';

            /* Password check */
            if (my_strcmp(pass, user_pass) == 0)
            {
                clcd_print("  LOGIN SUCCESS", LINE1(0));
                clcd_print("               ", LINE2(0));
                __delay_ms(1500);

                operation = 2;
                flag = 0;
                i = 0;
                user_active = 0;
                idle_count = 0;
                idle_prev_sec = sec;
                return;
            }
            else
            {
                attempt--;

                /* Block after 3 failures */
                if (attempt == 0)
                {
                    flag = 2;
                    block_start_sec = sec;
                }
                else
                {
                    clcd_print(" WRONG PASSWORD ", LINE1(0));
                    clcd_print(" ATTEMPTS LEFT  ", LINE2(0));
                    clcd_putch(attempt + '0', LINE2(15));
                    __delay_ms(2500);

                    i = 0;
                    user_active = 0;
                    idle_count = 0;
                    idle_prev_sec = sec;
                }
            }
        }
    }

    /* -------- BLOCKED STATE -------- */
    else if (flag == 2)
    {
        if (sec != prev_sec)
        {
            prev_sec = sec;
            unsigned int elapsed = sec - block_start_sec;

            if (elapsed <= 59)
            {
                unsigned int remain = 59 - elapsed;
                clcd_print("YOU ARE BLOCKED", LINE1(0));
                clcd_print("WAIT FOR    SEC", LINE2(0));
                clcd_putch((remain / 10) + '0', LINE2(9));
                clcd_putch((remain % 10) + '0', LINE2(10));
            }
            else
            {
                attempt = 3;
                i = 0;
                flag = 0;
                idle_prev_sec = sec;
            }
        }
    }

    /* -------- IDLE TIMEOUT STATE -------- */
    else if (flag == 3)
    {
        if (key == SW4)
        {
            flag = 0;
            operation = 0;
            user_active = 0;
            idle_count = 0;
            idle_prev_sec = sec;
        }
    }
}
