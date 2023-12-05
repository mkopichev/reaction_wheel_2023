#include "../include/control.h"

extern float inclinationAngle;
extern int16_t encoderFullRevolutions;
extern int16_t encoderPulseCounter;
float control = 0;
float error = 0;
int16_t encoderMotorSpeed = 0;

int16_t prevPos = 0;
int16_t currentPos = 0;

void controlLoopInit(void) {

    TCCR1B = (1 << CS11); // prescaler 8
    TCNT1 = 0xD8F0;
    TIMSK1 = (1 << TOIE1);
}

void pidControl(float angle) {

    static float errorIntegral = 0, errorDiff = 0, errorPrev = 0, setpointAngle = ANGLE_SETPOINT;

    // variate setpoint angle
    if(angle < setpointAngle)
        setpointAngle += ANGLE_FIXRATE * TIME_CONSTANT;
    else
        setpointAngle -= ANGLE_FIXRATE * TIME_CONSTANT;

    // reduce continuous rotation
    setpointAngle -= ANGLE_FIXRATE_2 * encoderMotorSpeed * TIME_CONSTANT;

    // pid coeffs calculation
    error = setpointAngle - angle;
    errorIntegral += error * TIME_CONSTANT;
    errorDiff = (error - errorPrev) / TIME_CONSTANT;

    // if(errorIntegral > ERROR_SUM)
    //     errorIntegral = ERROR_SUM;

    // if(errorIntegral < -ERROR_SUM)
    //     errorIntegral = -ERROR_SUM;

    errorPrev = error;

    control = (-MOTOR_R * ((P_COEF * error) + (I_COEF * errorIntegral) + (D_COEF * errorDiff))) + (P_SPEED_COEF * encoderMotorSpeed);

    if(control > 255)
        control = 255;
    if(control < -255)
        control = -255;

    if((angle >= ANGLE_END_LIMIT) || (angle <= -ANGLE_END_LIMIT)) {
        error = 0;
        errorIntegral = 0;
        errorDiff = 0;
        errorPrev = 0;
        motorStop();
    } else {
        if(control >= 0)
            motorRun((uint8_t)control, CCW);
        else {
            motorRun((uint8_t)-control, CW);
        }
    }
}

ISR(TIMER1_OVF_vect) { // period = 5 ms frq = 200 Hz

    TCNT1 = 0xD8F0;

    static uint8_t t1_counter = 0;
    t1_counter++;

    if(t1_counter == 0)
        PORTB ^= (1 << 5);

    currentPos = (encoderFullRevolutions * ENCODER_PPR) + encoderPulseCounter;
    encoderMotorSpeed = currentPos - prevPos;
    prevPos = currentPos;

    mpuGetAngle();
    pidControl(inclinationAngle);
}