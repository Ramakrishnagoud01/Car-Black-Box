 #include <xc.h>
#include "main.h"

unsigned char event_count = 0;
unsigned int log_write_addr = 0;

void store_event(char *event, char *time, unsigned char *speed)
{
    unsigned int address = log_write_addr;

    /* Time */
    write_external_eeprom(address++, time[0]);
    write_external_eeprom(address++, time[1]);
    write_external_eeprom(address++, time[3]);
    write_external_eeprom(address++, time[4]);
    write_external_eeprom(address++, time[6]);
    write_external_eeprom(address++, time[7]);

    /* Event */
    write_external_eeprom(address++, event[0]);
    write_external_eeprom(address++, event[1]);

    /* Speed */
    write_external_eeprom(address++, speed[0]);
    write_external_eeprom(address++, speed[1]);

    /* Log management */
    if (log_write_addr >= 100)
        log_write_addr = 0;

    if (event_count < 10)
        event_count++;

    log_write_addr += 10;

    if (log_write_addr >= 100)
        log_write_addr = 0;
}
