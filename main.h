 #ifndef MAIN_H
#define	MAIN_H
#include "adc.h"
#include "digital_keypad.h"
#include "clcd.h"
#include "i2c.h"
#include "ds1307.h"
#include "Timer.h"
#include "external_eeprom.h"
#include "uart.h"

extern unsigned char exit_view_log;
extern unsigned char exit_clear_log;
extern unsigned char exit_down_log;
extern unsigned char exit_ch_pass;
volatile extern unsigned int sec;
extern char pass[5];

extern unsigned char operation;
extern unsigned char event_count;
extern unsigned int log_write_addr;
void dashboard(unsigned char key);
void login_screen(unsigned char key);
void main_menu(unsigned char key);
void store_event(char *event,char *time,unsigned char *speed);
int my_strcmp(const char *str1,const char *str2);
void my_strcpy(char *str1,const char *str2);
void store_time(char*time,unsigned char *clock_reg);
void view_log(unsigned char key);
void down_log(void);
void clcd_clear_screen(void);
void clear_log(void);
void set_time(unsigned char key);
void change_pass(unsigned char key);
typedef enum
{
    SET_HR,
    SET_MIN,
    SET_SEC
} time_field_t;


#endif	/* MAIN_H */

