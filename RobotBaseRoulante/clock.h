#pragma once
#include <stdint.h>

// RCC Clock Frequency [Hz]
static const int RCC_CLOCK_FREQ_HZ = 64000000;

// Interruptions = 10kHz = 100us (beaucoup ?)
static const int SYSTICK_FREQ_HZ = 1000;
static const int SYSTICK_PERIOD = RCC_CLOCK_FREQ_HZ / SYSTICK_FREQ_HZ;

static const int MICROS_SYSTICK_RATIO = 1000000 / SYSTICK_FREQ_HZ;

#define MILLIS_TO_SYSTICK(ms) (ms * 1000 / MICROS_SYSTICK_RATIO)
// inline constexpr int32_t MILLIS_TO_SYSTICK(uint32_t ms) {
//     return ms * 1000 / MICROS_SYSTICK_RATIO;
// }
// inline constexpr int32_t MICROS_TO_SYSTICK(int32_t us) {
//     return us / MICROS_SYSTICK_RATIO;
// }
// inline constexpr int32_t SYSTICK_TO_MICROS(int32_t tick) {
//     return tick * MICROS_SYSTICK_RATIO;
// }
// inline constexpr int32_t SYSTICK_TO_MILLIS(int32_t tick) {
//     return tick * MICROS_SYSTICK_RATIO / 1000;
// }

// Granularité des PWM = prescaler = 100kHz = 10us
static const int PWM_GRANUL_FREQ_HZ = 100000;
static const int PWM_GRANUL_PERIOD = RCC_CLOCK_FREQ_HZ / PWM_GRANUL_FREQ_HZ;
//static_assert(PWM_GRANUL_PERIOD == 640);
// Fréquence des PWM = 50Hz = 20ms
static const int PWM_FREQ_HZ = 50;
static const int PWM_PERIOD = PWM_GRANUL_FREQ_HZ / PWM_FREQ_HZ;
//static_assert(PWM_PERIOD == 2'000);

// 50kHz and a maximum period of (1<<16)-1 means a minimal detection
// speed of about 3mm/s and a resolution of 0.1mm with a wheel diameter
// of 75mm and 60 pulses per tour
// 64MHz/1280 = 50kHz
static const int HALL_PRESCALER = 1280;
static const int HALL_COUNTER_PERIOD = (1<<16)-1;
// APB1 and APB2 timer frequency are considered to be the same
static const int APBx_TIMER_FREQ_HZ = RCC_CLOCK_FREQ_HZ;
static const int HALL_SAMPLE_FREQ_HZ = APBx_TIMER_FREQ_HZ / HALL_PRESCALER;




void clock_setup();

uint32_t get_frequency();
uint32_t get_systicks();
uint32_t get_uptime_ms();

void delay_ms(uint32_t ms);
