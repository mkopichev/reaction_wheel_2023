#ifndef MPU_H_
#define MPU_H_

#include "utils.h"

#define MPU_ADDRESS  0x68 // 0b01101000
// config registers
#define PWR_MGMT_1   0x6B
#define PWR_MGMT_2   0x6C
#define CONFIG       0x1A
#define GYRO_CONFIG  0x1B
#define ACCEL_CONFIG 0x1C
// data registers
#define ACCEL_XOUT_H 0x3B
#define ACCEL_XOUT_L 0x3C
#define ACCEL_YOUT_H 0x3D
#define ACCEL_YOUT_L 0x3E
#define ACCEL_ZOUT_H 0x3F
#define ACCEL_ZOUT_L 0x40
#define TEMP_OUT_H   0x41
#define TEMP_OUT_L   0x42
#define GYRO_XOUT_H  0x43
#define GYRO_XOUT_L  0x44
#define GYRO_YOUT_H  0x45
#define GYRO_YOUT_L  0x46
#define GYRO_ZOUT_H  0x47
#define GYRO_ZOUT_L  0x48
// check register
#define WHO_AM_I     0x75
// constants and figures
#define RAD_TO_DEG   180 / 3.141593
// axes of mpu6050
#define X_AXIS       0
#define Y_AXIS       1
#define Z_AXIS       2

void mpuInit(void);
void mpuGetAngle(void);
void mpuPrintAngle(void);

#endif