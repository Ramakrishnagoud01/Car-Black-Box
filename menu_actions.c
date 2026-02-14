 #include <xc.h>
#include "main.h"

/* -------- LOG & SETTINGS MODULE -------- */

/* Exit flags used to return control to main loop */
unsigned char exit_view_log  = 0;
unsigned char exit_clear_log = 0;
unsigned char exit_down_log  = 0;
unsigned char exit_ch_pass   = 0;

/* -------------------------------------------------
 * VIEW LOG FUNCTION
 * ------------------------------------------------- */
void view_log(unsigned char key)
{
    static unsigned int read_addr = 0;
    static int flag = 1;
    static unsigned char no_log_once = 1;

    char line[17];
    unsigned int addr;

    if(event_count == 0)
    {
        if(no_log_once)
        {
            clcd_print(" NO LOGS FOUND ", LINE1(0));
            clcd_print("   PRESS SW5   ", LINE2(0));
            no_log_once = 0;
        }

        if(key == SW5)
        {
            exit_view_log = 1;
            no_log_once = 1;
            read_addr = 0;
            flag = 1;
        }
        return;
    }

    if(flag)
    {
        clcd_print("#   TIME   EV SP", LINE1(0));
        read_addr = 0;
        no_log_once = 1;
        flag = 0;
    }

    if(key == SW5)
    {
        exit_view_log = 1;
        read_addr = 0;
        flag = 1;
        return;
    }

    if(key == SW2 && read_addr >= 10)
        read_addr -= 10;
    else if(key == SW1 && read_addr < (event_count - 1) * 10)
        read_addr += 10;

    addr = read_addr;

    line[0] = (read_addr / 10) + '0';
    line[1] = ' ';

    line[2]  = read_external_eeprom(addr++);
    line[3]  = read_external_eeprom(addr++);
    line[4]  = ':';
    line[5]  = read_external_eeprom(addr++);
    line[6]  = read_external_eeprom(addr++);
    line[7]  = ':';
    line[8]  = read_external_eeprom(addr++);
    line[9]  = read_external_eeprom(addr++);

    line[10] = ' ';
    line[11] = read_external_eeprom(addr++);
    line[12] = read_external_eeprom(addr++);

    line[13] = ' ';
    line[14] = read_external_eeprom(addr++);
    line[15] = read_external_eeprom(addr++);

    line[16] = '\0';

    clcd_print(line, LINE2(0));
}

/* -------------------------------------------------
 * CLEAR LOG FUNCTION
 * ------------------------------------------------- */
void clear_log(void)
{
    static unsigned int delay = 0;
    static unsigned char once = 1;

    if(once)
    {
        event_count = 0;
        log_write_addr = 0;

        clcd_print("LOGS ARE CLEARED", LINE1(0));
        clcd_print("                ", LINE2(0));

        once = 0;
    }

    if(delay++ > 20000)
    {
        exit_clear_log = 1;
        delay = 0;
        once = 1;
    }
}

/* -------------------------------------------------
 * DOWNLOAD LOG FUNCTION
 * ------------------------------------------------- */
void down_log(void)
{
    static unsigned char once = 1;
    static unsigned int delay = 0;
    char line[17];
    unsigned int addr = 0;

    if (once)
    {
        clcd_print("  THE LOGS ARE  ", LINE1(0));
        clcd_print("  DOWNLOADING   ", LINE2(0));

        puts("# TIME     EV SP\r\n");

        for (int i = 0; i < event_count; i++)
        {
            addr = i * 10;

            line[0] = i + '0';
            line[1] = ' ';

            line[2]  = read_external_eeprom(addr++);
            line[3]  = read_external_eeprom(addr++);
            line[4]  = ':';
            line[5]  = read_external_eeprom(addr++);
            line[6]  = read_external_eeprom(addr++);
            line[7]  = ':';
            line[8]  = read_external_eeprom(addr++);
            line[9]  = read_external_eeprom(addr++);

            line[10] = ' ';
            line[11] = read_external_eeprom(addr++);
            line[12] = read_external_eeprom(addr++);

            line[13] = ' ';
            line[14] = read_external_eeprom(addr++);
            line[15] = read_external_eeprom(addr++);

            line[16] = '\0';

            puts(line);
            puts("\r\n");
        }

        clcd_print(" DOWNLOAD DONE ", LINE2(0));
        once = 0;
        delay = 0;
    }

    if(delay++ > 20000)
    {
        exit_down_log = 1;
        once = 1;
        delay = 0;
    }
}

/* -------------------------------------------------
 * CHANGE PASSWORD FUNCTION
 * ------------------------------------------------- */
