 #include <xc.h>
#include "main.h"

unsigned char clock_reg[3];
unsigned char speed[3] = {0};

char *event[] = { "ON", "GN", "G1", "G2", "G3", "G4", "GR", "C " };

int pos = 0;
static int prev_pos = -1;

unsigned int adc_val;
char time[9];
char line1[17];
char line2[17];

void dashboard(unsigned char key)
{
    /* Exit dashboard */
    if (operation != 0)
    {
        prev_pos = -1;
        clcd_clear_screen();
        return;
    }

    /* Time */
    get_time(clock_reg);
    display_time(clock_reg);
    store_time(time, clock_reg);

    /* Event / Gear */
    if (key == SW1)
        pos = 7;
    else if (key == SW2)
    {
        if (pos < 6)
            pos++;
    }
    else if (key == SW3)
    {
        if (pos > 1)
            pos--;
    }

    /* Speed */
    adc_val = (unsigned int)(read_adc(CHANNEL0) / 10.24);
    speed[0] = (adc_val / 10) + '0';
    speed[1] = (adc_val % 10) + '0';

    /* Log event */
    if (pos != prev_pos)
    {
        store_event(event[pos], time, speed);
        prev_pos = pos;
    }

    /* LCD line 1 */
    line1[0]  = ' ';
    line1[1]  = 'T';
    line1[2]  = 'I';
    line1[3]  = 'M';
    line1[4]  = 'E';
    line1[5]  = ' ';
    line1[6]  = ' ';
    line1[7]  = ' ';
    line1[8]  = ' ';
    line1[9]  = ' ';
    line1[10] = 'E';
    line1[11] = 'V';
    line1[12] = ' ';
    line1[13] = ' ';
    line1[14] = 'S';
    line1[15] = 'P';
    line1[16] = '\0';

    /* LCD line 2 */
    line2[0]  = time[0];
    line2[1]  = time[1];
    line2[2]  = ':';
    line2[3]  = time[3];
    line2[4]  = time[4];
    line2[5]  = ':';
    line2[6]  = time[6];
    line2[7]  = time[7];
    line2[8]  = ' ';
    line2[9]  = ' ';
    line2[10] = event[pos][0];
    line2[11] = event[pos][1];
    line2[12] = ' ';
    line2[13] = ' ';
    line2[14] = speed[0];
    line2[15] = speed[1];
    line2[16] = '\0';

    clcd_print(line1, LINE1(0));
    clcd_print(line2, LINE2(0));
}
