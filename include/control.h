#ifndef CONTROL_H_
#define CONTROL_H_

#include "utils.h"

#define ANGLE_SETPOINT 0.0F
#define ANGLE_END_LIMIT 7.0F

#define ANGLE_FIXRATE   1.0F
#define ANGLE_FIXRATE_2 0.1F

#define ERROR_SUM 2.0F

// #define P_COEF 15.0F
// #define I_COEF 0.4F
// #define D_COEF 0.4F

#define P_COEF       0.5F
#define I_COEF       1.5F
#define D_COEF       0.001F
#define P_SPEED_COEF 0.5F

#define TIME_CONSTANT 0.005F

#define ANGLE_FILTER_GYRO 0.98F

#define MOTOR_R 14.0F

void controlLoopInit(void);
void pidControl(float angle);

#endif