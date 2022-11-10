#pragma once

void usart_setup(void);
void usart_send_debug(char* Message);
void usart_send_debugint(int Message);
void usart_send_debugln(char* Message);
void usart_send_debugintln(int Message);
void usart_send_debugfloat(float Message);
void usart_send_debugfloatln(float Message);
void usart_send_debugdouble(double Message);
void usart_send_debugdoubleln(double Message);

void usart_send_debugTab(void);

