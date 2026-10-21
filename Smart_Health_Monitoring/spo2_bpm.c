#include "header.h"

extern u32 t_sample[6];

u32 red[buffer_size], ir[buffer_size], sample_count, first_peak, second_peak;
float spo2, temprature;
u32 bpm;

void spo2_bpm(void)
{
    u8 status, s[100];
    u32 t_red, t_ir, i;

    for (i = 0; i < buffer_size;) {
        status = i2c_receive(0xAF, 0x00);
        if (status & 0x40) {
            i2c_sample_receive(0xAF, 0x07);
            t_red = ((t_sample[0] << 16) | (t_sample[1] << 8) | (t_sample[2]));
            t_red = t_red & 0x3ffff;
            t_ir = ((t_sample[3] << 16) | (t_sample[4] << 8) | (t_sample[5]));
            t_ir = t_ir & 0x3ffff;

            red[i] = t_red;
            ir[i] = t_ir;
            i++;
        }
    }
    sample_count++;
}

void bpm_cal(void)
{
    u32 th, i;

    th = threshold(ir);
    for (i = 0; i < buffer_size; i++) {
        if (ir[i] > th) {
            if (first_peak == 0) {
                get_smooth(&i);
                first_peak = i;
            } else {
                get_smooth(&i);
                second_peak = i + sample_count * 100;
            }
        }
    }
}

void lm35_cal(void)
{
    u32 adc_value;
    float vout, TA;

    adc_value = adc_read(0);
    vout = (adc_value * 3.3) / 1023;
    TA = (vout * 100);
    temprature = TA;
}

void data_representation(void)
{
    u8 s[100];

    // First 100 samples
    spo2_bpm();
    bpm_cal();
    spo2_cal();

    // If peak is not detected, another 100 samples
    if (first_peak == 0) {
        spo2_bpm();
        bpm_cal();
    }

    // If second peak is not detected, another 100 samples
    if (second_peak == 0) {
        spo2_bpm();
        bpm_cal();
    }

    // If peaks are detected
    if ((first_peak != 0) && (second_peak != 0)) {
        bpm = 6000 / (second_peak - first_peak);
    }

    // Data representation: SpO2
    sprintf(s, "spo2=%0.2f\r\n", spo2);
    uart0_string(s);

    // BPM
    sprintf(s, "bpm=%u\r\n", bpm);
    uart0_string(s);

    // Temperature
    lm35_cal();
    sprintf(s, "temp=%0.2f\r\n", temprature);
    uart0_string(s);

    // Reset
    first_peak = 0;
    second_peak = 0;
    sample_count = 0;
}

void get_smooth(u32 *i)
{
    for (; *i < (buffer_size - 1); ++(*i)) {
        if (ir[*i + 1] < ir[*i])
            break;
    }
}

u32 threshold(u32 *a)
{
    u32 i, l, s;

    if (a[0] > a[1]) {
        s = a[1];
        l = a[0];
    } else if (a[0] == a[1]) {
        s = l = a[0];
    } else {
        l = a[1];
        s = a[0];
    }

    for (i = 2; i < buffer_size; i++) {
        if (s > a[i])
            s = a[i];
        if (a[i] > l)
            l = a[i];
    }

    l = (l + s) / 2;
    return l;
}

void spo2_cal(void)
{
    float R;
    u32 ac_red, dc_red, ac_ir, dc_ir, i;
    unsigned long int sum;

    ac_red = diff(red);
    ac_ir = diff(ir);

    // Red average
    for (i = 0, sum = 0; i < buffer_size; i++)
        sum = sum + red[i];
    sum = sum / buffer_size;
    dc_red = sum;

    // IR average
    for (i = 0, sum = 0; i < buffer_size; i++)
        sum = sum + ir[i];
    sum = sum / buffer_size;
    dc_ir = sum;

    R = ((float)ac_red / (float)dc_red) / ((float)ac_ir / (float)dc_ir);
    spo2 = 110 - 25 * R;
}

void max_spo2_setting(void)
{
    i2c_send(0xAE, 0x09, 0x40);
    i2c_send(0xAE, 0x04, 0x00);
    i2c_send(0xAE, 0x05, 0x00);
    i2c_send(0xAE, 0x06, 0x00);
    i2c_send(0xAE, 0x08, 0x20);
    i2c_send(0xAE, 0x09, 0x03);
    i2c_send(0xAE, 0x0A, 0x47);
    i2c_send(0xAE, 0x0C, 0x24);
    i2c_send(0xAE, 0x0D, 0x24);
    i2c_send(0xAE, 0x02, 0x40);
}

u32 diff(u32 *a)
{
    int i, l, s;

    if (a[0] > a[1]) {
        s = a[1];
        l = a[0];
    } else if (a[0] == a[1]) {
        s = l = a[0];
    } else {
        l = a[1];
        s = a[0];
    }

    for (i = 2; i < buffer_size; i++) {
        if (s > a[i])
            s = a[i];
        if (a[i] > l)
            l = a[i];
    }

    return l - s;
}
