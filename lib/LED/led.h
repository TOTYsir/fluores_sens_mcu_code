#ifndef __LED_H
#define __LED_H

#define LED_PIN A0
#define PWM_CHANNEL 0
#define PWM_FREQ 1000
#define PWM_RESOLUTION 10
#define MAX_DUTY_CYCLE (int)(pow(2, PWM_RESOLUTION) - 1)

#define SAMPLES_PER_ON_CYCLE 35
#define SAMPLES_PER_OFF_CYCLE 15


void led_pwm_init(float dutyCycle);
int led_control(float dutyCycle, int sample_cnt);

#endif
