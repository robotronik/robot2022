/*
 * This file is part of the libopencm3 project.
 *
 * Copyright (C) 2009 Uwe Hermann <uwe@hermann-uwe.de>
 * Copyright (C) 2011 Stephen Caudle <scaudle@doceme.com>
 * Modified by Fernando Cortes <fermando.corcam@gmail.com>
 * modified by Guillermo Rivera <memogrg@gmail.com>
 *
 * This library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/usart.h>
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/exti.h>
#include "Test.h"
#include "uart.h"
#include "RoueCodeuse.h"
#include "math_precalc.h"
#include "moteur.h"
#include "asservissement.h"
#include "clock.h"
#include "i2C.h"
#include "commande.h"

uint8_t data[] = "FIN";

static void gpio_setup(void)
{
	/* Enable GPIOE clock. */
	rcc_periph_clock_enable(RCC_GPIOA);
	rcc_periph_clock_enable(RCC_GPIOB);
	rcc_periph_clock_enable(RCC_GPIOC);
	

	/* Set GPIO12 (in GPIO port E) to 'output push-pull'. */
	//gpio_mode_setup(GPIOB, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO3);

	gpio_mode_setup(GPIOA, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO11);


}

int valcmdDebug;
int xDebug;
int yDebug;
int zDebug;
coordonne consigneDeb;



void fonctionTest(i2CReceive val){
	coordonne consigneDeb;
	consigneDeb.x = (short int)val.x; //cast car sinon bug de signe
	consigneDeb.y = (short int)val.y;
	consigneDeb.alpha = (short int)val.r;
	valcmdDebug = val.cmd;
	xDebug = consigneDeb.x;
	yDebug = consigneDeb.y;
	zDebug = consigneDeb.alpha;

	usart_send_debugln("");
	usart_send_debugintln(val.cmd);
	usart_send_debugintln(consigneDeb.x);
	usart_send_debugintln(consigneDeb.y );
	usart_send_debugintln(consigneDeb.alpha);
	usart_send_debugln("");


	switch (val.cmd)
	{
	case SETPOSITOION :
		setCoordonneRobot(consigneDeb.x,consigneDeb.y,consigneDeb.alpha);
		resetAsserv();
		break;
	case LINEAIRE :
		setConsigneLineaire(consigneDeb);
		break;
	case ANGULAIRE :
		setConsigneAngulaire(consigneDeb);
		break;
	case TOURNERAVANCER :
		//setConsigneTournerAvancer(consigne);
		break;
	case TOURNER :
		setConsigneTourner(consigneDeb);
		break;
	case STOPMOTOR :
		stopMoteur();
		break;
		
	
	default:
		break;
	}
}

void fonctionFinAsserv(void){
	i2CReceiveFinUnion data;
	data.structR.cmd = 70;
	data.structR.distance = 0;
	//usart_send_debug("A");
	//i2c_send(40, data.tabR , 4);
	led_ON();
}

void fonctionDistance(int a){
	i2CReceiveFinUnion data;
	data.structR.cmd = 71;
	data.structR.distance = a;
	usart_send_debug("B");
	i2c_send(40, data.tabR , 4);
}


