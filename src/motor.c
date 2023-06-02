#include "../include/motor.h"

void motorInit(void) {

    DDRD |= (1 << 7) | (1 << 6) | (1 << 5) | (1 << 4);
    TCCR0A = (1 << COM0A1) | (1 << COM0B1) | (1 << WGM00) | (1 << WGM01);
    TCCR0B = (1 << CS01); // fast PWM ~8kHz
}

void motorStop(void) {

    PORTD &= ~(1 << 7);
    OCR0A = 0;
    PORTD &= ~(1 << 4);
    OCR0A = 0;
}

void motorRun(uint8_t speed, bool direction) {

    if(speed) {
        if(direction) {
            PORTD |= (1 << 7);
            OCR0A = speed;
            PORTD |= (1 << 4);
            OCR0B = 0;
        } else {
            PORTD |= (1 << 7);
            OCR0A = 0;
            PORTD |= (1 << 4);
            OCR0B = speed;
        }
    } else
        motorStop();
}