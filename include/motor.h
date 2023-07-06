#ifndef MOTOR_H_
#define MOTOR_H_

#include "utils.h"
#include "stdbool.h"

#define CW 0
#define CCW 1

void motorInit(void);
void motorRun(uint8_t speed, bool direction);
void motorStop(void);

#endif