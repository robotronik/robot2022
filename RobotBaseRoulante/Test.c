#include "Test.h"

void Blink(void){
    gpio_toggle(GPIOA, GPIO11);
}

void led_ON(void){
    gpio_set(GPIOA, GPIO11);
}

void led_OFF(void){
    gpio_clear(GPIOA, GPIO11);
}