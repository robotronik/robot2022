#include "i2C.h"
#include "uart.h"
#include "clock.h"

char buffer[256];
int buffercount;
i2CReceiveUnion bufferRecption;


void (*callbacki2cRec)(i2CReceiveUnion reception);
bool callbackinitialise;

bool btransmit = false;
bool breakWhileSend;

uint32_t TimePre;

void i2c_setup(void)
{

	callbackinitialise = false;

	rcc_periph_clock_enable(RCC_I2C1);
	rcc_periph_clock_enable(RCC_GPIOB);
	rcc_set_i2c_clock_hsi(I2C1);

	i2c_reset(I2C1);
	/* Setup GPIO pin GPIO_USART2_TX/GPIO9 on GPIO port A for transmit. */
	gpio_mode_setup(GPIOB, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO6 | GPIO7);
	gpio_set_af(GPIOB, GPIO_AF4, GPIO6 | GPIO7 );
	i2c_peripheral_disable(I2C1);
	//configure ANFOFF DNF[3:0] in CR1
	i2c_enable_analog_filter(I2C1);
	i2c_set_digital_filter(I2C1, 0);
	//interupt
	nvic_enable_irq(NVIC_I2C1_EV_EXTI23_IRQ);
	nvic_set_priority(NVIC_I2C1_EV_EXTI23_IRQ, 16);



	i2c_reset(I2C1);
	i2c_peripheral_disable(I2C1);

	/* HSI is at 8Mhz */
	i2c_set_speed(I2C1, i2c_speed_sm_100k, 8);

	i2c_set_own_7bit_slave_address(I2C1, 120);
	I2C1_OAR1 |= I2C_OAR1_OA1EN_ENABLE;
	i2c_enable_interrupt(I2C1, I2C_CR1_ADDRIE | I2C_CR1_RXIE | I2C_CR1_STOPIE);  //I2C_CR1_ADDRIE | 

	I2C1_CR1 |= (1<<19); //GCEN
	
	i2c_enable_rxdma(I2C1);

	i2c_disable_stretching(I2C1);

	i2c_peripheral_enable(I2C1);

	I2C1_CR1 |= I2C_CR1_RXDMAEN;
	

   

}


void i2c_transferAntiBlocage(uint32_t i2c, uint8_t addr, const uint8_t *w, size_t wn){
         /*  waiting for busy is unnecessary. read the RM */
                 i2c_set_7bit_address(i2c, addr);
                 i2c_set_write_transfer_dir(i2c);
                 i2c_set_bytes_to_transfer(i2c, wn);
                 i2c_disable_autoend(i2c);
                 i2c_send_start(i2c);
				 while (I2C_CR2(i2c)&I2C_CR2_START){
					 usart_send_debug("2");
				 }

				TimePre = get_uptime_ms()+100;
                 while (wn-- && TimePre > get_uptime_ms()) {
                         bool wait = true;
                         while (wait && TimePre > get_uptime_ms()) {
                                 if (i2c_transmit_int_status(i2c)) {
                                         wait = false;
                                 }
								 usart_send_debug("3");
                         }
						 while (i2c_nack(i2c) && TimePre > get_uptime_ms()){
						 	usart_send_debug("4");
						}
                         i2c_send_data(i2c, *w++);
                 }
				 while (!i2c_transfer_complete(i2c) && TimePre > get_uptime_ms());
				 i2c_send_stop(i2c);
				 
 }
	

void i2c_send(uint8_t addr, const uint8_t *w, size_t wn){

	uint8_t data = "";
	usart_send_debug("I");
	//i2c_transfer7(I2C1, addr, w, wn, &data, 0);
	i2c_transferAntiBlocage(I2C1, addr, w, wn);
	usart_send_debug("O");
	btransmit = true;

}



void i2c1_ev_exti23_isr(void){
	uint32_t sr1;
	sr1 = I2C1_ISR;
	if(sr1 & I2C_ISR_ADDR){
		buffercount = 0;
		I2C1_ICR |= I2C_ICR_ADDRCF;
		usart_send_debug("S");
	}
	if(sr1 & I2C_ISR_RXNE){
		uint8_t sr2 = i2c_get_data(I2C1);
		if(buffercount>=0 && buffercount <256){
			//buffer[buffercount] = sr1;
			bufferRecption.tabR[buffercount] = sr2;
			buffercount++;
			usart_send_debug("R");
		}
		
	}
	if(sr1 & I2C_ISR_STOPF){
		if(btransmit==true){
			btransmit = false;
		}
		else{
			if(callbackinitialise!=true && buffercount==8){
				(*callbacki2cRec)(bufferRecption);
				usart_send_debug("Q");
				breakWhileSend = true;
			}
		}		
		I2C1_ICR |= I2C_ICR_STOPCF;
		buffercount = 0;
		usart_send_debug("P");
	}
	//I2C1_ICR = 0xffffffff;
}




void printbit(void){
	buffer[buffercount]=0;
	usart_send_debugln(buffer);
}

void setCallback(void (*f)(i2CReceiveUnion reception)){
	callbacki2cRec = f;
	callbackinitialise =  true;
}

void disableCallback(void){
	callbackinitialise =  false;
}




