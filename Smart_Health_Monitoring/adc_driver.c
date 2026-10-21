#include "header.h"

int adc_read(int ch_num)
{
    unsigned int result;

    ADCR |= 1 << ch_num;   // Select channel number
    ADCR |= 1 << 24;       // Set bit 24 to start ADC

    while (DONE == 0)
        ;

    ADCR &= ~(1 << 24);
    ADCR &= ~(1 << ch_num);

    result = (ADDR >> 6) & 0x3FF;
    return result;
}

void adc_init(void)
{
    PINSEL1 |= 0x15400000;
    ADCR |= 0x200400;
}
