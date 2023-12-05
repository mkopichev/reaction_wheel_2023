#include "include/utils.h"

extern float inclination_angle, control, error;
extern int16_t encoder_motor_speed;

void initAll(void);

int main(void) {

    initAll();

    while(1) {

        // uartTransmitByte(42);
        // uartTransmitMultipleData(&control, 4);
        // uartTransmitMultipleData(&inclination_angle, 4);
        uartTransmitDec((int16_t)control);
        uartTransmitStr("\t");
        uartTransmitDec((int16_t)inclination_angle);
        uartTransmitStr("\t");
        uartTransmitDec(encoder_motor_speed);
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