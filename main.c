#include "include/utils.h"

extern float inclination_angle, control, error;

void initAll(void);

int main(void) {

    initAll();

    while(1) {

        uartTransmitDec((int16_t)(inclination_angle * 10));
        uartTransmitByte('\t');
        uartTransmitDec((int16_t)(error * 100));
        uartTransmitByte('\t');
        uartTransmitDec((int16_t)(control * 1));
        uartTransmitByte('\r');
        uartTransmitByte('\n');
        _delay_ms(5);
    }
}

void initAll(void) {

    DDRB |= (1 << 5);
    uartInit();
    twiInit();
    mpuInit();
    motorInit();
    controlLoopInit();
    _delay_ms(100);
    sei();
}