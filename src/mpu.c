#include "../include/mpu.h"

float timeElapsed;

float accelRes[3] = {0, 0, 0};
float tempRes;
float gyroRes[3] = {0, 0, 0};
float gyroResError[3] = {0, 0, 0};
float gyroAngle[3] = {0, 0, 0};
float accelAngle[3] = {0, 0, 0};
float inclinationAngle = 0;

void mpuSendCommand(uint8_t reg_addr, uint8_t reg_value) {

    twiStart();
    twiTransmitByte((MPU_ADDRESS << 1) | W);
    twiTransmitByte(reg_addr);
    twiTransmitByte(reg_value);
    twiStop();
}

uint8_t mpuReadRegister(uint8_t reg_addr) {

    twiStart();
    twiTransmitByte((MPU_ADDRESS << 1) | W);
    twiTransmitByte(reg_addr);
    twiStart();
    twiTransmitByte((MPU_ADDRESS << 1) | R);
    uint8_t result = twiReceiveByte(1);
    twiStop();
    return result;
}

void mpuErrorCalc(void) {

    for(uint8_t i = 0; i < 60; i++) {
        gyroRes[X_AXIS] = ((int)mpuReadRegister(GYRO_XOUT_H) << 8) | (int)mpuReadRegister(GYRO_XOUT_L);
        gyroRes[Y_AXIS] = ((int)mpuReadRegister(GYRO_YOUT_H) << 8) | (int)mpuReadRegister(GYRO_YOUT_L);
        gyroRes[Z_AXIS] = ((int)mpuReadRegister(GYRO_ZOUT_H) << 8) | (int)mpuReadRegister(GYRO_ZOUT_L);

        gyroResError[X_AXIS] = gyroResError[X_AXIS] + gyroRes[X_AXIS];
        gyroResError[Y_AXIS] = gyroResError[Y_AXIS] + gyroRes[Y_AXIS];
        gyroResError[Z_AXIS] = gyroResError[Z_AXIS] + gyroRes[Z_AXIS];
    }
    gyroResError[X_AXIS] = gyroResError[X_AXIS] / 60;
    gyroResError[Y_AXIS] = gyroResError[Y_AXIS] / 60;
    gyroResError[Z_AXIS] = gyroResError[Z_AXIS] / 60;
}

void mpuPrintAccGyroValues(void) {

    char buffer[20], float_[10];
    /* Take values in buffer to send all parameters over USART */
    dtostrf(accelRes[X_AXIS], 3, 2, float_);
    sprintf(buffer, " Ax = %s g\t", float_);
    uartTransmitStr(buffer);
    dtostrf(accelRes[Y_AXIS], 3, 2, float_);
    sprintf(buffer, " Ay = %s g\t", float_);
    uartTransmitStr(buffer);
    dtostrf(accelRes[Z_AXIS], 3, 2, float_);
    sprintf(buffer, " Az = %s g\t", float_);
    uartTransmitStr(buffer);
    dtostrf(tempRes, 3, 2, float_);
    /* 0xF8 Ascii value of degree on serial */
    sprintf(buffer, " T = %s C\t", float_);
    uartTransmitStr(buffer);
    dtostrf(gyroRes[X_AXIS], 3, 2, float_);
    sprintf(buffer, " Gx = %s d/s\t", float_);
    uartTransmitStr(buffer);
    dtostrf(gyroRes[Y_AXIS], 3, 2, float_);
    sprintf(buffer, " Gy = %s d/s\t", float_);
    uartTransmitStr(buffer);
    dtostrf(gyroRes[Z_AXIS], 3, 2, float_);
    sprintf(buffer, " Gz = %s d/s\r\n", float_);
    uartTransmitStr(buffer);
    uartTransmitByte('\n');
}

void mpuGetAccGyroData(void) {

    accelRes[X_AXIS] = (((int)mpuReadRegister(ACCEL_XOUT_H) << 8) | (int)mpuReadRegister(ACCEL_XOUT_L)) / 4096.0;
    accelRes[Y_AXIS] = (((int)mpuReadRegister(ACCEL_YOUT_H) << 8) | (int)mpuReadRegister(ACCEL_YOUT_L)) / 4096.0;
    accelRes[Z_AXIS] = (((int)mpuReadRegister(ACCEL_ZOUT_H) << 8) | (int)mpuReadRegister(ACCEL_ZOUT_L)) / 4096.0;
    tempRes = ((((int)mpuReadRegister(TEMP_OUT_H) << 8) | (int)mpuReadRegister(TEMP_OUT_L)) / 340.00) + 36.53;
    gyroRes[X_AXIS] = ((((int)mpuReadRegister(GYRO_XOUT_H) << 8) | (int)mpuReadRegister(GYRO_XOUT_L)) - gyroResError[X_AXIS]) / 65.5;
    gyroRes[Y_AXIS] = ((((int)mpuReadRegister(GYRO_YOUT_H) << 8) | (int)mpuReadRegister(GYRO_YOUT_L)) - gyroResError[Y_AXIS]) / 65.5;
    gyroRes[Z_AXIS] = ((((int)mpuReadRegister(GYRO_ZOUT_H) << 8) | (int)mpuReadRegister(GYRO_ZOUT_L)) - gyroResError[Z_AXIS]) / 65.5;
}

void mpuInit(void) {

    mpuSendCommand(PWR_MGMT_1, 0); // internal 8MHz clock
    _delay_ms(25);
    mpuSendCommand(PWR_MGMT_2, 0);
    mpuSendCommand(CONFIG, (1 << 2));       // filter is on
    mpuSendCommand(GYRO_CONFIG, (1 << 3));  // +-500 deg/s sensitivity 65.5 LSB/deg/s
    mpuSendCommand(ACCEL_CONFIG, (1 << 4)); // +-8 g sensitivity 4096 LSB/g
    _delay_ms(25);
    mpuErrorCalc();
}

void mpuPrintAngle(void) {

    char buffer[20], float_[10];
    dtostrf(inclinationAngle, 5, 3, float_);
    sprintf(buffer, "Incl. angle = %s d\r\n", float_);
    uartTransmitStr(buffer);
}

void mpuGetAngle(void) {

    mpuGetAccGyroData();

    // gyro data has period from control.c file
    gyroAngle[Y_AXIS] = gyroRes[Y_AXIS] * TIME_CONSTANT;
    gyroAngle[Z_AXIS] = -gyroRes[Z_AXIS] * TIME_CONSTANT;
    accelAngle[X_AXIS] = (atan2f((accelRes[Y_AXIS]), sqrt(pow((accelRes[X_AXIS]), 2) + pow((accelRes[Z_AXIS]), 2))) * RAD_TO_DEG);
    accelAngle[Y_AXIS] = (atan2f(-1 * (accelRes[X_AXIS]), sqrt(pow((accelRes[Y_AXIS]), 2) + pow((accelRes[Z_AXIS]), 2))) * RAD_TO_DEG);

    static float gyro_z = 0;
    gyro_z += gyroAngle[Z_AXIS];

    inclinationAngle = ANGLE_FILTER_GYRO * (inclinationAngle + gyroAngle[Z_AXIS]) + (1.0F - ANGLE_FILTER_GYRO) * accelAngle[Y_AXIS];

    // uartTransmitByte(42);
    // uartTransmitMultipleData(&accelAngle[Y_AXIS], sizeof(float));
    // uartTransmitMultipleData(&gyro_z, sizeof(float));
    // uartTransmitMultipleData(&inclinationAngle, sizeof(float));
}