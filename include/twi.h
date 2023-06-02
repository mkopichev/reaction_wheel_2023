#ifndef TWI_H_
#define TWI_H_

#include "utils.h"
#include <stdbool.h>

#define W 0
#define R 1

#define SCL_FREQ 400 // kHz

#define VACANT   0
#define OCCUPIED 1

void twiInit(void);
void twiStart(void);
void twiStop(void);
void twiTransmitByte(uint8_t data);
uint8_t twiReceiveByte(bool is_last_byte);
void twiWriteData(uint8_t *tx_buf, uint8_t length);
void twiReadData(uint8_t *rx_buf, uint8_t length);

void twiAcquire(void);
void twiRelease(void);

#endif