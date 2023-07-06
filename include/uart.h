#ifndef UART_H_
#define UART_H_

#include "utils.h"

void uartInit(void);

// transmit byte of data
void uartTransmitByte(uint8_t byte);

// recieve byte of data
uint8_t uartReceiveByte(void);

// transmit any data byte by byte
void uartTransmitMultipleData(void *buf, uint8_t length);

// transmit string, phrase or word
void uartTransmitStr(char *string);

// transmit decimal value from mk to terminal
void uartTransmitDec(int16_t val);

// transmit binary value from mk to terminal
void uartTransmitBin(uint8_t val);

// transmit hex value from mk to terminal
void uartTransmitHex(uint32_t val);

// transmit float value from mk to terminal
void uarTransmitFloat(float val);

// recieve decimal value from terminal to mk
uint32_t uartReceiveDec(void);

#endif