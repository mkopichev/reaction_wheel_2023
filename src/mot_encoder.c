#include "../include/mot_encoder.h"

int16_t encoder_pulse_counter = 0;
int16_t encoder_full_revolution = 0;

void encoderInit(void) {

    EICRA = (1 << ISC00);
    EIMSK = (1 << INT0);
}

ISR(INT0_vect) {

    if(PIND & (1 << 2)) {
        if(PIND & (1 << 3))
            encoder_pulse_counter++;
        else
            encoder_pulse_counter--;
    } else {
        if(PIND & (1 << 3))
            encoder_pulse_counter--;
        else
            encoder_pulse_counter++;
    }

    if(encoder_pulse_counter >= ENCODER_PPR) {
        encoder_pulse_counter -= ENCODER_PPR;
        encoder_full_revolution++;
    }
    if(encoder_pulse_counter <= -ENCODER_PPR) {
        encoder_pulse_counter += ENCODER_PPR;
        encoder_full_revolution--;
    }
}