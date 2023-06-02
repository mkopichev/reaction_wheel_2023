#ifndef CONTROL_H_
#define CONTROL_H_

#include "utils.h"

#define SETPOINT 0

#define P_COEF 70
#define I_COEF -0.9
#define D_COEF 0

#define TIME_CONSTANT 0.004

void controlLoopInit(void);
void pidControl(float angle);

#endif