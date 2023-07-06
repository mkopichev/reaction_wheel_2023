#include "../include/uart.h"

// #define INTERRUPT
#define NO_INTERRUPT

void uartInit(void) {

#ifdef NO_INTERRUPT
    UCSR0B |= (1 << RXEN0) | (1 << TXEN0);
#endif
#ifdef INTERRUPT
    UCSR0B |= (1 << RXCIE0) | (1 << RXEN0) | (1 << TXEN0);
#endif
    UCSR0C |= (1 << UCSZ01) | (1 << UCSZ00);
    // 19200 baud
    UBRR0H = 0;
    // UBRR0L = 51 = 0x33
    UBRR0L = 0x33;
}

// transmit byte of data
void uartTransmitByte(uint8_t byte) {

    // waiting for UDRE0 set, then put data into UDR0
    while(!(UCSR0A & (1 << UDRE0)))
        continue;
    UDR0 = byte;
}

void uartTransmitMultipleData(void *buf, uint8_t length) {
    for(uint8_t i = 0; i < length; i++) {
        uartTransmitByte(*((uint8_t *)buf + i));
    }
}

// recieve byte of data
uint8_t uartReceiveByte(void) {

    // waiting for UDRE0 set, then get data from UDR0
    while(!(UCSR0A & (1 << RXC0)))
        continue;
    return UDR0;
}

// transmit string, phrase or word
void uartTransmitStr(char *string) {

    /* transmit data byte-by-byte with pointer *
    pointer – address of the first element of data package*/
    while(*string)
        uartTransmitByte(*string++);
}

// transmit decimal value from mk to terminal
void uartTransmitDec(int16_t val) {

    uint8_t i, j;
    // if 5-digit value is nonNull
    if(val != 0) {
        if(val < 0) {
            val = -val;
            uartTransmitByte('-');
        }
        /* check the number of zero digits in 5-digit value
        for instance value 64 has 3 insignificant zeroes - 00064 */
        j = 1;
        while(!(digit((uint16_t)val, j++)))
            continue;
        // eliminating insignificant zeroes
        for(i = j - 1; i <= 5; i++) uartTransmitByte(digit(val, i) + '0');
    }
    // if value is zero just return zero
    else
        uartTransmitByte('0');
}

// transmit binary value from mk to terminal
void uartTransmitBin(uint8_t val) {

    uartTransmitByte('b');
    if(val != 0) {
        for(uint8_t i = (1 << 7); i >= 1; i >>= 1) {
            if(val & i)
                uartTransmitDec(1);
            else
                uartTransmitDec(0);
        }
    }
    // if value is zero just return zero
    else
        uartTransmitDec(00000000);
}

// transmit hex value from mk to terminal
void uartTransmitHex(uint32_t val) {

    uint8_t msb = (val & 0xF0) >> 4, lsb = val & 0x0F;
    msb += msb > 9 ? 'A' - 10 : '0';
    lsb += lsb > 9 ? 'A' - 10 : '0';
    uartTransmitByte(msb);
    uartTransmitByte(lsb);
}

// transmit float value from mk to terminal
void uarTransmitFloat(float val) {

    char buffer[20], float_[10];
    dtostrf(val, 5, 3, float_);
    sprintf(buffer, "%s", float_);
    uartTransmitStr(buffer);
}

// recieve decimal value from terminal to mk
uint32_t uartReceiveDec(void) {

    uint32_t data = 0;
    uint8_t digit = 0;
    // recieving 5-digit value
    do {
        digit = uartReceiveByte();
        // recieve until \r\n reveals
        if(digit == '\r')
            continue;
        if(digit == '\n')
            break;
        if(!((digit >= '0') && (digit <= '9')))
            break;
        // showing ASCII codes of digits
        digit -= '0';
        data *= 10;
        data += digit;
    }
    // finish when \r\n appears
    while(1);
    return data;
}
