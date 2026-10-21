#include "header.h"

void uart0_init(unsigned int baud)
{
    int result, pclk;

    if (VPBDIV == 1)
        pclk = 60000000;
    else if (VPBDIV == 2)
        pclk = 30000000;
    else
        pclk = 15000000;

    result = pclk / (16 * baud);
    PINSEL0 |= 0X5;
    U0LCR = 0X83;
    U0DLL = result & 0xff;
    U0DLM = (result >> 8) & 0xff;
    U0LCR = 0X03;
}

void uart0_tx(unsigned char data)
{
    U0THR = data;
    while (THRE == 0)
        ;
}

unsigned char uart0_rx(void)
{
    while (RDR == 0)
        ;
    return U0RBR;
}

void uart0_string(unsigned char *p)
{
    while (*p) {
        uart0_tx(*p);
        p++;
    }
}

void uart0_integer(unsigned int num)
{
    int i, a[10];

    for (i = 0; num; num = num / 10)
        a[i++] = num % 10;
    i--;

    for (; i >= 0; i--)
        uart0_tx(a[i] + 48);
}

void uart0_rx_string(unsigned char *p, int len)
{
    int i;

    for (i = 0; i < len; i++) {
        while (RDR == 0)
            ;
        p[i] = U0RBR;
        if (p[i] == '\r')
            break;
    }
    p[i] = '\0';
}
