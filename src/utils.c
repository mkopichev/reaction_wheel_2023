#include "../include/utils.h"

uint8_t digit(uint16_t d, uint8_t m) {

    uint8_t i = 5, a;
    while(i) {      // цикл по разрядам числа
        a = d % 10; // выделяем очередной разряд
        // выделен заданный разряд - уходим
        if(i-- == m)
            break;
        d /= 10; // уменьшаем число в 10 раз
    }
    return (a);
}

uint16_t findMiddleValue(uint16_t a, uint16_t b, uint16_t c) {

    if(a > b) {
        if(b > c) {
            return (b);
        } else if(a > c) {
            return (c);
        } else {
            return (a);
        }
    } else {
        if(a > c) {
            return (a);
        } else if(b > c) {
            return (c);
        } else {
            return (b);
        }
    }
}

uint16_t findMaxValue(uint16_t a, uint16_t b, uint16_t c) {

    if(a > b) {
        if(b > c) {
            return (a);
        } else if(c > a) {
            return (c);
        } else {
            return (a);
        }
    } else {
        if(b > c) {
            return (b);
        } else {
            return (c);
        }
    }
}

uint16_t findMinValue(uint16_t a, uint16_t b, uint16_t c) {
    
    if(a < b) {
        if(b < c) {
            return (a);
        } else if(c < a) {
            return (c);
        } else {
            return (a);
        }
    } else {
        if(b < c) {
            return (b);
        } else {
            return (c);
        }
    }
}