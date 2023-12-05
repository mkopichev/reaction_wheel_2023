#include "../include/mot_encoder.h"

int16_t encoderPulseCounter = 0;
int16_t encoderFullRevolutions = 0;

void encoderInit(void) {

    EICRA = (1 << ISC00);
    EIMSK = (1 << INT0);
}

ISR(INT0_vect) {

    if(PIND & (1 << 2)) {
        if(PIND & (1 << 3))
            encoderPulseCounter++;
        else
            encoderPulseCounter--;
    } else {
        if(PIND & (1 << 3))
            encoderPulseCounter--;
        else
            encoderPulseCounter++;
    }

    if(encoderPulseCounter >= ENCODER_PPR) {
        encoderPulseCounter -= ENCODER_PPR;
        encoderFullRevolutions++;
    }
    if(encoderPulseCounter <= -ENCODER_PPR) {
        encoderPulseCounter += ENCODER_PPR;
        encoderFullRevolutions--;
    }
}