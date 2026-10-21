#include "header.h"

extern float spo2, temprature;
extern u32 bpm;

int main()
{
    u8 t, s, b, t1, s1, b1;

    uart0_init(9600);
    lcd_init();
    adc_init();
    i2c_init();
    max_spo2_setting();

    uart0_string("V25be9v1\\r\\n");
    uart0_string("Patient Monitoring started\\r\\n");

    while (1) {
        data_representation();

        uart0_string("data represemtion completed\\r\\n");
        uart0_string("\\r\\ncompleted\\r\\n");

        t = (u8)temprature;
        i2c_send(0xA0, 0x00, t);
        delay_ms(40);

        s = (u8)spo2;
        i2c_send(0xA0, 0x01, s);
        delay_ms(40);

        b = (u8)bpm;
        i2c_send(0xA0, 0x02, b);
        delay_ms(40);

        t1 = i2c_receive(0xA1, 0x0);
        s1 = i2c_receive(0xA1, 0x1);
        b1 = i2c_receive(0xA1, 0x2);

        if ((t1 == t) && (s == s1) && (b == b1))
            uart0_string("\\r\\nstored\\r\\n");
        else
            uart0_string("\\r\\n not stored\\r\\n");
    }
}
