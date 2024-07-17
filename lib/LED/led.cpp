#include <Arduino.h>
#include "led.h"

void led_pwm_init(float dutyCycle) {
    ledcSetup(PWM_CHANNEL, PWM_FREQ, PWM_RESOLUTION);
    ledcAttachPin(LED_PIN, PWM_CHANNEL);
    ledcWrite(PWM_CHANNEL, (int)(MAX_DUTY_CYCLE * dutyCycle));
}

int led_control(float dutyCycle, int total_sample_cnt) {
    int led_state = (total_sample_cnt % 210 <= 150) ? HIGH : LOW;

    if (led_state)  
        ledcWrite(PWM_CHANNEL, (int)(MAX_DUTY_CYCLE * dutyCycle));
    else    
        ledcWrite(PWM_CHANNEL, 0);

    if (total_sample_cnt % 210 == 0)    return 1;

    return 0;
}
