#include "uart.h"

#include <libopencm3/stm32/usart.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>
#include <stdio.h>



void usart_setup(void){
	/* Enable clocks for USART2. */
	rcc_periph_clock_enable(RCC_USART2);

	/* Setup GPIO pins for USART2 transmit. */
	gpio_mode_setup(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO2);

	/* Setup USART2 TX pin as alternate function. */
	gpio_set_af(GPIOA, GPIO_AF7, GPIO2);

	/* Setup USART2 parameters. */
		usart_set_baudrate(USART2, 115200);
	usart_set_databits(USART2, 8);
	usart_set_stopbits(USART2, USART_STOPBITS_1);
	usart_set_mode(USART2, USART_MODE_TX);
	usart_set_parity(USART2, USART_PARITY_NONE);
	usart_set_flow_control(USART2, USART_FLOWCONTROL_NONE);

	/* Finally enable the USART. */
	usart_enable(USART2);
}

void usart_send_debug(char* Message){
	int i = 0;
	while (Message[i] != 0)	{
		usart_send_blocking(USART2,Message[i]);
		i++;
	}	
}


void usart_send_debugln(char* Message){
	usart_send_debug(Message);
	usart_send_blocking(USART2, '\r');
	usart_send_blocking(USART2,'\n');
}


void usart_send_debugint(int Message){
	char inverse[20];
	int i = 0;
	int negatif = 0;
	if(Message<0){
		Message = -Message;
		negatif = 1;
	}

	do {
		inverse[i] = Message % 10 + '0';
		i++;		
		Message /= 10;
	}while(Message != 0);
	if(negatif == 1){
		inverse[i] = '-';
		i++;
	}
	while (i>0)
	{
		usart_send_blocking(USART2,inverse[i-1]);
		i--;
	}
}

void usart_send_debugintln(int Message){
	usart_send_debugint(Message);
	usart_send_blocking(USART2, '\r');
	usart_send_blocking(USART2,'\n');
}


void usart_send_debugfloat(float Message){
	char Buffer[20];
	sprintf(Buffer,"%f",Message);
	usart_send_debug(Buffer);
}



void usart_send_debugfloatln(float Message){
	usart_send_debugfloat(Message);
	usart_send_blocking(USART2, '\r');
	usart_send_blocking(USART2,'\n');
}

void usart_send_debugdouble(double Message){
	char Buffer[20];
	sprintf(Buffer,"%lf",Message);
	usart_send_debug(Buffer);
}
void usart_send_debugdoubleln(double Message){
	usart_send_debugdouble(Message);
	usart_send_blocking(USART2, '\r');
	usart_send_blocking(USART2,'\n');
}


void usart_send_debugTab(void){
	char buffer[2]={9,0};
	usart_send_debug("      ");
}