int main(void)
{

	gpio_setup();
	exti_setup();
	moteur_setup();
	clock_setup();
	asservisement_setup();

	init_pwm_moteurs();
	setVitesseMoteurGauche(0);
	setVitesseMoteurDroit(0);

	int i, j = 0, c = 0;
	commande comMoteur;
	coordonne consigne;
	coordonne coordRobot;
	consigne.alpha = 0;
	consigne.y = 0;
	consigne.x = 0;

	//setConsigneAngulaire(consigne);

	
	usart_setup();
	usart_send_debugln("Usart ok");
	// usart_send_debugln("Test");

	// usart_send_debugln("??????");
	

	setCallback(fonctionTest);
	asservSetCallback(fonctionFinAsserv);
	asservSetDistanceCallback(fonctionDistance);

	i2c_setup();

	int itest =0;
	int valDebugPre = valcmdDebug;
	int valDebugPrex = consigneDeb.x;
	int valDebugPrey = consigneDeb.y;
	int valDebugPrealpha = consigneDeb.alpha;
	while (1)
	{	
		//delay_ms(1);
		if(valcmdDebug != valDebugPre || valDebugPrex != consigneDeb.x || valDebugPrey != consigneDeb.y || valDebugPrealpha != consigneDeb.alpha){
			valDebugPre = valcmdDebug;
			valDebugPrex = consigneDeb.x;
			valDebugPrey = consigneDeb.y;
			valDebugPrealpha = consigneDeb.alpha;

			// usart_send_debugint(consigneDeb.x);
			// usart_send_debugTab();
			// usart_send_debugint(consigneDeb.y);
			// usart_send_debugTab();
			// usart_send_debugint(consigneDeb.alpha);
			// usart_send_debugTab();
			// usart_send_debugintln(valcmdDebug);

			// switch (valcmdDebug)
			// {
			// case SETPOSITOION :
			// 	setCoordonneRobot(consigneDeb.x,consigneDeb.y,consigneDeb.alpha);
			// 	resetAsserv();
			// 	break;
			// case LINEAIRE :
			// 	setConsigneLineaire(consigneDeb);
			// 	break;
			// case ANGULAIRE :
			// 	setConsigneAngulaire(consigneDeb);
			// 	break;
			// case TOURNERAVANCER :
			// 	//setConsigneTournerAvancer(consigne);
			// 	break;
			// case TOURNER :
			// 	setConsigneTourner(consigneDeb);
			// 	break;
			// case STOPMOTOR :
			// 	stopMoteur();
			// 	break;
				
			
			// default:
			// 	break;
			// }
		}
		usart_send_debug("1");
		
		
	}
	

	

	delay_ms(2000);
	consigne.y = -200;
	consigne.x = 0;
	setConsigneLineaire(consigne);

	delay_ms(1500);
	consigne.alpha = 180;
	setConsigneAngulaire(consigne);

	delay_ms(2000);
	consigne.y = -2000;
	consigne.x = 0;
	setConsigneLineaire(consigne);
	
	delay_ms(4000);
	consigne.alpha = 0;
	setConsigneAngulaire(consigne);

	delay_ms(2000);
	consigne.y = 0;
	consigne.x = 0;
	setConsigneLineaire(consigne);

	delay_ms(10000);
	consigne.alpha = 0;
	setConsigneAngulaire(consigne);

	// delay_ms(10000);

	while (1)
	{
		// delay_ms(30);
		// printnum();
		// usart_send_debugTab();
		// usart_send_debug("Lineaire : ");
		// usart_send_debugfloat(getinfo());
		// usart_send_debugTab();
		// usart_send_debug("vitesse : ");
		// usart_send_debugfloat(getinfo2());
		// usart_send_debugTab();
		// delay_ms(10);
		// printAsserv();
		// usart_send_debugln("");
	}
	


	while (1)
	{

		//asservissement
		//coordRobot = getCoordonneRobot();
		//usart_send_debugfloatln(coordRobot.alpha);
		//comMoteur = asservissementAngulaire(consigne,coordRobot);
		//usart_send_debugfloatln(comMoteur.gauche);
		//setVitesseMoteurGauche(comMoteur.gauche);
		//setVitesseMoteurDroit(comMoteur.droit);
		
		// c = (c == 400) ? 0 : c + 1;	/* Increment c. */
		// if (c == 0) {		/* Newline after line full. */
		// 	consigne.alpha = -720;
		// 	setConsigne(consigne);
		// }
		// else if(c==150){
		// 	consigne.alpha = 720;
		// 	setConsigne(consigne);
		// }
		// else if(c==200){
		// 	consigne.alpha = 700;
		// 	setConsigne(consigne);
		// }
		// else if(c==250){
		// 	consigne.alpha = 720;
		// 	setConsigne(consigne);
		// }

		
		// if (c == 0) {		/* Newline after line full. */
		// 	consigne.y = 3000;
		// 	consigne.x = 0;
		// 	setConsigneLineaire(consigne);
		// }
		// else if(c==100){
		// 	consigne.alpha = 180;
		// 	setConsigneAngulaire(consigne);
		// }
		// else if(c==200){
		// 	consigne.y = 0;
		// 	consigne.x = 0;
		// 	setConsigneLineaire(consigne);
		// }
		// else if(c==300){
		// 	consigne.alpha = 180;
		// 	setConsigneAngulaire(consigne);
		// }
		// else if(c==400){
		// 	consigne.alpha = 0;
		// 	setConsigneAngulaire(consigne);
		// }
		// else if(c==500){
		// 	consigne.y = 0;
		// 	consigne.x = 0;
		// 	setConsigneLineaire(consigne);
		// }
		// c = (c == 600) ? 0 : c + 1;	/* Increment c. */


		//usart_send_debugintln((int)get_systicks());
		
		delay_ms(30);
		printnum();
		usart_send_debugTab();
		usart_send_debug("Lineaire : ");
		usart_send_debugfloat(getinfo());
		usart_send_debugTab();
		usart_send_debug("vitesse : ");
		usart_send_debugfloat(getinfo2());
		// usart_send_debugTab();
		// delay_ms(10);
		// printAsserv();
		usart_send_debugln("");
		// for (i = 0; i < 1000; i++) {	/* Wait a bit. */
		// 	__asm__("NOP");
		// }
		
		
	}

	return 0;
}

