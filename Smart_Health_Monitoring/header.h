#include <lpc21xx.h>
#include <stdio.h>
#define RDR (U0LSR&1)
#define THRE ((U0LSR>>5)&1)
#define DONE ((ADDR>>31)&1)
#define buffer_size 100
typedef unsigned int u32;
typedef unsigned char u8;
#define SI ((I2CONSET>>3)&1)
extern u32 threshold(u32 *a);
extern void bpm_cal(void);
extern void get_smooth(u32 *u);
extern void data_representation(void);
void lm35_cal(void);
extern void uart0_tx(unsigned char data);
extern unsigned char uart0_rx(void);
extern void uart0_init(unsigned int baud);
extern void uart0_string(unsigned char *p);
extern void uart0_rx_string(unsigned char *p,int len);
extern int adc_read(int ch_num);
extern void adc_init(void);
extern void uart0_integer(unsigned int num);
extern void lcd_data(unsigned char data);
extern void lcd_cmd(unsigned char cmd);
extern void lcd_string(unsigned char *p);
extern void lcd_init(void);
extern void delay_sec(unsigned int sec);
extern void delay_ms(unsigned int ms);
extern void i2c_init(void);
extern void i2c_send(u8, u8, u8);
extern u8 i2c_receive(u8, u8);
extern void max_spo2_setting(void);
extern void spo2_bpm(void);
extern void i2c_sample_receive(u8, u8);
extern u32 diff(u32 *);
extern void spo2_cal(void);