void change_pass(unsigned char key)
{
    static char arr1[5], arr2[5];
    static unsigned int i = 0, j = 0;
    static unsigned char once1 = 1, once2 = 1;
    static unsigned int prev_sec = 0, prev_sec2 = 0;
    static unsigned int res_time = 0;
    static int flag = 0;
    static char str[17] = "                ";
    static unsigned char prev_key = 0;

    if (key == ALL_RELEASED)
        prev_key = ALL_RELEASED;

    if(flag == 0)
    {
        if(once1)
        {
            clcd_print(" ENTER PASSWORD ", LINE1(0));
            once1 = 0;
        }

        if(i == 0)
            clcd_print(str, LINE2(0));

        if(i < 4)
        {
            if(sec != prev_sec)
            {
                prev_sec = sec;
                clcd_putch((sec % 2) ? ' ' : 0xFF, LINE2(i + 3));
            }

            if ((key == SW1 || key == SW2) && key != prev_key)
            {
                prev_key = key;
                clcd_putch('*', LINE2(i + 3));
                arr1[i++] = (key == SW1) ? '0' : '1';
            }
        }

        if (i == 4)
            arr1[4] = '\0';

        if(i == 4 && j == 0)
            prev_sec2 = sec;

        if(i == 4 && j < 4)
        {
            if(once2)
            {
                clcd_print("RE-ENTER PASSWD ", LINE1(0));
                once2 = 0;
            }

            if(j == 0)
                clcd_print(str, LINE2(0));

            if(sec != prev_sec2)
            {
                prev_sec2 = sec;
                clcd_putch((sec % 2) ? ' ' : 0xFF, LINE2(j + 3));
            }

            if ((key == SW1 || key == SW2) && key != prev_key)
            {
                prev_key = key;
                clcd_putch('*', LINE2(j + 3));
                arr2[j++] = (key == SW1) ? '0' : '1';
            }
        }

        if (j == 4)
        {
            arr2[4] = '\0';
            flag = 1;
        }
    }

    if(flag == 1)
    {
        if(my_strcmp(arr1, arr2) == 0)
        {
            my_strcpy(pass, arr1);
            clcd_print("PASSWD IS CHANGED", LINE1(0));
        }
        else
        {
            clcd_print(" WRONG PASSWORD ", LINE1(0));
        }

        clcd_print("                 ", LINE2(0));

        if (res_time == 0)
            res_time = sec;

        if((sec - res_time) >= 2)
        {
            exit_ch_pass = 1;
            flag = 0;
            i = j = 0;
            once1 = once2 = 1;
            res_time = 0;
            prev_key = ALL_RELEASED;
        }
    }
}

/* -------------------------------------------------
 * SET TIME FUNCTION
 * ------------------------------------------------- */
void set_time(unsigned char key)
{
    static unsigned char time[3];
    static unsigned int hr, min, sec;
    static unsigned int rtc_sec;
    static int once = 1;
    static time_field_t field = SET_HR;
    static unsigned int prev_sec = 0;
    static unsigned char prev_key = ALL_RELEASED;

    if (once)
    {
        get_time(time);

        sec = ((time[2] & 0x70) >> 4) * 10 + (time[2] & 0x0F);
        min = ((time[1] & 0x70) >> 4) * 10 + (time[1] & 0x0F);
        hr  = ((time[0] & 0x30) >> 4) * 10 + (time[0] & 0x0F);

        once = 0;
    }

    get_time(time);
    rtc_sec = ((time[2] & 0x70) >> 4) * 10 + (time[2] & 0x0F);

    if (key == ALL_RELEASED)
        prev_key = ALL_RELEASED;

    if (key == SW2 && key != prev_key)
    {
        prev_key = key;
        field = (field == SET_HR) ? SET_MIN :
                (field == SET_MIN) ? SET_SEC : SET_HR;
    }

    if (key == SW1 && key != prev_key)
    {
        prev_key = key;
        if (field == SET_HR)      hr  = (hr + 1) % 24;
        else if (field == SET_MIN) min = (min + 1) % 60;
        else                       sec = (sec + 1) % 60;
    }

    clcd_print(" SET TIME       ", LINE1(0));

    clcd_putch((hr / 10) + '0', LINE2(3));
    clcd_putch((hr % 10) + '0', LINE2(4));
    clcd_putch(':', LINE2(5));
    clcd_putch((min / 10) + '0', LINE2(6));
    clcd_putch((min % 10) + '0', LINE2(7));
    clcd_putch(':', LINE2(8));
    clcd_putch((sec / 10) + '0', LINE2(9));
    clcd_putch((sec % 10) + '0', LINE2(10));

    if (rtc_sec != prev_sec)
    {
        prev_sec = rtc_sec;
        unsigned char pos = (field == SET_HR) ? 3 :
                            (field == SET_MIN) ? 6 : 9;
        clcd_putch((rtc_sec % 2) ? ' ' : 0xFF, LINE2(pos));
    }

    if (key == SW4)
    {
        write_ds1307(HOUR_ADDR, ((hr / 10) << 4) | (hr % 10));
        write_ds1307(MIN_ADDR,  ((min / 10) << 4) | (min % 10));
        write_ds1307(SEC_ADDR,  ((sec / 10) << 4) | (sec % 10));

        operation = 0;
        once = 1;
        field = SET_HR;
        prev_key = ALL_RELEASED;
    }

    if(key == SW5)
    {
        operation = 0;
        once = 1;
        field = SET_HR;
        prev_key = ALL_RELEASED;
        return;
    }
}
