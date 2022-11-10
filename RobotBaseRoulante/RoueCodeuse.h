#pragma once

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/exti.h>
#include <math.h>
#include "math_precalc.h"
#include "uart.h"
#include "variable.h"

// STEPANGLE = (RayonRoueCodeuse * 360) / (NbDePasRoueCodeuse * LargeurEntraxeDes2RoueCodeuses * 2)
// STEPANGLE = (41.30 * 360) / (360 * 204 * 2)
#define STEPANGLED 0.10200558         //0.10221
// STEPANGLE = (41.05 * 360) / (360 * 204 * 2)
#define STEPANGLEG 0.1013469         //0.10145

// STEPAVANCE = (41.30 * pi) / (360 * 2)
#define STEPAVANCE 0.18135
// STEPASTEPAVANCENGLE = (41.05 * pi) / (360 * 2)

static coordonne robot;

void exti_setup(void);
void printnum(void);
coordonne getCoordonneRobot(void);
coordonne getCoordonneRobotMod(void); //de -180 a 180 degre

void setCoordonneRobot(double x, double y, double alpha);
