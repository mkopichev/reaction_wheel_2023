#include "include/utils.h"

void initAll(void);

int main(void) {

    initAll();
    while(1) {
        mpuPrintAngle();
        _delay_ms(100);
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