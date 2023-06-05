#include "../include/control.h"

extern float inclination_angle;
float control = 0;
float error = 0;

void controlLoopInit(void) {

    TCCR1B = (1 << CS11); // prescaler 8
    TCNT1 = 0xD8F0;
    TIMSK1 = (1 << TOIE1);
}

void pidControl(float angle) {

    static float error_integral = 0, error_diff = 0, error_prev = 0, setpoint_angle = ANGLE_SETPOINT;

    // variate setpoint angle
    if(angle < setpoint_angle)
        setpoint_angle += ANGLE_FIXRATE * TIME_CONSTANT;
    else
        setpoint_angle -= ANGLE_FIXRATE * TIME_CONSTANT;

    // pid coeffs calculation
    error = setpoint_angle - angle;
    error_integral += error * TIME_CONSTANT;
    error_diff = (error - error_prev) / TIME_CONSTANT;

    if(error_integral > ERROR_SUM)
        error_integral = ERROR_SUM;

    if(error_integral < -ERROR_SUM)
        error_integral = -ERROR_SUM;

    error_prev = error;

    control = (P_COEF * error) + (I_COEF * error_integral) + (D_COEF * error_diff);

    if(control > 255)
        control = 255;
    if(control < -255)
        control = -255;

    if((angle >= 8.0) || (angle <= -8.0)) {
        setpoint_angle = ANGLE_SETPOINT;
        error = 0;
        error_integral = 0;
        error_diff = 0;
        error_prev = 0;
        motorStop();
    } else {
        if(control >= 0)
            motorRun((uint8_t)control, CCW);
        else {
            control = -control;
            motorRun((uint8_t)control, CW);
        }
    }
}

ISR(TIMER1_OVF_vect) { // period = 5 ms frq = 200 Hz

    TCNT1 = 0xD8F0;
    static uint8_t t1_counter = 0;
    t1_counter++;

    if(t1_counter == 0)
        PORTB ^= (1 << 5);

    mpuGetAngle();
    pidControl(inclination_angle);
}