#include "../include/mpu.h"

float time_elapsed;

float accel_res[3] = {0, 0, 0};
float temp_res;
float gyro_res[3] = {0, 0, 0};
float gyro_res_error[3] = {0, 0, 0};
float gyro_angle[3] = {0, 0, 0};
float accel_angle[3] = {0, 0, 0};
float inclination_angle = 0;

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
        gyro_res[X_AXIS] = ((int)mpuReadRegister(GYRO_XOUT_H) << 8) | (int)mpuReadRegister(GYRO_XOUT_L);
        gyro_res[Y_AXIS] = ((int)mpuReadRegister(GYRO_YOUT_H) << 8) | (int)mpuReadRegister(GYRO_YOUT_L);
        gyro_res[Z_AXIS] = ((int)mpuReadRegister(GYRO_ZOUT_H) << 8) | (int)mpuReadRegister(GYRO_ZOUT_L);

        gyro_res_error[X_AXIS] = gyro_res_error[X_AXIS] + gyro_res[X_AXIS];
        gyro_res_error[Y_AXIS] = gyro_res_error[Y_AXIS] + gyro_res[Y_AXIS];
        gyro_res_error[Z_AXIS] = gyro_res_error[Z_AXIS] + gyro_res[Z_AXIS];
    }
    gyro_res_error[X_AXIS] = gyro_res_error[X_AXIS] / 60;
    gyro_res_error[Y_AXIS] = gyro_res_error[Y_AXIS] / 60;
    gyro_res_error[Z_AXIS] = gyro_res_error[Z_AXIS] / 60;
}

void mpuPrintAccGyroValues(void) {

    char buffer[20], float_[10];
    /* Take values in buffer to send all parameters over USART */
    dtostrf(accel_res[X_AXIS], 3, 2, float_);
    sprintf(buffer, " Ax = %s g\t", float_);
    uartTransmitStr(buffer);
    dtostrf(accel_res[Y_AXIS], 3, 2, float_);
    sprintf(buffer, " Ay = %s g\t", float_);
    uartTransmitStr(buffer);
    dtostrf(accel_res[Z_AXIS], 3, 2, float_);
    sprintf(buffer, " Az = %s g\t", float_);
    uartTransmitStr(buffer);
    dtostrf(temp_res, 3, 2, float_);
    /* 0xF8 Ascii value of degree on serial */
    sprintf(buffer, " T = %s C\t", float_);
    uartTransmitStr(buffer);
    dtostrf(gyro_res[X_AXIS], 3, 2, float_);
    sprintf(buffer, " Gx = %s d/s\t", float_);
    uartTransmitStr(buffer);
    dtostrf(gyro_res[Y_AXIS], 3, 2, float_);
    sprintf(buffer, " Gy = %s d/s\t", float_);
    uartTransmitStr(buffer);
    dtostrf(gyro_res[Z_AXIS], 3, 2, float_);
    sprintf(buffer, " Gz = %s d/s\r\n", float_);
    uartTransmitStr(buffer);
    uartTransmitByte('\n');
}

void mpuGetAccGyroData(void) {

    accel_res[X_AXIS] = (((int)mpuReadRegister(ACCEL_XOUT_H) << 8) | (int)mpuReadRegister(ACCEL_XOUT_L)) / 4096.0;
    accel_res[Y_AXIS] = (((int)mpuReadRegister(ACCEL_YOUT_H) << 8) | (int)mpuReadRegister(ACCEL_YOUT_L)) / 4096.0;
    accel_res[Z_AXIS] = (((int)mpuReadRegister(ACCEL_ZOUT_H) << 8) | (int)mpuReadRegister(ACCEL_ZOUT_L)) / 4096.0;
    temp_res = ((((int)mpuReadRegister(TEMP_OUT_H) << 8) | (int)mpuReadRegister(TEMP_OUT_L)) / 340.00) + 36.53;
    gyro_res[X_AXIS] = ((((int)mpuReadRegister(GYRO_XOUT_H) << 8) | (int)mpuReadRegister(GYRO_XOUT_L)) - gyro_res_error[X_AXIS]) / 65.5;
    gyro_res[Y_AXIS] = ((((int)mpuReadRegister(GYRO_YOUT_H) << 8) | (int)mpuReadRegister(GYRO_YOUT_L)) - gyro_res_error[Y_AXIS]) / 65.5;
    gyro_res[Z_AXIS] = ((((int)mpuReadRegister(GYRO_ZOUT_H) << 8) | (int)mpuReadRegister(GYRO_ZOUT_L)) - gyro_res_error[Z_AXIS]) / 65.5;
}

void mpuInit(void) {

    mpuSendCommand(PWR_MGMT_1, 0); // internal 8MHz clock
    _delay_ms(25);
    mpuSendCommand(PWR_MGMT_2, 0);
    mpuSendCommand(CONFIG, (1 << 2));       // filter is on
    mpuSendCommand(GYRO_CONFIG, (1 << 3));  // +-500 deg/s sensitivity 65.5 LSB/deg/s
    mpuSendCommand(ACCEL_CONFIG, (1 << 4)); // +-8 g sensitivity 4096 LSB/g
    mpuErrorCalc();
}

void mpuPrintAngle(void) {

    char buffer[20], float_[10];
    dtostrf(inclination_angle, 5, 3, float_);
    sprintf(buffer, "Incl. angle = %s d\r\n", float_);
    uartTransmitStr(buffer);
}

void mpuGetAngle(void) {

    mpuGetAccGyroData();

    // gyro data has period from control.c file
    gyro_angle[Y_AXIS] = gyro_res[Y_AXIS] * TIME_CONSTANT;
    gyro_angle[Z_AXIS] = -gyro_res[Z_AXIS] * TIME_CONSTANT;
    accel_angle[X_AXIS] = (atan2f((accel_res[Y_AXIS]), sqrt(pow((accel_res[X_AXIS]), 2) + pow((accel_res[Z_AXIS]), 2))) * RAD_TO_DEG);
    accel_angle[Y_AXIS] = (atan2f(-1 * (accel_res[X_AXIS]), sqrt(pow((accel_res[Y_AXIS]), 2) + pow((accel_res[Z_AXIS]), 2))) * RAD_TO_DEG);

    static float gyro_z = 0;
    gyro_z += gyro_angle[Z_AXIS];

    inclination_angle = ANGLE_FILTER_GYRO * (inclination_angle + gyro_angle[Z_AXIS]) + (1.0F - ANGLE_FILTER_GYRO) * accel_angle[Y_AXIS];

    // uartTransmitByte(42);
    // uartTransmitMultipleData(&accel_angle[Y_AXIS], sizeof(float));
    // uartTransmitMultipleData(&gyro_z, sizeof(float));
    // uartTransmitMultipleData(&inclination_angle, sizeof(float));
}