#include "header.h"

u32 t_sample[6];

void i2c_init(void)
{
    PINSEL0 |= 0X50;                 // P0.2, P0.3
    I2CONSET = (1 << 6);             // AA=0, I2EN=1; select master mode
    I2SCLL = I2SCLH = 75;
}

void i2c_send(u8 SA, u8 MR, u8 DATA)
{
    // Step 1: Generate the start condition
    I2CONSET = (1 << 5);             // STA=1
    I2CONCLR = (1 << 3);             // SI=0
    while (SI == 0)
        ;
    I2CONCLR = (1 << 5);

    // Step 2: Send SA+W
    I2DAT = SA;
    I2CONCLR = (1 << 3);
    while (SI == 0)
        ;
    if (I2STAT == 0x20) {
        uart0_string("ERR: SA+w sent no ack\\r\\n");
        goto exit;
    }

    // Step 3: Send MR and check ACK
    I2DAT = MR;
    I2CONCLR = (1 << 3);
    while (SI == 0)
        ;
    if (I2STAT == 0x30) {
        uart0_string("ERR: Mr sent, no ack\\r\\n");
        goto exit;
    }

    // Step 4: Send data and check ACK
    I2DAT = DATA;
    I2CONCLR = (1 << 3);
    while (SI == 0)
        ;
    if (I2STAT == 0x30) {
        uart0_string("ERR: data sent, no ack");
        goto exit;
    }

    // Step 5: Generate the stop condition
exit:
    I2CONSET = (1 << 4);
    I2CONCLR = (1 << 3);
}

void i2c_sample_receive(u8 SA, u8 MR)
{
    int i;
    u8 temp;

    // Step 1: Generate start condition
    I2CONSET = (1 << 5);             // STA=1
    I2CONCLR = (1 << 3);             // SI=0
    while (SI == 0)
        ;
    I2CONCLR = (1 << 5);             // STA=0

    // Step 2: Send SA+W and check ACK
    I2DAT = SA ^ 1;
    I2CONCLR = (1 << 3);
    while (SI == 0)
        ;
    if (I2STAT == 0x20) {
        uart0_string("ERR: SA+W AND NO ACK");
        goto exit;
    }

    // Step 3: Send MR address and check ACK
    I2DAT = MR;
    I2CONCLR = (1 << 3);
    while (SI == 0)
        ;
    if (I2STAT == 0x30) {
        uart0_string("ERR: MR AND no ack");
        goto exit;
    }

    // Step 4: Generate restart condition
    I2CONSET = (1 << 5);             // STA=1
    I2CONCLR = (1 << 3);
    while (SI == 0)
        ;
    I2CONCLR = (1 << 5);             // STA=0

    // Step 5: Send SA+R and check ACK
    I2DAT = SA;
    I2CONCLR = (1 << 3);
    while (SI == 0)
        ;
    if (I2STAT == 0x48) {
        uart0_string("ERR: SA+R AND NO ACK");
        goto exit;
    }

    // Step 6: Read six successive bytes: five with ACK and one with NACK
    for (i = 0; i < 5; i++) {
        // Get data and send ACK
        I2CONSET = (1 << 2);
        I2CONCLR = (1 << 3);
        while (SI == 0)
            ;
        if (I2STAT == 0x58) {
            uart0_string("ERR: DATA received AND NO ACK");
            goto exit;
        }
        if (I2STAT != 0x50) {
            uart0_string("ERR: DATA received AND returned ACK");
            goto exit;
        }
        temp = I2DAT;
        t_sample[i] = (u32)temp;
    }

    // Last byte: NACK
    I2CONCLR = (1 << 2);
    I2CONCLR = (1 << 3);
    while (SI == 0)
        ;
    if (I2STAT != 0x58) {
        uart0_string("ERR: DATA received AND NO ACK");
        goto exit;
    }
    temp = I2DAT;
    t_sample[i] = (u32)temp;

    // Step 7: Generate stop condition
exit:
    I2CONSET = (1 << 4);
    I2CONCLR = (1 << 3);
}

u8 i2c_receive(u8 SA, u8 MR)
{
    u8 temp;

    // Step 1: Generate start condition
    I2CONSET = (1 << 5);             // STA=1
    I2CONCLR = (1 << 3);             // SI=0
    while (SI == 0)
        ;
    I2CONCLR = (1 << 5);             // STA=0

    // Step 2: Send SA+W and check ACK
    I2DAT = SA ^ 1;
    I2CONCLR = (1 << 3);
    while (SI == 0)
        ;
    if (I2STAT == 0x20) {
        uart0_string("ERR: SA+W AND NO ACK");
        goto exit;
    }

    // Step 3: Send MR address and check ACK
    I2DAT = MR;
    I2CONCLR = (1 << 3);
    while (SI == 0)
        ;
    if (I2STAT == 0x30) {
        uart0_string("ERR: MR AND no ack");
        goto exit;
    }

    // Step 4: Generate restart condition
    I2CONSET = (1 << 5);             // STA=1
    I2CONCLR = (1 << 3);
    while (SI == 0)
        ;
    I2CONCLR = (1 << 5);             // STA=0

    // Step 5: Send SA+R and check ACK
    I2DAT = SA;
    I2CONCLR = (1 << 3);
    while (SI == 0)
        ;
    if (I2STAT == 0x48) {
        uart0_string("ERR: SA+R AND NO ACK");
        goto exit;
    }

    // Step 6: Read data
    I2CONCLR = (1 << 3);
    while (SI == 0)
        ;
    temp = I2DAT;

    // Step 7: Generate stop condition
exit:
    I2CONSET = (1 << 4);
    I2CONCLR = (1 << 3);

    // Return
    return temp;
}
