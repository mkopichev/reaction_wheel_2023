#include "../include/control.h"

extern float inclination_angle;

void controlLoopInit(void) {

    TCCR1B = (1 << CS10);  // prescaler 1
    TCNT1 = 0x600;         
    TIMSK1 = (1 << TOIE1);
}

void pidControl(float angle) {

    static float error = 0, error_integral = 0, error_diff = 0, error_prev = 0;

    error = SETPOINT - angle;
    error_integral += error * TIME_CONSTANT;
    error_diff = (error_prev - error) / TIME_CONSTANT;

    error_prev = error;

    float control = (P_COEF * error) + (I_COEF * error_integral) + (D_COEF * error_diff);

    if(control > 255)
        control = 255;
    if(control < -255)
        control = -255;

    if((angle >= 8.0) || (angle <= -8.0))
        motorStop();
    else {
        if(control >= 0)
            motorRun((uint8_t)control, CCW);
        else {
            control = -control;
            motorRun((uint8_t)control, CW);
        }
    }
}

ISR(TIMER1_OVF_vect) { // period = 4 ms frq = 250 Hz

    TCNT1 = 0x0600;
    static uint8_t t1_counter = 0;
    t1_counter++;

    if(t1_counter == 0)
        PORTB ^= (1 << 5);

    mpuGetAngle();
    pidControl(inclination_angle);
}