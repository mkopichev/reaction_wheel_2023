#include "include/utils.h"

extern float inclination_angle, control, error;

void initAll(void);

int main(void) {

    initAll();

    while(1) {

        uartTransmitByte(42);
        uartTransmitMultipleData(&control, 4);
        uartTransmitMultipleData(&inclination_angle, 4);
        _delay_ms(20);
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