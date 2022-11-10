#ifndef i2C_H
#define i2C_H

#include <libopencm3/stm32/i2c.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/cm3/nvic.h>
#include "clock.h"



typedef struct
{
    uint16_t cmd;
    uint16_t x;
    uint16_t y;
    uint16_t r;

}i2CReceive;

typedef union
{
    i2CReceive structR;
    uint8_t tabR[7];
}i2CReceiveUnion;


typedef struct
{
    uint16_t cmd;
    uint16_t distance;
}i2CReceiveFin;

typedef union
{
    i2CReceiveFin structR;
    uint8_t tabR[4];
}i2CReceiveFinUnion;



#define MYADRESSE 120
#define COMPUTERADRESSE 40

void i2c_setup(void);
void i2c_transferAntiBlocage(uint32_t i2c, uint8_t addr, const uint8_t *w, size_t wn);
void i2c_send(uint8_t addr, const uint8_t *w, size_t wn);
void i2c_testRecaive(void);

void printbit(void);

void setCallback(void (*f)(i2CReceiveUnion reception));
void disableCallback(void);


#endif