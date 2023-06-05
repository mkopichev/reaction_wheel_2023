#ifndef CONTROL_H_
#define CONTROL_H_

#include "utils.h"

#define ANGLE_SETPOINT 0.2F

#define ANGLE_FIXRATE 0.1F

#define ERROR_SUM 200.0F

#define P_COEF 35.4F
#define I_COEF 89.3F
#define D_COEF 0.0019F

#define TIME_CONSTANT 0.005F

#define ANGLE_FILTER_GYRO 0.98F

void controlLoopInit(void);
void pidControl(float angle);

#endif