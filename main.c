#include "include/utils.h"

extern float inclinationAngle, control, error;
extern int16_t encoderMotorSpeed;

void initAll(void);

int main(void) {

    initAll();

    while(1) {

        // uartTransmitByte(42);
        // uartTransmitMultipleData(&control, 4);
        // uartTransmitMultipleData(&inclinationAngle, 4);
        uartTransmitDec((int16_t)control);
        uartTransmitStr("\t");
        uartTransmitDec((int16_t)inclinationAngle);
        uartTransmitStr("\t");
        uartTransmitDec(encoderMotorSpeed);
        uartTransmitStr("\r\n");
        _delay_ms(20);
    }
}

void initAll(void) {

    DDRB |= (1 << 5);
    uartInit();
    twiInit();
    mpuInit();
    motorInit();
    encoderInit();
    controlLoopInit();
    _delay_ms(100);
    sei();
}