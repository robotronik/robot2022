#pragma once

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/timer.h>

#define PWM_MAX 500
#define PWM_MIN 500
#define TIMER_PERIODE 1000

void moteur_setup(void);
void init_pwm_moteurs(void);
void setVitesseMoteurGauche(int PWM);
void setVitesseMoteurDroit(int PWM);


void enableMoteurGauche(void);
void disableMoteurGauche(void);
void enableMoteurDroit(void);
void disableMoteurDroit(void);

void enableBrakeMoteurGauche(void);
void disableBrakeMoteurGauche(void);
void enableBrakeMoteurDroit(void);
void disableBrakeMoteurDroit(void);

