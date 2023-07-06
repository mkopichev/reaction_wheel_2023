#include "../include/twi.h"

uint8_t twi_mutex = VACANT;

void twiInit(void) {

    TWBR = (uint8_t)((F_CPU / SCL_FREQ) - 16) / 2;
}

void twiStart(void) {

    TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
    while(!(TWCR & (1 << TWINT)))
        continue;
}

void twiStop(void) {

    TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);
}

void twiTransmitByte(uint8_t data) {

    TWDR = data;
    TWCR = (1 << TWINT) | (1 << TWEN);
    while(!(TWCR & (1 << TWINT)))
        continue;
}

uint8_t twiReceiveByte(bool is_last_byte) {

    if(is_last_byte)
        TWCR = (1 << TWINT) | (1 << TWEN);
    else
        TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWEA);
    while(!(TWCR & (1 << TWINT)))
        continue;
    return (TWDR);
}

void twiWriteData(uint8_t *tx_buf, uint8_t length) {

    for(uint8_t i = 0; i < length; i++)
        twiTransmitByte(*tx_buf++);
}

void twiReadData(uint8_t *rx_buf, uint8_t length) {

    for(uint8_t i = 0; i < length; i++) {
        if(i < (length - 1))
            *rx_buf++ = twiReceiveByte(false);
        else
            *rx_buf++ = twiReceiveByte(true);
    }
}

void twiAcquire(void) {

    while(1) {
        if(twi_mutex == VACANT) {
            twi_mutex = OCCUPIED;
            break;
        }
    }
}

void twiRelease(void) {
    
    twi_mutex = VACANT;
}