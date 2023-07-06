#ifndef UTILS_H_
#define UTILS_H_

#include <avr/io.h>
// #define F_CPU 16000000
#include "control.h"
#include "motor.h"
#include "mpu.h"
#include "twi.h"
#include "uart.h"
#include <avr/interrupt.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <util/delay.h>

uint8_t digit(uint16_t d, uint8_t m);
uint16_t findMiddleValue(uint16_t a, uint16_t b, uint16_t c);
uint16_t findMaxValue(uint16_t a, uint16_t b, uint16_t c);
uint16_t findMinValue(uint16_t a, uint16_t b, uint16_t c);

#endif