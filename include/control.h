#ifndef CONTROL_H_
#define CONTROL_H_

#include "utils.h"

#define ANGLE_FIXRATE 0.1F

#define P_COEF 98.2F
#define I_COEF 19.7F
#define D_COEF 0.009F

#define TIME_CONSTANT 0.005F

#define ANGLE_FILTER_GYRO 0.98F

void controlLoopInit(void);
void pidControl(float angle);

#endif