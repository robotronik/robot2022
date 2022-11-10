#include "moteur.h"

void moteur_setup(void){

    //BRAKE
    gpio_mode_setup(GPIOB, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO4);
    gpio_set(GPIOB, GPIO4);

    //Enable
    gpio_mode_setup(GPIOB, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO5);
    gpio_set(GPIOB, GPIO5);


    //PA15
    gpio_mode_setup(GPIOA, GPIO_MODE_INPUT, GPIO_PUPD_NONE, GPIO15);






    //BRAKE
    gpio_mode_setup(GPIOA, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO7);
    gpio_set(GPIOA, GPIO7);

    //Enable
    gpio_mode_setup(GPIOA, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO6);
    gpio_set(GPIOA, GPIO6);

    //PA15
    gpio_mode_setup(GPIOB, GPIO_MODE_INPUT, GPIO_PUPD_NONE, GPIO1);
    //gpio_mode_setup(GPIOB, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO1);
    //gpio_clear(GPIOB, GPIO1);

}




void init_pwm_moteurs(void) {
    //RCC
    rcc_clock_setup_hsi(&rcc_hsi_configs[RCC_CLOCK_HSI_64MHZ]);
	rcc_periph_clock_enable(RCC_TIM2);
    rcc_periph_clock_enable(RCC_TIM1);
	rcc_periph_clock_enable(RCC_GPIOB);


    //GPIO
	gpio_mode_setup(GPIOB, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO3);
	gpio_set_af(GPIOB, GPIO_AF1, GPIO3);
	gpio_set_output_options(GPIOB, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO3);

	//TIMER
	timer_set_mode(TIM2, TIM_CR1_CKD_CK_INT, TIM_CR1_CMS_CENTER_1,
				TIM_CR1_DIR_UP);
	timer_set_oc_mode(TIM2, TIM_OC2, TIM_OCM_PWM1);
	timer_enable_oc_output(TIM2, TIM_OC2);
	timer_enable_break_main_output(TIM2);
    timer_set_oc_polarity_high(TIM2, TIM_OC2);
    timer_set_period(TIM2, TIMER_PERIODE);
    timer_set_oc_value(TIM2, TIM_OC2, 500);
    timer_enable_counter(TIM2);


	
    //GPIO
    gpio_mode_setup(GPIOB, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO0);
	gpio_set_af(GPIOB, GPIO_AF6, GPIO0);
	gpio_set_output_options(GPIOB, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO0);
	
    //TIMER
    timer_set_mode(TIM1, TIM_CR1_CKD_CK_INT, TIM_CR1_CMS_CENTER_1,
				TIM_CR1_DIR_UP);
	timer_set_oc_mode(TIM1, TIM_OC2, TIM_OCM_PWM1);	
	timer_enable_oc_output(TIM1, TIM_OC2N);
    timer_enable_break_main_output(TIM1);
    timer_set_oc_polarity_high(TIM1, TIM_OC2N);
    timer_set_period(TIM1, TIMER_PERIODE);
	timer_set_oc_value(TIM1, TIM_OC2, 500);
	timer_enable_counter(TIM1);



	
    
}

//Prend en entré une valeur de -500 a 500
void setVitesseMoteurGauche(int PWM) {
    int pwm =  PWM - 500;
    pwm = -pwm;
    if (pwm > TIMER_PERIODE){
        pwm = TIMER_PERIODE;
    }
    if (pwm < 0){
        pwm = 0;
    }
    timer_set_oc_value(TIM1, TIM_OC2,pwm);  
}

//Prend en entré une valeur de -500 a 500
void setVitesseMoteurDroit(int PWM) {
     int pwm =  PWM + 500;

    if (pwm > TIMER_PERIODE){
        pwm = TIMER_PERIODE;
    }
    if (pwm < 0){
        pwm = 0;
    }
    timer_set_oc_value(TIM2, TIM_OC2,pwm);         
}


void enableMoteurGauche(void){
    gpio_set(GPIOA, GPIO6);
}
void disableMoteurGauche(void){
    gpio_clear(GPIOA, GPIO6);
}

void enableMoteurDroit(void){
    gpio_set(GPIOB, GPIO5);
    
}
void disableMoteurDroit(void){
    gpio_clear(GPIOB, GPIO5);
}


void enableBrakeMoteurGauche(void){
    gpio_clear(GPIOA, GPIO7);
}
void disableBrakeMoteurGauche(void){
    gpio_set(GPIOA, GPIO7);
}

void enableBrakeMoteurDroit(void){
    gpio_clear(GPIOB, GPIO4);
}
void disableBrakeMoteurDroit(void){
    gpio_set(GPIOB, GPIO4);
}