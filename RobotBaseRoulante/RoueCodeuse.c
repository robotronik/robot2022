#include "RoueCodeuse.h"


void exti_setup(void)
{
	/* Enable GPIOA clock. */
	rcc_periph_clock_enable(RCC_GPIOA);
	robot.alpha = 0;
	robot.x = 0;
	robot.y = 0;


	/* Enable EXTI0 interrupt. */
	nvic_enable_irq(NVIC_EXTI9_5_IRQ);
	nvic_set_priority(NVIC_EXTI9_5_IRQ, 0);

	/* Set GPIO0 (in GPIO port A) to 'input open-drain'. */
	gpio_mode_setup(GPIOA, GPIO_MODE_INPUT, GPIO_MODE_INPUT , GPIO8);
	gpio_mode_setup(GPIOA, GPIO_MODE_INPUT, GPIO_MODE_INPUT , GPIO9);

	/* Configure the EXTI subsystem. */
	exti_select_source(EXTI8, GPIOA);
	exti_set_trigger(EXTI8, EXTI_TRIGGER_BOTH);
	exti_enable_request(EXTI8);


	nvic_enable_irq(NVIC_EXTI1_IRQ);
	nvic_set_priority(NVIC_EXTI1_IRQ, 0);

	gpio_mode_setup(GPIOA, GPIO_MODE_INPUT, GPIO_MODE_INPUT , GPIO0);
	gpio_mode_setup(GPIOA, GPIO_MODE_INPUT, GPIO_MODE_INPUT , GPIO1);

	exti_select_source(EXTI1, GPIOA);
	exti_set_trigger(EXTI1, EXTI_TRIGGER_BOTH);
	exti_enable_request(EXTI1);
}

void printnum(void){
	coordonne printcoor = getCoordonneRobotMod();
	usart_send_debug("angle : ");
	usart_send_debugfloat(getCoordonneRobotMod().alpha);
	usart_send_debugTab();
	usart_send_debug("x : ");
	usart_send_debugfloat(robot.x);
	usart_send_debugTab();
	usart_send_debug("y : ");
	usart_send_debugfloat(robot.y);
};

//GAUCHE
void exti9_5_isr(void)
{
	exti_reset_request(EXTI8);
	if(gpio_get (GPIOA,GPIO9)){
		if(gpio_get (GPIOA,GPIO8)==0){
			//Vers l'avant
			robot.x += STEPAVANCE * cos_precalc(robot.alpha-90); //Voir pour optimisation
			robot.y -= STEPAVANCE * sin_precalc(robot.alpha-90); //Voir pour optimisation
			robot.alpha += STEPANGLEG;
		}
		else{
			//Vers l'arrière
			robot.x -= STEPAVANCE * cos_precalc(robot.alpha-90); //Voir pour optimisation
			robot.y += STEPAVANCE * sin_precalc(robot.alpha-90); //Voir pour optimisation
			robot.alpha -= STEPANGLEG;
		}
	}
}

//DROITE
void exti1_isr(void)
{
	exti_reset_request(EXTI1);
	if(gpio_get (GPIOA,GPIO0)){
		if(gpio_get (GPIOA,GPIO1)==0){
			//Vers l'avant
			robot.x += STEPAVANCE * cos_precalc(robot.alpha-90); //Voir pour optimisation
			robot.y -= STEPAVANCE * sin_precalc(robot.alpha-90); //Voir pour optimisation
			robot.alpha -= STEPANGLED;
		}
		else{
			//Vers l'arrière
			robot.x -= STEPAVANCE * cos_precalc(robot.alpha-90); //Voir pour optimisation
			robot.y += STEPAVANCE * sin_precalc(robot.alpha-90); //Voir pour optimisation
			robot.alpha += STEPANGLED;
		}
	}
	//gpio_set(GPIOA, GPIO11);
}


coordonne getCoordonneRobot(void){
	return robot;
}

coordonne getCoordonneRobotMod(void){
	coordonne retcoord;
	retcoord = robot;
	retcoord.alpha = fmod(retcoord.alpha,360);
	if(retcoord.alpha>180){
		retcoord.alpha -=360;
	}
	else if(retcoord.alpha<-180){
		retcoord.alpha +=360;
	}
	return retcoord;
}


void setCoordonneRobot(double x, double y, double alpha){
	robot.x = x;
	robot.y = y;
	robot.alpha = alpha;
}

