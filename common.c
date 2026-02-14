 #include <xc.h>
#include "main.h"

/* Compare two strings */
int my_strcmp(const char *str1, const char *str2)
{
    while (*str1 && (*str1 == *str2))
    {
        str1++;
        str2++;
    }
    return *(unsigned char *)str1 - *(unsigned char *)str2;
}

/* Convert RTC BCD time to "HH:MM:SS" format */
void store_time(char *time, unsigned char *clock_reg)
{
    time[0] = ((clock_reg[0] >> 4) & 0x03) + '0';   // Hour tens
    time[1] = (clock_reg[0] & 0x0F) + '0';          // Hour units

    time[2] = ':';                                 // Separator

    time[3] = ((clock_reg[1] >> 4) & 0x07) + '0';   // Minute tens
    time[4] = (clock_reg[1] & 0x0F) + '0';          // Minute units

    time[5] = ':';                                 // Separator

    time[6] = ((clock_reg[2] >> 4) & 0x07) + '0';   // Second tens
    time[7] = (clock_reg[2] & 0x0F) + '0';          // Second units
    time[8] = '\0';                                // String terminator
}

/* Clear both lines of the LCD */
void clcd_clear_screen(void)
{
    clcd_print("                ", LINE1(0));
    clcd_print("                ", LINE2(0));
}

/* Copy source string to destination */
void my_strcpy(char *dst, const char *src)
{
    while ((*dst++ = *src++) != '\0');
}
